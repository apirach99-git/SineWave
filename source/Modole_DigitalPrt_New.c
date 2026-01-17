/*
 * Modole_DigitalPrt_New.c
 *
 *  Created on: 16 ат.б. 2568
 *      Author: Piyasak
 */




//completed : sampling port/data
//completed : select port
//got data from each port
//completed : final data in loop 10ms
//revised : Byte h/l DIN
//completed : receive logic from port and select with active hive/low
//revised : check active H/L CPU same as terminal logic
//got check port when Tdebounce = 0
//completed : Digital Input with debounce
//completed : setup digital output port
//could select"0" and active high/low of F217
//completed : selector DOUT
//revised : check FMin with gap 0.005 of Fout
//revised : check FMax with gap 0.005 of Fout
//completed : buffer 3chanels of digtal output
//completed : load buffer to O/P HW
//15/5/2012 : added DOUT warning
//15/5/2012 : added DOUT fault OH,LU,OU,OC,OL
//2012-5-31 : moved setup GPIO into main
//2012-6-20 : remove check PID enable? by checking main variable
//2012-7-7 : added check enable? enable drive/acdc by DIN
//2012-7-10 : completed check bit block up/down DIN
//2012-7-10 : completed load data to buffer before up,down by DIN
//2012-7-10 : revised setting concept with pointer
//2012-7-10 : completed up/down by DIN
//2012-7-12 : completed FAR function
//2012-7-12 : completed FDT function
//2012-7-23 : completed selection motor
//2012-9-1 : added current stall DOUT
//2012-9-1 : added function DOUT 19,20 restart/auto reset
//2012-9-4 : added select model(F015=0 disable F207,F208)
//2012-9-4 : added select model(F015=0 disable F207,F208) of DOUT
//2012-9-6 : debugged eep error after reset fault DOUT must be cleared(case 4)
//2012-9-21 : debugged DIN H/L
//2013-3-7 : add check bit system ready in DOUT
//2013-3-8 : only stop state can change motor1/2 by DIN
//2013-3-8 : use PU for checking DOUT max/min
//2013-3-8 : revise checking restart LU
//2013-4-24 : revise DOUT active  LU and stop and hold state
//2013-6-14 : revise DOUT ready,running,stopping with machine status
//2013-6-17 : revise DOUT FAR,FDT with flying start
//2013-7-2 : DOUT = 3-->W-OL
//2013-7-8 : revise DOUT waiting for restarting LU with checking timer
//2013-7-19 : Debug DOUT LU no active when stop

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

//Display table data
#define dash    1
#define hi      2
#define lo      3

//------------Gus----------------
extern Uint16  BufFMax;//pre-buffer for setting in function before data enter
extern Uint16  BufferPreDECIB_FMax;
extern Uint16  BufferPreSIGN_FMax;
//-----Gus-----
#define tm_delay_UpDown   60000
//-----Gus-----
extern Uint16  Tmr_Mode_Set;
struct DGTLPRT_REG DgtlPrt_Reg={0,0,0,0,0};

Uint16 T_DebounceDIN[] = {0,0,0,0,0,0,0};//timer for debounce of each GPIO

struct KEYPADData BuffDgtPrt={0,0,0,0,0,
                              0,0,0,0}; //buffer for run setting in Digital port
struct KEYPADData *CurrentCntr;

//---External variable import--------------------------------------

struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];

extern const Uint16 *Pntr_max[];
extern const Uint16 *Pntr_min[];
extern const Uint16 *Pntr_def[];

extern int Flag_DCbreak;
extern long Freq_DCbreak;

_iq buff_F_DCbreak = 0;

/////////////////////////////////////////////////////////////////////////////////
void interrupt_timerDigitalPrt(void)
{
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
    {
        //check/debounce when input change state with Ts = 1ms
        DgtlPrt_Reg.TMR_DGTLPRT.bit.T_SmpPort++;    //sampling status port = 1ms
        if(DgtlPrt_Reg.TMR_DGTLPRT.bit.T_SmpPort == TsUpdatePrtDIN)
        {
            DgtlPrt_Reg.TMR_DGTLPRT.bit.T_SmpPort = 0;
            Check_DebounceDIN();                        //check the status and debounce
        }
//--------------------------------------------------------------------
        //Update status DOUT to buffer
        //DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30 = UpdateFinalDOUT(grp_num[2].value[16],grp_num[2].value[19]);
        if(grp_num[0].value[14]==1)
        {
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30 = UpdateFinalDOUT(grp_num[2].value[16],grp_num[2].value[19]);
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31 = UpdateFinalDOUT(grp_num[2].value[17],grp_num[2].value[20]);
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32 = UpdateFinalDOUT(grp_num[2].value[18],grp_num[2].value[21]);
        }else if(grp_num[0].value[14]==0)
        {
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30 = UpdateFinalDOUT(grp_num[2].value[16],grp_num[2].value[19]);
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31 = 0;
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32 = 0;
        }

        //Update buffer DOUT to UP port
        /*
        //GpioDataRegs.GPASET.bit.GPIO7 = DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30;
        //GpioDataRegs.GPACLEAR.bit.GPIO7 = ~DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30;*/
        //GpioDataRegs.GPASET.bit.GPIO16 = DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30;
        //GpioDataRegs.GPACLEAR.bit.GPIO16 = ~DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30;

        GPIO_writePin(16, DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT30);
/*
        //GpioDataRegs.GPASET.bit.GPIO9 = DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31;
        //GpioDataRegs.GPACLEAR.bit.GPIO9 = ~DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31;*/
        //GpioDataRegs.GPASET.bit.GPIO12 = DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31;
        //GpioDataRegs.GPACLEAR.bit.GPIO12 = ~DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31;

        GPIO_writePin(35, DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT31);
/*
        //GpioDataRegs.GPASET.bit.GPIO10 = DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32;
        //GpioDataRegs.GPACLEAR.bit.GPIO10 = ~DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32;*/
        //GpioDataRegs.GPASET.bit.GPIO13 = DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32;
        //GpioDataRegs.GPACLEAR.bit.GPIO13 = ~DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32;

    //    GPIO_writePin(37, DgtlPrt_Reg.FLAG_DGTLPRT.bit.Buff_DOUT32);


//----------------------------------------------------------------------------
        DgtlPrt_Reg.TMR_DGTLPRT.bit.T_SmpData++;    //sampling final data = 10ms
        if(DgtlPrt_Reg.TMR_DGTLPRT.bit.T_SmpData == TsUpdateDataDIN)
        {
            //update final data to ram and ready to use
            DgtlPrt_Reg.TMR_DGTLPRT.bit.T_SmpData = 0;
            UpdateFinalDIN();                       //update final data of each port
            //Up/Down by DIN operation
            UpDwnDIN();

            //Selection M1/M2
            SelectMotor();
        }
//--------------------------------------------------------------------
    }
}
//////////////////////////////////////////////////////////////////
void SelectMotor(void)
{
    if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
    {
        if(DgtlPrt_Reg.DataHDIN.bit.SelM2M1==Active)
        {
            CurrentMotor = &DataMotor2;
        }else
        {
            CurrentMotor = &DataMotor1;
        }
    }
}
//////////////////////////////////////////////////////////////////
void UpDwnDIN(void)
{
    struct{
        Uint16 blockUPDWNDIN:1;//buffer for blocking up/down DIN of keypad freq. of freq. function and PID
    }flagUpDwnDIN={0};

    //Uint16 MaxSetDIN=0,MinSetDIN=0;
    Uint16 buffGrp=0,buffSubG=0;
    Uint16 *PntrMaxSet, *PntrMinSet;
    Uint16 *PntrFactMaxSet;
    Uint16 buff=0;

    _iq MaxsetConvtsg=0;


        //---check mode and disable up,down DIN
        if(KeyPad_reg.MODE_FLAG.bit.mode_state==3)//state_setting
        {
            //---check parameter is keypad freq. and PID for blocking up/down by DIN
            if((KeyPad_reg.PNTR_MODE.bit.PntrGrp==3)&&(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==8))
            {
                flagUpDwnDIN.blockUPDWNDIN=1;
            }else if((KeyPad_reg.PNTR_MODE.bit.PntrGrp==7)&&(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==19))
            {
                flagUpDwnDIN.blockUPDWNDIN=1;
            }
        }
        //---
        if((flagUpDwnDIN.blockUPDWNDIN==1) ||
          (DgtlPrt_Reg.DataLDIN.bit.Up==DgtlPrt_Reg.DataLDIN.bit.Down))
        {//case disable up,down DIN->clear data
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.UpDwnDIN    = 0;      //out of setting by DIN
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.SttUpDwnDIN = 0;   //back to first status
            DgtlPrt_Reg.TMR_DGTLPRT.bit.T_CntUpDwn   = 0;      //clear timer

            CurrentCntr = &BuffDgtPrt;
            CurrentCntr->BuffVal  = 0;
            CurrentCntr->BuffDEC  = 0;
            CurrentCntr->BuffSign = 0;
        }
        else
        {
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.UpDwnDIN = 1;      //setting by DIN
            KeyPad_reg.TIMER0.bit.Time_blink      = 0;             //no blink when setting
            KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg   = 0;
    //--------------------------------Gus-----------------------------------------
            //check mode to determind grp/sub grp
            if(grp_num[7].value[0]==1)
            {
                buffGrp = 7;
                buffSubG = 19;
            }
            else
            {
                buffGrp = 3;
                buffSubG = 8;
            }

            Freq_Reg.MainAcc = Freq_Reg.AccT1;
            Freq_Reg.MainDec = Freq_Reg.DecT1;

            //load register to buffer
            CurrentCntr = &BuffDgtPrt;
            CurrentCntr->BuffVal  = grp_num[buffGrp].value[buffSubG-1];
            CurrentCntr->BuffVal  = grp_num[buffGrp].value[buffSubG-1];
            CurrentCntr->BuffFact = grp_num[buffGrp].factor[buffSubG-1];
            CurrentCntr->BuffDEC  = (grp_num[buffGrp].factor[buffSubG-1] & 0x0300)>>8;
            CurrentCntr->BuffSign = (grp_num[buffGrp].factor[buffSubG-1] & 0x3000)>>12;


            PntrMaxSet = &grp_num[buffGrp].value[2];
            PntrMinSet = &grp_num[buffGrp].value[1];
            PntrFactMaxSet = &grp_num[buffGrp].factor[2];
            //PntrFactMinSet = &grp_num[buffGrp].factor[1];

            CurrentCntr->BuffValMax    = *PntrMaxSet;//load original value to buffer for setting
            CurrentCntr->BuffFactMax   = (*PntrFactMaxSet & 0x0300)>>8;//load original value to buffer for setting
            CurrentCntr->BuffSign_Max  = (*PntrFactMaxSet & 0x3000)>>12;//load original value to buffer for setting

            MaxsetConvtsg = ConvStg(*PntrMaxSet,*PntrFactMaxSet);
            //MaxSetDIN =*PntrMaxSet;
            //MinSetDIN =*PntrMinSet;
            UpDownOprDIN(*PntrMaxSet,*PntrMinSet,*PntrFactMaxSet);//function up/Down DIN
            grp_num[buffGrp].value[buffSubG-1]  = CurrentCntr->BuffVal;
            buff = ((CurrentCntr->BuffDEC & 0x0003)<<8);//load current decib
            buff = ((CurrentCntr->BuffSign & 0x0003)<<12)|buff;//load current sign
            ConvAllKeyPad2IQ(buffGrp,(buffSubG-1));
            CurrentCntr->BuffSp_Norm = ConvStg(CurrentCntr->BuffVal,buff);

            if (CurrentCntr->BuffSp_Norm >= MaxsetConvtsg)
            {
                if (CurrentCntr->BuffDEC <= CurrentCntr->BuffFactMax)
                {
                  CurrentCntr->BuffVal   = CurrentCntr->BuffValMax;
                  CurrentCntr->BuffDEC   = CurrentCntr->BuffFactMax;
                  CurrentCntr->BuffSign  = CurrentCntr->BuffSign_Max;
                  grp_num[buffGrp].value[buffSubG-1]  = CurrentCntr->BuffVal;
                  buff = (CurrentCntr->BuffFactMax & 0x0003)<<8;
                  buff = ((CurrentCntr->BuffSign_Max & 0x0003)<<12)|buff;
                  CurrentCntr->BuffSp_Norm = ConvStg(CurrentCntr->BuffValMax, buff);
                  KeyPad_reg.DISP_TAB.bit.Disp_Table = hi;
                  ConvAllKeyPad2IQ(buffGrp,(buffSubG-1));
                 }
            }
                        //--------------------------------Gus-----------------------------------------

        }
}
//////////////////////////////////////////////////////////////////
void UpDownOprDIN(Uint16 MaxSet,Uint16 MinSet, Uint16 Factor)
{
Uint16  x=0,y=0;
x = 1000/pow(10,((Factor & 0x000C)>>2));//mark1100 at first byte for checking how many digit
y = 9999/pow(10,((Factor & 0x000C)>>2));//mark1100 at first byte for checking how many digit

//-----------enter for key down-----------
    if(DgtlPrt_Reg.DataLDIN.bit.Down == Active)
    {

        if(CurrentCntr->BuffSign == 3)//check negative sign
        {
            goto    Up_value;
        }
        else//no sign data
        {
Dn_value:   if(CurrentCntr->BuffDEC<((CurrentCntr->BuffFact & 0x0030)>>4))//check max decib
            {

                if(CurrentCntr->BuffVal > x)//mark for how many digit is used
                {
                    CurrentCntr->BuffVal--;
                }
                else
                {
                    CurrentCntr->BuffDEC++;//increase decib
                    CurrentCntr->BuffVal = y;
                }
            }
            else if(CurrentCntr->BuffVal>MinSet)
            {
                CurrentCntr->BuffVal--;
            }
            else
            {
                CurrentCntr->BuffVal++;
                if(CurrentCntr->BuffSign==3)
                {
                    CurrentCntr->BuffSign = 1;
                }
                else if(CurrentCntr->BuffSign == 1)
                {
                    CurrentCntr->BuffSign = 3;
                }
            }
        }
    }
    else if(DgtlPrt_Reg.DataLDIN.bit.Up == Active)
//-----------enter for key up-----------
    {

        if(CurrentCntr->BuffSign == 3)//check negative sign
        {
            goto    Dn_value;
        }
        else//no sign data
        {
Up_value:   if(CurrentCntr->BuffDEC>((CurrentCntr->BuffFact & 0x00C0)>>6))//check min decib
            {
                if(CurrentCntr->BuffVal == y)
                {
                    CurrentCntr->BuffVal = x;
                    CurrentCntr->BuffDEC--;//decrease decib
                }
                else
                {
                    CurrentCntr->BuffVal++;
                }
            }
            else if(CurrentCntr->BuffVal < MaxSet)
            {
                CurrentCntr->BuffVal++;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
Uint16 UpdateFinalDOUT(Uint16 selector_DOUT,Uint16 DOUT_state)
{
Uint16 buff_dout = 0;
_iq buff_data = 0;
_iq pustartF = _IQ17div(ND2System_Reg.StartingF,MainIQ_Variable.FRef);//per unit start freq.

//_iq buff_F_DCbreak = _IQ17div(ND2System_Reg.DCBreak_Start_Frq,MainIQ_Variable.FRef);

buff_F_DCbreak = _IQ17div(ND2System_Reg.DCBreak_Start_Frq, MainIQ_Variable.FRef);


    switch(selector_DOUT)
    {
        case 0://Check status is ready to run
            /*if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready==1
                    &&ND2System_Reg.STATE_FLAG.bit.State == state_stop)
            {
                buff_dout = 1;
            }*/

            if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready==1
                    &&ChkFault_Reg.DATA_FAULT.all == 0)
            {
                buff_dout = 1;
            }

            break;
        case 1://Check status is running(all except stop)?
            buff_dout = ((ND2System_Reg.STATE_FLAG.bit.State == state_start)
                    ||(ND2System_Reg.STATE_FLAG.bit.State == state_run)
                    ||(ND2System_Reg.STATE_FLAG.bit.State == state_stopping))?1:0;
            break;
        case 2://Check status is stopping?
            buff_dout = (ND2System_Reg.STATE_FLAG.bit.State == state_stopping)?1:0;
            break;
        case 3://Check Current limit
            buff_dout = (ChkFault_Reg.FLAG_FAULT.bit.W_OL == 1)?1:0;
            break;
        case 4://Check status is faulting?
            if(DgtlPrt_Reg.FLAG_DGTLPRT.bit.StDOUTFault==1)
            {
                if(ChkFault_Reg.Fault_Buffer == 0)
                {
                    buff_dout = 0;
                    DgtlPrt_Reg.FLAG_DGTLPRT.bit.StDOUTFault=0;
                }
                buff_dout = DgtlPrt_Reg.FLAG_DGTLPRT.bit.StDOUTFault;
            }
            else
            {
                if(ChkFault_Reg.Fault_Buffer != 0)
                {
                    buff_dout = 1;
                    if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==1
                            &&ND2System_Reg.STATE_FLAG.bit.State == state_stop)
                    {
                        buff_dout = 0;
                    }
                }
                DgtlPrt_Reg.FLAG_DGTLPRT.bit.StDOUTFault=buff_dout;
            }
            break;
        case 5://Check status is overheat?
            buff_dout = (ChkFault_Reg.DATA_FAULT.bit.Fault_OH == 1)?1:0;
            break;
        case 6://Check status is under voltage?
            if(ND2System_Reg.STATE_FLAG.bit.State != state_stop)
            {
                buff_dout = (ChkFault_Reg.DATA_FAULT.bit.Fault_LU == 1)?1:0;
            }
            break;
        case 7://Check status is over voltage?
            buff_dout = (ChkFault_Reg.DATA_FAULT.bit.Fault_OU == 1)?1:0;
            break;
        case 8://Check status is current limit?
            buff_dout = (ND2System_Reg.FLAG_STALL.bit.StallIActive == 1)?1:0;
            break;
        case 9://Check status is over current?
            buff_dout = (ChkFault_Reg.DATA_FAULT.bit.Fault_OC == 1)?1:0;
            break;
        case 10://Check status is thermal overload?
            buff_dout = (ChkFault_Reg.DATA_FAULT.bit.Fault_OL == 1)?1:0;
            break;
        case 11://Check status is SC?
            buff_dout = (ChkFault_Reg.DATA_FAULT.bit.Fault_SC == 1)?1:0;
            break;
        case 12://Check status is accel?
            buff_dout = (ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemAccel)?1:0;
            break;
        case 13://Check status is decel?
            buff_dout = (ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemDecel)?1:0;
            break;
        case 14://Check status is steady?
            buff_dout = (ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemSteady)?1:0;
            break;
        case 15://Check status is FAR?
            ChkFAR_FDT(15);//function check FAR/FDT
            buff_dout = DgtlPrt_Reg.FLAG_DGTLPRT.bit.StateFAR;
            break;
        case 16://Check status is FDT?
            ChkFAR_FDT(16);//function check FAR/FDT
            buff_dout = DgtlPrt_Reg.FLAG_DGTLPRT.bit.StateFDT;
            break;
        case 17://Check status is LowerF?
            if(ND2System_Reg.STATE_FLAG.bit.State != state_stop)
            {
                if(_IQ17abs(MainIQ_Variable.FCommand)<=MainIQ_Variable.PU_FMin)
                {
                    buff_dout = 1;
                }else
                {
                    buff_data = _IQ17abs(MainIQ_Variable.FCommand)-MainIQ_Variable.PU_FMin;
                    if(_IQ17abs(buff_data)<=_IQ17(0.005))
                    {
                        buff_dout = 1;
                    }
                }
            }
            break;
        case 18://Check status is UpperF?
            if(ND2System_Reg.STATE_FLAG.bit.State != state_stop)
            {
                if(_IQ17abs(MainIQ_Variable.FCommand)>=MainIQ_Variable.PU_FMax)
                {
                    buff_dout = 1;
                }else
                {
                    buff_data = MainIQ_Variable.PU_FMax-_IQ17abs(MainIQ_Variable.FCommand);
                    if(_IQ17abs(buff_data)<=_IQ17(0.005))
                    {
                        buff_dout = 1;
                    }
                }

            }
            break;
        case 19://Check status is Auto-Restarting?
            buff_dout = (ChkFault_Reg.TimerLU!=0)?1:0;
            break;
        case 20://Check status is ready to reset?
            buff_dout = (ChkFault_Reg.Timer_ResetFlt>=(grp_num[4].value[25]*1000))?1:0;
            break;
        case 21://Inverter running 2 (Add the output freq. condition for analog input12)
            buff_dout = ((ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
                    ||(MainIQ_Variable.FCommandA>pustartF))?1:0;
            break;
        case 22:
            if (ND2System_Reg.DCBreak_Time != 0)
            {
                if ((ND2System_Reg.STATE_FLAG.bit.State == state_start)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_run))
                {
                    buff_dout = 1;
                }
                else if (ND2System_Reg.STATE_FLAG.bit.State == state_stopping
                        &&MainIQ_Variable.FCommandA>=buff_F_DCbreak)
                {
                    buff_dout = 1;
                }
                else if (ND2System_Reg.STATE_FLAG.bit.State == state_stopping
                        &&MainIQ_Variable.FCommandA<=buff_F_DCbreak)
                {
                    buff_dout = 0;
                }

            }
            else if (ND2System_Reg.DCBreak_Time == 0)
            {
                buff_dout = ((ND2System_Reg.STATE_FLAG.bit.State == state_start)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_run)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_stopping))?1:0;
            }
            break;
    }

    buff_dout = (DOUT_state == 0)?buff_dout:~buff_dout;
    return buff_dout;
}
//////////////////////////////////////////////////////////////////
void ChkFAR_FDT(Uint16 selector)
{
    struct {
        Uint16 bufffar:1;
        Uint16 bufffdt:1;
        Uint16 disablefarfdt:1;
    }flagfarfdt={0};

    _iq PUHystFAR  = _IQ17div(MainIQ_Variable.FAR_Hyst,MainIQ_Variable.FRef);
    _iq PULevelFDT = _IQ17div(MainIQ_Variable.FDT_Level,MainIQ_Variable.FRef);
    _iq PUHystFDT  = _IQ17div(MainIQ_Variable.FDT_Hyst,MainIQ_Variable.FRef);

        if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive == 1
                ||ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero == 1)
        {
            flagfarfdt.disablefarfdt = 1;
        }

        if(flagfarfdt.disablefarfdt == 0
                && ((ND2System_Reg.STATE_FLAG.bit.State == state_start)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_run)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_stopping)))
        {
            switch (selector)
            {
            case 15:
                //---Check FAR
                if(MainIQ_Variable.FAR_Hyst != 0)
                {
                    if(_IQ17abs((_IQ17abs(MainIQ_Variable.FCommand)-_IQ17abs(MainIQ_Variable.FSet)))
                            <=PUHystFAR)
                    {
                        flagfarfdt.bufffar=1;
                        DgtlPrt_Reg.FLAG_DGTLPRT.bit.StateFAR = flagfarfdt.bufffar;
                    }
                    else if(_IQ17abs((_IQ17abs(MainIQ_Variable.FCommand)-_IQ17abs(MainIQ_Variable.FSet)))
                            >PUHystFAR)
                    {
                        flagfarfdt.bufffar=0;
                        DgtlPrt_Reg.FLAG_DGTLPRT.bit.StateFAR = flagfarfdt.bufffar;
                    }

                }
                break;
            case 16:
                //---Check FDT
                   if(MainIQ_Variable.FDT_Hyst != 0)
                   {
                       if(_IQ17abs(MainIQ_Variable.FCommand)<(PULevelFDT-PUHystFDT))
                       {
                           flagfarfdt.bufffdt=0;
                           DgtlPrt_Reg.FLAG_DGTLPRT.bit.StateFDT = flagfarfdt.bufffdt;
                       }
                       else if(_IQ17abs(MainIQ_Variable.FCommand)>=PULevelFDT)
                       {
                           flagfarfdt.bufffdt=1;
                           DgtlPrt_Reg.FLAG_DGTLPRT.bit.StateFDT = flagfarfdt.bufffdt;
                       }
                   }
                break;
            }
        }

}
//////////////////////////////////////////////////////////////////
void UpdateFinalDIN(void)
{
Uint16 Buff_Setting = 0,i;
Uint16 Buff_PrtSel = 0;//port selector in for loop
Uint32 Buff_Data = 0,Buff_DIN = 0;
struct{
    Uint16 enbdrvenb:1; //enable DIN drive enable(DIN=0)
    Uint16 enbacdc:1;   //enable DIN ACDC(DIN=16,17)
}flagDIN={0};

    for(i=0; i<=6; i++)//loop update DIN FWD->X5
    {
        Buff_PrtSel = 1;                                //start at least port
        Buff_PrtSel = Buff_PrtSel<<i;                   //shift to current port

        Buff_Data = DgtlPrt_Reg.DataPrtDIN.bit.DataGPIO_DIN & Buff_PrtSel;      //mark data with current port
        Buff_Data = Buff_Data>>i;                       //shift back to LSB for re-shift with setting parameter again

        Buff_Setting = grp_num[2].value[1+i];   //load current setting function
        //---
        if(Buff_Setting==0)
        {
            flagDIN.enbdrvenb = 1;                      //check any port set as enable drive?
        }

        if((Buff_Setting==16)||(Buff_Setting==17))
        {
            flagDIN.enbacdc = 1;                        //check any port set as enable ac/dc command?
        }
        //---
        Buff_Data = Buff_Data<<Buff_Setting;            //shift data to setting position
        Buff_DIN = Buff_DIN | Buff_Data;                //load data to to buffer
    }

    DgtlPrt_Reg.FLAG_DGTLPRT.bit.EnbDINDrvEnb = flagDIN.enbdrvenb;
    DgtlPrt_Reg.FLAG_DGTLPRT.bit.EnbDINACDC   = flagDIN.enbacdc;

    DgtlPrt_Reg.DataLDIN.all = Buff_DIN & 0x0000FFFF;       //load final data low byte from buffer to data ram
    DgtlPrt_Reg.DataHDIN.all = (Buff_DIN & 0x001F0000)>>16; //load final data high byte from buffer to data ram
}
//////////////////////////////////////////////////////////////////
void Check_DebounceDIN(void)
{
    Uint16 buff_GPADAT = 0, i;
    Uint16 Buff_PrtSel = 0;//port selector in for loop
    //Uint16 buff_DATAGPIO = 0;

        //check logic from GPIO and check with active high/low
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = 0;

        buff_GPADAT = (GpioDataRegs.GPADAT.bit.GPIO17 != grp_num[2].value[8])?1:0;//
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | buff_GPADAT;

        buff_GPADAT = (GpioDataRegs.GPBDAT.bit.GPIO58 != grp_num[2].value[9])?1:0;
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | (buff_GPADAT<<1);

        buff_GPADAT = (GpioDataRegs.GPADAT.bit.GPIO25 != grp_num[2].value[10])?1:0;
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | (buff_GPADAT<<2);

        buff_GPADAT = (GpioDataRegs.GPBDAT.bit.GPIO34 != grp_num[2].value[11])?1:0;
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | (buff_GPADAT<<3);

        buff_GPADAT = (GpioDataRegs.GPADAT.bit.GPIO23 != grp_num[2].value[12])?1:0;
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | (buff_GPADAT<<4);

        buff_GPADAT = (GpioDataRegs.GPADAT.bit.GPIO6 != grp_num[2].value[13])?1:0;
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | (buff_GPADAT<<5);

        buff_GPADAT = (GpioDataRegs.GPADAT.bit.GPIO7 != grp_num[2].value[14])?1:0;
        DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN | (buff_GPADAT<<6);
        //Check big/small model
        if(grp_num[0].value[14]==0)
        {
            DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN = DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN & 0x1F;
        }
    //-----------------------------------------------------------------------------------------
        //loop debounce 7GPIOs
        for(i=0; i<=6; i++)
        {
            Buff_PrtSel = 1;
            Buff_PrtSel = Buff_PrtSel<<i;//shift to current port

            if((DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN & Buff_PrtSel)
                    == (DgtlPrt_Reg.DataPrtDIN.bit.DataGPIO_DIN & Buff_PrtSel))//check logic change or not?
            {
                T_DebounceDIN[i] = 0;//if data not change, will clear timer
            }
            else
            {
                if(T_DebounceDIN[i]==grp_num[2].value[15])
                {
                    T_DebounceDIN[i] = 0;

                    DgtlPrt_Reg.DataPrtDIN.bit.DataGPIO_DIN =
                            (DgtlPrt_Reg.DataPrtDIN.bit.DataGPIO_DIN & ~Buff_PrtSel)
                            |(DgtlPrt_Reg.DataPrtDIN.bit.BuffGPIO_DIN & Buff_PrtSel);
                }
                else
                {
                    T_DebounceDIN[i]++;
                }
            }
        }
    //-----------------------------------------------------------------------------------------
}
