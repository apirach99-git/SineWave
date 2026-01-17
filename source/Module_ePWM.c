/*
 * Module_ePWM.c
 *
 *  Created on: Nov 21, 2022
 *      Author: Piyasak
 */

//2013-5-10 : add function bootstrap by set IO on/of
//2013-5-11 : completed IO Bootstrap
//2013-5-14 : completed bootstrap
//2013-5-25 : add bit check shutdown output fault to reset PWM Power pin
//2013-7-5 : add check shutdown op for PWM
//2013-7-8 : revise check shut-off PWM algorithm
//2013-8-15 : move config A2D PWM1 to this file
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2012-09-08  :   Add F006 is PWM Output
                    F007 is Deadtime1
                    F008 is Deadtime2
                    F015 is Active of PWM
                    F016 is Frequency On PWM

2013-06-08  :       F518 is Carrier frequency
                    F519 is Motor Sound
                    F523 is Compensete DC bus
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//2014-3-1 : Satulate PWM Module of Phase A, Phase B and Phase C
//2014-3-1Rev1 : Make Two Arms Modulation
//2014-3-8 : Two Arms Modulation Function
//              F539    Enable Two Arms Modulation
//              F540    Enable Two Arms Frequency
//              F541    Disable Two Arms Frequency
//2014-3-21 : Add 5 types of two arm modulation. (F542)
//              F542 = 0 ;>>>>Clamp Neg
//              F542 = 1 ;>>>>Clamp Pos
//              F542 = 2 ;>>>>DPWM0
//              F542 = 3 ;>>>>DPWM2
//              F542 = 4 ;>>>>DPWM3
//              F542 = 5 ;>>>>DPWM1


#include "F28x_Project.h"     // DSP280x Headerfile Include File
#include "ND2.h"



//void ePWM_Setup(void);
//void Run_ePWM(void);
//void ConfigPWM2GPIO(void);

int ar_period[] = {25000,12500,8334,6250,5000,
                   4166,3572,3126,2778,2382,
                   2274,2084,1924,1786,1690};

struct data_grp{
   Uint16 value[100];
   Uint16 factor[100];
};
extern struct data_grp grp_num[8] ;
//void HPWM(void);

struct  PWM_COMMAN{
   long va_com;
   long vb_com;
   long vc_com;
};
extern struct  PWM_COMMAN PWM_COM;

int     Period_PWM;
float  Period_PWM_f;


float  va_com_f = 0, vb_com_f = 0,vc_com_f = 0;

int Vmax = 0,Vmin = 0;
long VmaxPU = 0;
long VminPU = 0;
float K0 = 1.0;
long V1HPWM = 0,V2HPWM = 0,V3HPWM = 0,VoHPWM = 0,tempHPWM = 0;
long Voa = 0, Vob = 0, Voc = 0;
int FunctionHPWM = 0,PeriodPWMMid = 0;

struct HPWM_REG Hpwm_Reg= {0,0,0};



/*##################################################################################################
Calculate Time-Base Period Register (TBPR).
When count up and down mode.                |            When Up-count mode and Down-count mode
                                            |
                         T_PWM              |
 TBPR = Period_PWM = -------------          |             TBPR = Period_PWM = (T_PWM / T_TBCLK) -1
                      2 * T_TBCLK           |

                SYSCLKOUT
 T_TBCLK = --------------------
            HSPCLKDIV * CLKDIV


 SYSCLKOUT = XCLKOUT = 10 ns
 HSPCLKDIV = 1/2
 CLKDIV    = 1

 So T_TBCLK = (10ns)/(1*1/2) = 20ns
-------------------------------------------------------------------------------------------------------

 Ex. Find TBPR when period of PWM equal 100 us (10kHz) in count up and down mode.
                         100 us
 TBPR = Period_PWM = ------------- = 2500
                        2 * 20ns

//int Period_PWM = 2500;//10kHz
//int Period_PWM = 5000;//5kHz

####################################################################################################*/

/*############################### Dead-Band Delay Values ##########################################

FED = DBFED * T_TBCLK
RED = DBRED * T_TBCLK

DBFED, DBRED        TBCLK = SYSCLKOUT /2
    1                   0.02 us
    5                   0.10 us
    10                  0.20 us
    100                 2.00 us
    200                 4.00 us
    300                 6.00 us
    400                 8.00 us
    500                 10.00 us
    600                 12.00 us
    700                 14.00 us
    800                 16.00 us
    900                 18.00 us
    1000                20.00 us
############################### Dead-Band Delay Values ##########################################*/

extern int va ;
extern int vb ;
extern int vc ;

int Vmid = 0;
int va_HPWM = 0;
int vb_HPWM = 0;
int vc_HPWM = 0;
int Vdc_LPF = 0,Vdc = 0;

extern int Vdcbus;
extern int Flag_DCbreak;//From Function_DCBreak.c

int flag_PWMEnable    = 0;
char Count_PWMdisable = 0;
int DeadTime = 0;
long Check_ABS_FCommand = 0;

int Period_PWM_old = 0;
long Random_reg    = 0;
int count_random   = 0;

int FlagTwoArmEna = 0;

//long SPWM = 5000;

void Adjust_PWM(void)
{
    if(Flag_DCbreak == 0)
    {

/*
        if(MainIQ_Variable.FCommand>=0)
        {
            Check_ABS_FCommand = MainIQ_Variable.FCommand;
        }
        else
        {
            Check_ABS_FCommand = -MainIQ_Variable.FCommand;
        }
*/


        if(grp_num[5].value[33] == 1 && grp_num[5].value[0] == 4)//F534 = 1 is Torque Control and F501 = 4 Vector
        {
            if(TrqCmnd_Reg.TrqCommand>=0)
                Check_ABS_FCommand = TrqCmnd_Reg.TrqCommand;
            else
                Check_ABS_FCommand = -TrqCmnd_Reg.TrqCommand;
        }
        else//F534 = 0 is Torque Control
        {
            if(MainIQ_Variable.FCommand>=0)
                Check_ABS_FCommand = MainIQ_Variable.FCommand;
            else
                Check_ABS_FCommand = -MainIQ_Variable.FCommand;
        }
    }

//Disable PWM
    if(grp_num[0].value[5] == 0
            ||ND2System_Reg.STATE_FLAG.bit.State == state_stop
            ||ND2System_Reg.STATE_FLAG.bit.ShtdwnOP==1)
    {
        goto chkPWMEn;
    }

    else if(BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp==0)
    {
        goto chkPWMEn;
    }
    //----------GUS-----------------
    else if(((Check_ABS_FCommand<=MainIQ_Variable.FreqCut_PU)
            &&(MainIQ_Variable.FreqCut_PU!=0))
            && (ND2System_Reg.STATE_FLAG.bit.State == state_start
                    || ND2System_Reg.STATE_FLAG.bit.State == state_run))
    //----------GUS-----------------
    {
    chkPWMEn:
        if(flag_PWMEnable!=2)
        {
            ConfigPWM2GPIO();
            flag_PWMEnable = 2;//Off PWM
        }
        Count_PWMdisable = flag_PWMEnable;
        if(BtStrp_reg.BTSTRP_FLAG1.bit.TrnOnBtStrp==1)
        {
            if(BtStrp_reg.BTSTRP_FLAG1.bit.OnX==1)
            {
                /*
                GpioDataRegs.GPACLEAR.bit.GPIO3=1;  //Off Y
                GpioDataRegs.GPACLEAR.bit.GPIO5=1;  //Off Z
                GpioDataRegs.GPASET.bit.GPIO1=1;    //On X
                */
                GPIO_writePin(3, 0);
                GPIO_writePin(5, 0);
                GPIO_writePin(1, 1);
            }

            else if(BtStrp_reg.BTSTRP_FLAG1.bit.OnY==1)
            {
                /*
                GpioDataRegs.GPACLEAR.bit.GPIO1=1;  //Off X
                GpioDataRegs.GPACLEAR.bit.GPIO5=1;  //Off Z
                GpioDataRegs.GPASET.bit.GPIO3=1;    //On Y
                */
                GPIO_writePin(1, 0);
                GPIO_writePin(5, 0);
                GPIO_writePin(3, 1);
            }

            else if(BtStrp_reg.BTSTRP_FLAG1.bit.OnZ==1)
            {
                /*
                GpioDataRegs.GPACLEAR.bit.GPIO1=1;  //Off X
                GpioDataRegs.GPACLEAR.bit.GPIO3=1;  //Off Y
                GpioDataRegs.GPASET.bit.GPIO5=1;    //On Z
                */
                GPIO_writePin(1, 0);
                GPIO_writePin(3, 0);
                GPIO_writePin(5, 1);
            }
            //GpioDataRegs.GPBSET.bit.GPIO33 = 1;
            GPIO_writePin(33, 1);
        }

        else
        {//in the case of after complete bootstrap
            if(GpioDataRegs.GPBDAT.bit.GPIO33 == 1)
            //if (GPIO_readPin(33))
            {
                /*
                GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;//'1'ON '0'OFF
                GpioDataRegs.GPACLEAR.bit.GPIO0=1;
                GpioDataRegs.GPACLEAR.bit.GPIO1=1;
                GpioDataRegs.GPACLEAR.bit.GPIO2=1;
                GpioDataRegs.GPACLEAR.bit.GPIO3=1;
                GpioDataRegs.GPACLEAR.bit.GPIO4=1;
                GpioDataRegs.GPACLEAR.bit.GPIO5=1;
                */

                GPIO_writePin(33, 0);
                GPIO_writePin(0, 0);
                GPIO_writePin(1, 0);
                GPIO_writePin(2, 0);
                GPIO_writePin(3, 0);
                GPIO_writePin(4, 0);
                GPIO_writePin(5, 0);
            }
        }
    }

    else//Enable PWM
    {
        flag_PWMEnable = 1;//On PWM

        if((Count_PWMdisable - flag_PWMEnable) !=0)//It is doing 1 Time.
        {

            EALLOW;

            while(GpioCtrlRegs.GPAMUX1.bit.GPIO0 == 0)
            {
                //GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO6 as EPWM4A
                //GPIO_setPadConfig(0, GPIO_PIN_TYPE_PULLUP);
                GPIO_setPinConfig(GPIO_0_EPWM1_A);
                //GPIO_setQualificationMode(0, GPIO_QUAL_SYNC);
            }
            while(GpioCtrlRegs.GPAMUX1.bit.GPIO1 == 0)
            {
                //GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO8 as EPWM5A
                //GPIO_setPadConfig(1, GPIO_PIN_TYPE_PULLUP);
                GPIO_setPinConfig(GPIO_1_EPWM1_B);
                //GPIO_setQualificationMode(1, GPIO_QUAL_SYNC);
            }
            while(GpioCtrlRegs.GPAMUX1.bit.GPIO2 == 0)
            {
                //GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO6 as EPWM4A
                //GPIO_setPadConfig(2, GPIO_PIN_TYPE_PULLUP);
                GPIO_setPinConfig(GPIO_2_EPWM2_A);
                //GPIO_setQualificationMode(2, GPIO_QUAL_SYNC);
            }
            while(GpioCtrlRegs.GPAMUX1.bit.GPIO3 == 0)
            {
                //GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO8 as EPWM5A
                //GPIO_setPadConfig(3, GPIO_PIN_TYPE_PULLUP);
                GPIO_setPinConfig(GPIO_3_EPWM2_B);
                //GPIO_setQualificationMode(3, GPIO_QUAL_SYNC);
            }
            while(GpioCtrlRegs.GPAMUX1.bit.GPIO4 == 0)
            {
                //GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO6 as EPWM4A
                //GPIO_setPadConfig(4, GPIO_PIN_TYPE_PULLUP);
                GPIO_setPinConfig(GPIO_4_EPWM3_A);
                //GPIO_setQualificationMode(4, GPIO_QUAL_SYNC);
            }
            while(GpioCtrlRegs.GPAMUX1.bit.GPIO5 == 0)
            {
                //GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO8 as EPWM5A
                //GPIO_setPadConfig(5, GPIO_PIN_TYPE_PULLUP);
                GPIO_setPinConfig(GPIO_5_EPWM3_B);
                //GPIO_setQualificationMode(5, GPIO_QUAL_SYNC);
            }

            EDIS;
        }
        Count_PWMdisable = flag_PWMEnable;
        //GpioDataRegs.GPBSET.bit.GPIO33 = 1;
        GPIO_writePin(33, 1);
    }
//=================F006 PWM OUTPUT and F016 Frequency On PWM=============================

//=====================F007 DeadTime1 and F008 DeadTime2=================================
    if(MainIQ_Variable.DeadTime1 == MainIQ_Variable.DeadTime2)
    {
        //DeadTime = _IQ17toF(MainIQ_Variable.DeadTime1)*50;
        DeadTime = (float)MainIQ_Variable.DeadTime1 * 0.00038146972656f;
        EPwm1Regs.DBFED.bit.DBFED = DeadTime;
        EPwm1Regs.DBRED.bit.DBRED = DeadTime;

        EPwm2Regs.DBFED.bit.DBFED = DeadTime;
        EPwm2Regs.DBRED.bit.DBRED = DeadTime;

        EPwm3Regs.DBFED.bit.DBFED = DeadTime;
        EPwm3Regs.DBRED.bit.DBRED = DeadTime;

    }
    else
    {
        DeadTime = 300;//6us
        EPwm1Regs.DBFED.bit.DBFED = DeadTime;
        EPwm1Regs.DBRED.bit.DBRED = DeadTime;

        EPwm2Regs.DBFED.bit.DBFED = DeadTime;
        EPwm2Regs.DBRED.bit.DBRED = DeadTime;

        EPwm3Regs.DBFED.bit.DBFED = DeadTime;
        EPwm3Regs.DBRED.bit.DBRED = DeadTime;

    }
/*##################################################################################################
Calculate Time-Base Period Register (TBPR).
When count up and down mode.             |            When Up-count mode and Down-count mode
                                         |
                        T_PWM            |
 TBPR = Period_PWM = -------------       |           TBPR = Period_PWM = (T_PWM / T_TBCLK) -1
                      2 * T_TBCLK        |

                SYSCLKOUT
 T_TBCLK = --------------------
         HSPCLKDIV * CLKDIV


 SYSCLKOUT = XCLKOUT = 10 ns
 HSPCLKDIV = 1/2
 CLKDIV      = 1

 So T_TBCLK = (10ns)/(1*1/2) = 20ns
-------------------------------------------------------------------------------------------------------

 Ex. Find TBPR when period of PWM equal 100 us (10kHz) in count up and down mode.
                         100 us
 TBPR = Period_PWM = ------------- = 2500
                        2 * 20ns

####################################################################################################*/

    if(grp_num[5].value[29] == 0)//F530 Motor Sound defualt
    {
        //===============F529 Carrier frequency===============
        Period_PWM = ar_period[grp_num[5].value[28]-1];
    }
//================F530 Motor Sound Tone 1 - Tone 3====================================
    else
    {
//      Period_PWM_old = 6250;//4kHz
        Period_PWM_old = ar_period[grp_num[5].value[28]-1];
        if(Period_PWM_old <= 3126)
        {
            Period_PWM_old = 3126;
        }


        if(grp_num[5].value[29] == 1)//=======================Random 1
        {
            if(count_random>=2)
            {
                count_random = 0;
                Random_reg = (AdcbResultRegs.ADCRESULT3 + AdcaResultRegs.ADCRESULT5 - AdcaResultRegs.ADCRESULT3 +
                            AdccResultRegs.ADCRESULT4 - AdccResultRegs.ADCRESULT1 + AdcbResultRegs.ADCRESULT4 -
                            AdcbResultRegs.ADCRESULT1 + AdcaResultRegs.ADCRESULT4 - AdcaResultRegs.ADCRESULT1 +
                            AdccResultRegs.ADCRESULT3 - AdccResultRegs.ADCRESULT0 + AdccResultRegs.ADCRESULT2 -
                            AdcbResultRegs.ADCRESULT0 + AdcbResultRegs.ADCRESULT2 - AdcaResultRegs.ADCRESULT0 +
                            AdcaResultRegs.ADCRESULT2)>>4;

                Random_reg = (Random_reg & 0x0000008F)<<1;
                if(Random_reg & 0x0001 == 1)
                {
                    Period_PWM = Period_PWM_old - (Random_reg);
                }
                else
                {
                    Period_PWM = Period_PWM_old + (Random_reg);
                }
            }
            count_random++;
        }

        else
        {
            if(grp_num[5].value[29] == 2)//===================Random 2
            {
                if(count_random>=5)
                {
                    count_random = 0;
                    Random_reg = (  AdcbResultRegs.ADCRESULT3 + AdcaResultRegs.ADCRESULT5 - AdcaResultRegs.ADCRESULT3+
                            AdccResultRegs.ADCRESULT4 - AdccResultRegs.ADCRESULT1 + AdcbResultRegs.ADCRESULT4 -
                            AdcbResultRegs.ADCRESULT1 + AdcaResultRegs.ADCRESULT4 - AdcaResultRegs.ADCRESULT1 +
                            AdccResultRegs.ADCRESULT3 - AdccResultRegs.ADCRESULT0 + AdccResultRegs.ADCRESULT2 -
                            AdcbResultRegs.ADCRESULT0 + AdcbResultRegs.ADCRESULT2 - AdcaResultRegs.ADCRESULT0 +
                            AdcaResultRegs.ADCRESULT2 )>>4;

                    Random_reg = (Random_reg & 0x000006F)<<3;
                    if(Random_reg & 0x0001 == 1)
                    {
                        Period_PWM = Period_PWM_old + (Random_reg);
                    }
                    else
                    {
                        Period_PWM = Period_PWM_old - (Random_reg);
                    }
                }
                count_random++;
            }
            else//============================================Random 3
            {
                if(count_random>=3)
                {
                    count_random = 0;
                    Random_reg = (AdcbResultRegs.ADCRESULT3 + AdcaResultRegs.ADCRESULT5 - AdcaResultRegs.ADCRESULT3+
                            AdccResultRegs.ADCRESULT4 - AdccResultRegs.ADCRESULT1 + AdcbResultRegs.ADCRESULT4 -
                            AdcbResultRegs.ADCRESULT1 + AdcaResultRegs.ADCRESULT4 - AdcaResultRegs.ADCRESULT1 +
                            AdccResultRegs.ADCRESULT3 - AdccResultRegs.ADCRESULT0 + AdccResultRegs.ADCRESULT2 -
                            AdcbResultRegs.ADCRESULT0 + AdcbResultRegs.ADCRESULT2 - AdcaResultRegs.ADCRESULT0 +
                            AdcaResultRegs.ADCRESULT2)>>4;

                    Random_reg = (Random_reg & 0x0000006F)<<2;
                    if(Random_reg & 0x0001 == 1)
                    {
                        Period_PWM = Period_PWM_old - (Random_reg);
                    }
                    else
                    {
                        Period_PWM = Period_PWM_old + (Random_reg);
                    }
                    count_random++;
                }
            }
        }
    }
//================F530 Motor Sound Tone 1 - Tone 3====================================

//      Period_PWM = SPWM;

        EPwm1Regs.TBPRD = Period_PWM;
        EPwm2Regs.TBPRD = Period_PWM;
        EPwm3Regs.TBPRD = Period_PWM;
}
////////////////////////////////////////////////////////////////////////////////////////////
void ConfigPWM2GPIO(void)
{
    //GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;//'1'ON '0'OFF     (ON/OFF PWM)
    GPIO_writePin(33, 0);

    //EALLOW;

    GPIO_setPadConfig(0, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(1, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(2, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(3, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_PULLUP);
/*
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;   // Configure GPIO6 as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;   // Configure GPIO8 as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;   // Configure GPIO6 as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;   // Configure GPIO8 as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;   // Configure GPIO6 as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;   // Configure GPIO8 as I/O
*/
    GPIO_setPinConfig(GPIO_0_GPIO0);
    GPIO_setPinConfig(GPIO_1_GPIO1);
    GPIO_setPinConfig(GPIO_2_GPIO2);
    GPIO_setPinConfig(GPIO_3_GPIO3);
    GPIO_setPinConfig(GPIO_4_GPIO4);
    GPIO_setPinConfig(GPIO_5_GPIO5);

/*
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; //1-output 0-input
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1; //1-output 0-input
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; //1-output 0-input
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1; //1-output 0-input
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1; //1-output 0-input
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1; //1-output 0-input
*/

    GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(2, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(3, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_OUT);

/*
    GpioDataRegs.GPACLEAR.bit.GPIO0=1;
    GpioDataRegs.GPACLEAR.bit.GPIO1=1;
    GpioDataRegs.GPACLEAR.bit.GPIO2=1;
    GpioDataRegs.GPACLEAR.bit.GPIO3=1;
    GpioDataRegs.GPACLEAR.bit.GPIO4=1;
    GpioDataRegs.GPACLEAR.bit.GPIO5=1;
*/
    GPIO_writePin(0, 0);
    GPIO_writePin(1, 0);
    GPIO_writePin(2, 0);
    GPIO_writePin(3, 0);
    GPIO_writePin(4, 0);
    GPIO_writePin(5, 0);
    //EDIS;
}
////////////////////////////////////////////////////////////////////////////////////////////
void HPWM(void)
{
    _iq buff_vdcbus;
//####################################################
//      High Bridge Pulse Width Modulation
//####################################################


//=================F531 Compensete DC bus==========================
    if(grp_num[5].value[30] == 1)//F531
    {
//      Vdcbus = ((long)VDC_Read_reg.PUVDC>>5);//Q17-Q5 = Q12
        //Low pass filter at 10ms
        //Vdcbus = _IQ12mpy(81,(VDC_Read_reg.PUVDC>>5)) + _IQ12mpy(4016,Vdcbus);

        //Vdcbus = _IQ12mpy(60,(VDC_Read_reg.PUVDC>>5)) + _IQ12mpy(4036,Vdcbus);
        //Vdcbus = _IQ12mpy(81,(VDC_Read_reg.PUVDC>>5)) + _IQ12mpy(4015,Vdcbus);

        if (VDC_Read_reg.CntAvgPUVDC > 4095)
        {
            buff_vdcbus = VDC_Read_reg.BuffVDCcomp>>12;
            Vdcbus = ((long)buff_vdcbus>>5);
            VDC_Read_reg.CntAvgPUVDC = 0;
            VDC_Read_reg.BuffVDCcomp = 0;
        }
        else
        {
            VDC_Read_reg.CntAvgPUVDC++;
            VDC_Read_reg.BuffVDCcomp = VDC_Read_reg.BuffVDCcomp+(VDC_Read_reg.PUVDC);
        }

    }
    else
    {
/*
        if (VDC_Read_reg.CntAvgPUVDC >= 4095)
        {
            buff_vdcbus = VDC_Read_reg.BuffVDCcomp>>12;
            Vdcbus = ((long)buff_vdcbus>>5);
            VDC_Read_reg.CntAvgPUVDC = 0;
            VDC_Read_reg.BuffVDCcomp = 0;
        }
        else
        {
            VDC_Read_reg.CntAvgPUVDC++;
            VDC_Read_reg.BuffVDCcomp = VDC_Read_reg.BuffVDCcomp+(VDC_Read_reg.PUVDC);
        }
*/

        //Vdcbus = ((long)VDC_Read_reg.PUVDC>>5);//Q17-Q5 = Q12

        if (grp_num[0].value[4]==0)
        {
            Vdcbus = _IQ12div(_IQ17(300),MainIQ_Variable.V_base);
        }
        else
        {
            Vdcbus = _IQ12div(_IQ17(555),MainIQ_Variable.V_base);
        }


     }
//=================F531 Compensete DC bus==========================


//   va_com = (Period_PWM>>1) + _IQ1div(_IQ1mpy(Period_PWM,va),Vdcbus);//Vdc = 350 Vrms = 2867 Q12
//   vc_com = (Period_PWM>>1) + _IQ1div(_IQ1mpy(Period_PWM,vc),Vdcbus);

   float Period_PWM_f   = (float)Period_PWM;   // ค่า period เป็น float
   float vc_f                   = (float)vc;            // ค่า sine reference
   float va_f                   = (float)va;            // ค่า sine reference
   float Vdcbus_f           = (float)Vdcbus;        // ค่า Vdc bus ที่สเกลแล้ว

   va_com_f = (Period_PWM_f * 0.5f) + (Period_PWM_f * va_f) / Vdcbus_f;
   vc_com_f = (Period_PWM_f * 0.5f) + (Period_PWM_f * vc_f) / Vdcbus_f;


    PWM_COM.va_com = (Uint16)va_com_f;
    PWM_COM.vb_com = (Uint16)vb_com_f;
    PWM_COM.vc_com = (Uint16)vc_com_f;

//---------Limit Pulse Width Modulation---------------------
    PWM_COM.va_com = fmaxf(0, fminf(PWM_COM.va_com, Period_PWM));
//    PWM_COM.vb_com = fmaxf(0, fminf(PWM_COM.vb_com, Period_PWM));
    PWM_COM.vc_com = fmaxf(0, fminf(PWM_COM.vc_com, Period_PWM));
//---------Limit Pulse Width Modulation---------------------

//========================================================


   return;
}
////////////////////////////////////////////////////////////////////////////////////////////
void Run_ePWM(void)
{
    Adjust_PWM();

    HPWM();

//  EPwm1Regs.CMPA.bit.CMPA = 1000;
//  EPwm2Regs.CMPA.bit.CMPA = 250; // adjust duty for output EPWM2A
//  EPwm3Regs.CMPA.bit.CMPA = 2000; // adjust duty for output EPWM3A


//=============================================================================
//          Satulate PWM Module of Phase A, Phase B and Phase C
//=============================================================================
    if(flag_PWMEnable == 1) //On PWM
    {
    //Phase A
        if(PWM_COM.va_com >= Period_PWM)
        {
            EALLOW;
            GpioCtrlRegs.GPAMUX1.bit.GPIO0   = 0;// Configure as I/O
            GpioCtrlRegs.GPADIR.bit.GPIO0    = 1;//1-output 0-input
            GpioDataRegs.GPACLEAR.bit.GPIO0  = 1;//output = "0"

            GpioCtrlRegs.GPAMUX1.bit.GPIO1   = 0;// Configure as I/O
            GpioCtrlRegs.GPADIR.bit.GPIO1    = 1;//1-output 0-input
            GpioDataRegs.GPASET.bit.GPIO1    = 1;//output = "1"
            EDIS;
        }
        else
        {
            if(PWM_COM.va_com <= 0)
            {
                EALLOW;
                GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;// Configure as I/O
                GpioCtrlRegs.GPADIR.bit.GPIO0  = 1;//1-output 0-input
                GpioDataRegs.GPASET.bit.GPIO0  = 1;//output = "1"

                GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;// Configure as I/O
                GpioCtrlRegs.GPADIR.bit.GPIO1  = 1;//1-output 0-input
                GpioDataRegs.GPACLEAR.bit.GPIO1= 1;//output = "0"
                EDIS;
            }
            else
            {
                EALLOW;
                GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;//Configure PWM
                GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;//Configure PWM
                EDIS;
                EPwm1Regs.CMPA.bit.CMPA = PWM_COM.va_com; // adjust duty for output EPWM1A
            }
        }

    //Phase B
        if(PWM_COM.vb_com >= Period_PWM)
        {
            EALLOW;
            GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;// Configure as I/O
            GpioCtrlRegs.GPADIR.bit.GPIO2   = 1;//1-output 0-input
            GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;//output = "0"

            GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;// Configure as I/O
            GpioCtrlRegs.GPADIR.bit.GPIO3   = 1;//1-output 0-input
            GpioDataRegs.GPASET.bit.GPIO3   = 1;//output = "1"
            EDIS;
        }
        else
        {
            if(PWM_COM.vb_com <= 0)
            {
                EALLOW;
                GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;// Configure as I/O
                GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;//1-output 0-input
                GpioDataRegs.GPASET.bit.GPIO2  = 1;//output = "1"

                GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;// Configure as I/O
                GpioCtrlRegs.GPADIR.bit.GPIO3   = 1;//1-output 0-input
                GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;//output = "0"
                EDIS;
            }
            else
            {
                EALLOW;
                GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;//Configure PWM
                GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;//Configure PWM
                EDIS;
                EPwm2Regs.CMPA.bit.CMPA = PWM_COM.vb_com; // adjust duty for output EPWM2A
            }
        }

    //Phase C
        if(PWM_COM.vc_com >= Period_PWM)
        {
            EALLOW;
            GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 0;// Configure as I/O
            GpioCtrlRegs.GPADIR.bit.GPIO4   = 1;//1-output 0-input
            GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;//output = "0"

            GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;// Configure as I/O
            GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;//1-output 0-input
            GpioDataRegs.GPASET.bit.GPIO5  = 1;//output = "1"
            EDIS;
        }
        else
        {
            if(PWM_COM.vc_com <= 0)
            {
                EALLOW;
                GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;// Configure as I/O
                GpioCtrlRegs.GPADIR.bit.GPIO4  = 1;//1-output 0-input
                GpioDataRegs.GPASET.bit.GPIO4  = 1;//output = "1"

                GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 0;// Configure as I/O
                GpioCtrlRegs.GPADIR.bit.GPIO5   = 1;//1-output 0-input
                GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;//output = "0"
                EDIS;
            }
            else
            {
                EALLOW;
                GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;//Configure PWM
                GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;//Configure PWM
                EDIS;
                EPwm3Regs.CMPA.bit.CMPA = PWM_COM.vc_com; // adjust duty for output EPWM3A
            }
        }

    }
//=============================================================================
//      End Satulate PWM Module of Phase A, Phase B and Phase C
//=============================================================================

    EPwm1Regs.TBPHS.bit.TBPHS = 0;
    EPwm2Regs.TBPHS.bit.TBPHS = 0;
    EPwm3Regs.TBPHS.bit.TBPHS = 0;

    return;
}
////////////////////////////////////////////////////////////////////////////////////////////
void ePWM_Setup(void)
{
    //SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);

    EALLOW;

    Period_PWM = 5000;  //5kHz = 5000       10kHz = 2381        15kHz = 1667
                        //4kHz = 6250       9kHz = 2778         14kHz = 1786
                        //3kHz = 8333       8kHz = 3125         13kHz = 1923
                        //2kHz = 12500      7kHz = 3571         12kHz = 2083
                        //1kHz = 25000      6kHz = 4167         11kHz = 2273
    EALLOW;                                 // This is needed to write to EALLOW protected register
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC=0;    //stop timer for setting PWM
    EDIS;

    EALLOW;
    // Initialization Time
    //========================// EPWM Module 1 config
    EPwm1Regs.TBPRD                = Period_PWM; // Period = 1600 TBCLK counts
    EPwm1Regs.TBPHS.bit.TBPHS      = 0; // Set Phase register to zero
    EPwm1Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;// Symmetrical mode
    EPwm1Regs.TBCTL.bit.PHSEN      = TB_DISABLE; // Master module
    EPwm1Regs.TBCTL.bit.PRDLD      = TB_SHADOW;
    EPwm1Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO; // Sync down-stream module
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm1Regs.AQCTLA.bit.CAU       = AQ_SET; // set actions for EPWM1A
    EPwm1Regs.AQCTLA.bit.CAD       = AQ_CLEAR;
    EPwm1Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE; // enable Dead-band module
    EPwm1Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC; // Active Hi complementary
    EPwm1Regs.DBFED.bit.DBFED      = 50; // FED = 50 TBCLKs
    EPwm1Regs.DBRED.bit.DBRED      = 50; // RED = 50 TBCLKs
    EPwm1Regs.ETSEL.bit.SOCAEN     = 1;  // Enable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL    = 1;  // Select SOC from from CPMA on upcount
    EPwm1Regs.ETPS.bit.SOCAPRD     = 1;  // Generate pulse on 1st event

    // EPWM Module 2 config
    EPwm2Regs.TBPRD                = Period_PWM; // Period = 1600 TBCLK counts
    EPwm2Regs.TBPHS.bit.TBPHS      = 0; // Set Phase register to zero
    EPwm2Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;// Symmetrical mode
    EPwm2Regs.TBCTL.bit.PHSEN      = TB_ENABLE; // Slave module
    EPwm2Regs.TBCTL.bit.PRDLD      = TB_SHADOW;
    EPwm2Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO; // Sync down-stream module
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm2Regs.AQCTLA.bit.CAU       = AQ_SET; // set actions for EPWM2A
    EPwm2Regs.AQCTLA.bit.CAD       = AQ_CLEAR;
    EPwm2Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE; // enable Dead-band module
    EPwm2Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC; // Active Hi complementary
    EPwm2Regs.DBFED.bit.DBFED      = 50; // FED = 50 TBCLKs
    EPwm2Regs.DBRED.bit.DBRED      = 50; // RED = 50 TBCLKs

    // EPWM Module 3 config
    EPwm3Regs.TBPRD                = Period_PWM; // Period = 1600 TBCLK counts
    EPwm3Regs.TBPHS.bit.TBPHS      = 0; // Set Phase register to zero
    EPwm3Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;// Symmetrical mode
    EPwm3Regs.TBCTL.bit.PHSEN      = TB_ENABLE; // Slave module
    EPwm3Regs.TBCTL.bit.PRDLD      = TB_SHADOW;
    EPwm3Regs.TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO; // Sync down-stream module
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm3Regs.AQCTLA.bit.CAU       = AQ_SET; // set actions for EPWM3A
    EPwm3Regs.AQCTLA.bit.CAD       = AQ_CLEAR;
    EPwm3Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE; // enable Dead-band module
    EPwm3Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC; // Active Hi complementary
    EPwm3Regs.DBFED.bit.DBFED      = 50; // FED = 50 TBCLKs
    EPwm3Regs.DBRED.bit.DBRED      = 50; // RED = 50 TBCLKs


    // Run Time (Note: Example execution of one run-time instant)
    //=========================================================
    // EPWM Module 6 config
    //SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);

    EPwm6Regs.TBPRD                = 2500; // Period = 1600 TBCLK counts
    EPwm6Regs.TBPHS.bit.TBPHS      = 0; // Set Phase register to zero
    EPwm6Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;// Symmetrical mode
    EPwm6Regs.TBCTL.bit.PHSEN      = TB_DISABLE; // Slave module
    EPwm6Regs.TBCTL.bit.PRDLD      = TB_SHADOW;
    EPwm6Regs.TBCTL.bit.SYNCOSEL   = TB_SYNC_DISABLE; // Disable EPWMxSYNCO signal
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm6Regs.AQCTLA.bit.CAU       = AQ_SET; // set actions for EPWM3A
    EPwm6Regs.AQCTLA.bit.CAD       = AQ_CLEAR;
    EPwm6Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE; // enable Dead-band module
    EPwm6Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC; // Active Hi complementary
    EPwm6Regs.DBFED.bit.DBFED      = 150; // FED = 50 TBCLKs
    EPwm6Regs.DBRED.bit.DBRED      = 150; // RED = 50 TBCLKs
    EPwm6Regs.ETSEL.bit.INTSEL     = ET_CTR_ZERO;//interrupt when counter=0
    EPwm6Regs.ETSEL.bit.INTEN      = 1;
    EPwm6Regs.ETPS.bit.INTPRD      = ET_1ST;//second time of interrupt to 50us*2
    EDIS;

    EALLOW;                                 // This is needed to write to EALLOW protected register
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;    //run timer for setting PWM
    EDIS;
    return;
}
/////////////////////////////////////////////////////////////////////////////////////////////
