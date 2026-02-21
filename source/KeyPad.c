//2012-4-25 debugged : clear bit fault eep1 and buffer table fault after correct data
//2012-4-25 debugged : no reset bit KeyPad.KEY_FLAG.bit.state_key1 to avoid data key
//2012-4-25 added : convert keypad format to IQ format F020
//2012-4-25 completed : display VDC bus
//2012-4-25 debug : display table
//2012-4-25 revised : display table show fault only normal mode
//2012-4-25 revised : switch display normal/setting mode when fault
//2012-4-25 revised : can setting when fault
//2012-4-25 revised : eep1 after revise table
//2012-5-21 revised : once eep when check range start value with default
//2012-5-21 revised : KeyPad_reg.KEY_FLAG.bit.state_key1=1 after key stop active(solve the problem can press
//					  key when auto reset fault by DIN) 
//2012-6-2 removed : check group3 max/min/F308
//2012-6-5 revised : normal mode F308
//2012-6-5 revised locking key when system is running
//2012-6-5 revised F806 couldnt modify
//2012-6-11 added : converse IQ F637/F638
//2012-6-14 revised : normal mode-> can count setting PID by up/dn key
//2012-6-14 could count in normal mode
//2012-6-14 could count/save/converse in normal mode
//2012-6-14 added save/not save data follow F621
//2012-6-15 revised : key data to rotate display "BufDispNorm"
//2012-6-15 control blinking in normal mode
//2012-6-15 revised display in normal mode output
//2012-6-15 debugged after load default by user need to re-converse IQ
//2012-6-15 added converse keypad to IQ F411-F414
//2012-6-16 completed up/down in normal mode for PID
//2012-6-16 remove converse IQ F637,F638
//2012-6-15 added converse IQ F345/F346
//2012-6-19 added converse IQ F341-F344
//2012-6-20 revised converse IQ multiF PID with for loop
//2012-6-20 completed converse IQ F311-F325
//2012-6-20 converse IQ Grp3
//2012-6-20 completed converse IQ grp3
//2012-6-20 revised converse IQ grp3 with exit grp
//2012-6-21 added conv. IQ F223-F240
//2012-6-25 revised converse iq grp4 with loop for
//2012-6-25 added cal.pu-fmax,pu-fmin
//2012-7-2 completed normal mode with setting exept PID
//2012-7-2 completed normal mode
//2012-7-3 debuged converse IQ grp3 with sign of multF
//2012-7-6 added display L/R follow F201
//2012-7-9 added display FER
//2012-7-10 added display for setting by DIN
//2012-7-12 completed converse IQ F255-F260
//2012-7-12 changed stepdown in counter
//2012-7-13 completed converse IQ F514-F516
//2012-7-13 revised IQ F514-F516 to ND2System_Reg
//2012-7-17 : revised keypad up/down operation 
//2012-7-17 : revised display show hi/lo
//2012-7-18 : revised display hi/lo with time change speed-200
//2012-7-18 : got down F620 form 200 to 100 but from 100 to -1.00
//2012-7-18 : got down F620 form 100 to 0 but from 0 to -2.00
//2012-7-18 : completed Up/Dn counter by using bit "KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg"
//			  check up/dn or not?			
//2012-7-18 : revised KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg=0 when up new speed
//2012-7-23 : added converse all IQ grp1
//2012-7-23 : revised converse IQ grp1 with seperate data motor1/2 
//2012-7-23 : completed converse data motor 1/2 with seperate data
//2012-7-23 : completed converse IQ F416,F417
//2012-7-23 : revised smoother running counter with checking reach the counter
//2012-7-30 : revised display I with multiply sqrt(2) of F004
//2012-8-1 : add display Vout
//2012-8-7 : add converse IQ F003
//2012-8-7 : got flag R charge C
//2012-8-7 : display not ready
//2012-8-9 : revised display blinking WOL/WOC
//2012-8-10 : debugged display WOC by clearing bit statusB
//2012-8-15 : add converse IQ F021,F022 temporaly
//2012-8-18 : add display DC brk
//2012-8-20 : revised display �Ѵ���
//2012-8-30 : add converse IQ F420
//2012-8-31 : add function read VR key
//2012-9-1 : add function display RPM
//2012-9-1 : add converse IQ F522
//2012-9-1 : add function displpay F520,F522
//2012-9-3 : moved function VR Keypad to ADC
//2012-9-3 : debugged save data fault before clear in eep1
//2012-9-3 : revised save data fault last is [0]
//2012-9-4 : completed by pass hiden sun-function for big/small model
//2012-9-6 : reset WD for normal operation(loaded e2prom,no error e2)
//2012-9-6 : added reset WD for normal case(loaded e2p,no error)
//2012-9-12 : added waiting for keypad before set bit "KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd"
//2012-9-25 : added blinking fault data 
//2012-9-26 : added function reset/set eCAN
//2012-9-26 : completed function select boud rate CAN
//2012-9-26 : move bit CAN connected at TA
//2012-9-29 : add load default F103/F112 by F004 data
//2012-10-1 : revised key run/stop active on single press and clear fault hold press for 2sec.
//2012-10-1 : added LED VR for keypad 7seg.
//2012-10-1 : revised clear key when no key by and 0x0300
//2012-10-2 : revised normal mode display but not complted from LPF error
//2012-10-2 : revised LPF in normal display
//2012-10-2 : completed filter display
//2012-10-12 : revised lock key enable and need to setting when initial(over-range)
//2012-10-15 : add wait offset A2D before set bit initKeypad
//2012-10-15 : remove load orginal data to LPF for display
//2012-10-30 : add skip grp vector
//2012-11-2 : revised display before system ready
//2012-11-7 : add reset bit check cal PU when load default 
//2012-11-7 : add blinking LED run under first test
//2012-11-22 : debugged no load minimum data when run cntr setting until timer trg=0
//2012-11-30 : add converse IQ F424-F427
//2012-12-14 : revised converse iq F244-F254 by loop for
//2012-12-14 : revised converse iq F605-F611 by loop for
//2012-12-14 : revised converse iq F613-F620 by loop for
//2013-1-10 : completed got variable Sp during up/down operation
//2013-1-28 : add display actual speed
//2013-2-9 : add converse iq F722-F724
//2013-2-15 : move I2C E2prom into i2c.c and revise algorithm
//2013-2-15 : move function uploading/downloading into backup.c
//2013-2-20 : revise display FWD/REV
//2013-2-20 : revise display RPM with +-
//2013-2-22 : debug F520 maxset=maxset-minmaingrp
//2013-3-5 : debug display LdF first time
//2013-3-5 : debug add check "KeyPad_reg.MODE_FLAG.bit.mode_state==0" in check mode 
//2013-3-5 : debug add check "MODE_FLAG[mode_state]==0"," MODE_FLAG[setting_mode]=1",clear key program in check mode
//2013-3-5 : add check bit test run for display
//2013-3-16 : use absrpm to display and check sign
//2013-4-1 : no reset Syst_ready when load default
//2013-4-23 : replace LPF RPM with QEP_Reg.ImdABSRPM_UTO
//2013-5-2 : check model for displaying minus speed 
//2013-5-6 : add bit test port and LED&Data blink same time
//2013-5-14 : revise converse F405,F407
//2013-5-20 : add firmware version with date release
//2013-5-21 : use general test instead of toggle port
//2013-5-27 : add up/down F301=0,5,6,7,8
//2013-5-27 : add converse IQ F503-F509,F511-F516
//2013-5-28 : add load minimum setting
//2013-5-28 : add load maximum setting
//2013-6-5 : Revised Grp8 with function grp ND2
//2013-6-7 : Revise Grp0,1 with function grp ND2
//2013-6-8 : Add hotkey for hardreset
//2013-6-8 : Add delay waiting for keypad
//2013-6-8 : revise converse IQ grp5
//2013-6-8 : revise converse IQ grp6
//2013-6-10 : revise optimize memory on converse grp0
//2013-6-10 : revise optimize memory on converse grp2
//2013-6-10 : remove sign+- for display actual speed
//2013-6-10 : revise optimize memory on converse grp2
//2013-6-11 : revise optimize memory on converse grp3
//2013-6-11 : use fault buffer eep1,eep2
//2013-6-12 : revise display blinking
//2013-6-12 : revise load minimum value
//2013-6-17 : add display FLY
//2013-6-22 : add no save e2prom F532
//2013-7-23 : add converse IQ F030-F033
//2013-7-24 : run "DISP_FLAG1[BufDispNorm]"
//2013-7-24 : display temp 4channels
//2013-7-25 : Cancell clear DATAH/L display in"ClrStSetting1"
//2013-8-1 : Debug can not cancel clear DATAH/L in "ClrStSetting1"
//2013-9-2 : add converse IQ torque sp/acc/dec
//2013-9-3 : add display torque
//2013-9-4 : add display FWD/REV with torque command
//2013-9-14 : revise display torque as %
//2013-10-07 : debug display temp
//2013-10-07 : display torque command with Imotor
//2013-11-12 : revise display FWD/REV
//2023-03-14 : LED status of the VR is active when the F301 is set to 2

//factory passward 9944

#include "F28x_Project.h"
#include <math.h>
#include "include\ND2.h"
#include "include\DSP280x_I2C_defines.h"
#include "include\KeyPad.h"				//header for "const" variable
#include "f28004x_sw_prioritized_isr_levels.h"

struct KEYPAD_REG KeyPad_reg={0,0,0,0,0,
							  0x00010000,0,0,0,0x0002,
							  0,0x0040,0,0,0,
							  0,0,0,0,0,
							  0,0,0};

//-----------------------------------------------------------------
//---External variable import--------------------------------------
extern int Flag_DCbreak;
extern int First_Test_ND2;
extern Uint16 arA2dBuff[];
extern int direction;
extern int Speed_Feedback;
extern Uint32 MSG_MBX1DataL, MSG_MBX1DataH, BufferMBX0L;
extern int flag_PWMEnable;
//----------------------------------------------------------------- 

//----------------------
extern float Ls;
extern float Rr;
extern float Rs;
extern float Lm;
extern float SigmaLs;
extern int AutoTuneFinish;

extern volatile bool lcd_refresh_req;
//----------------------

//----------------------------------------Gus
//----------------------------------------Time out for hot key
Uint16 TImeOutHotKey = 0;
int CountTime = 0;
//long Tp1,Tp2;
float fp1;
Uint16 buffGrp=0, buffSubG=0, buff=0;

_iq BUFF_ANA_A12 = 0;
_iq BUFF_NEW_ANA_A12 = 0;


_iq Test_buff_I_rate = 0;

struct {
Uint16 save_data:1;
Uint16 activeupdn:1;
Uint16 activeA12:1;
}flagnorm={0};

Uint16 T_DebounceKeypad[] = {0,0,0,0,0,0,0};//timer for debounce of each GPIO
Uint16 timer_button_index;

//////////////////////////////////////////////////////////////////
void Record_fault(void)
{
    Uint16 i=0,j=0x02;
    if(KeyPad_reg.GENERAL_FLAG.bit.new_fault == 1)
    {
        KeyPad_reg.GENERAL_FLAG.bit.new_fault = 0;
        for(i=0; i<=3; i++)
        {
            ar_fault[4-i] = ar_fault[3-i];
            if(ChkFault_Reg.DATA_FAULT.bit.Fault_EEP2==0)//e2prom fail so do not write anyway
            {
                WriteE2prom(0x00, j, ar_fault[4-i]);//write shift e2prom
            }
            j+=2;
        }
        ar_fault[0] = ChkFault_Reg.Fault_Buffer;
        if(ChkFault_Reg.DATA_FAULT.bit.Fault_EEP2==0)
        {
            WriteE2prom(0x00, 0x0A, ar_fault[0]);//write data fault 4th
        }
    }
}
//////////////////////////////////////////////////////////////////
void initE2Prom(void)
{
Uint16 i=0,j=0;

    KeyPad_reg.PNTR_MODE.bit.PntrGrp = 0;
    KeyPad_reg.E2PROM_ADRS.all       = 0x0100;
    DispTab(52);

    KeyPad_reg.E2PROM_BUFF_CDRR.all = ReadE2prom(0x00, 0x00);//adrsH,adrsL,numbyte
    if(KeyPad_reg.E2PROM_BUFF_CDRR.all==0xCCCC)
    {
        while (TImeOutHotKey < 2000)
        {
            ServiceDog();
            /*
            if(KeyPad_reg.DelayKeypad<50)// Wait for keypad connection
            {                                                   // to checking hotkey hard reset
                ServiceDog();
            }*/
            if((BufferMBX0L & 0x000000FF)==maindefault)
            {
                goto harddreset;
            }

        }
//------------------------------------------------------------------------
reread:
       ServiceDog();
       Run_AdrsGRP();

       if(KeyPad_reg.PNTR_MODE.bit.PntrGrp<GrpNumber &&
          KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
       {
            KeyPad_reg.E2PROM_BUFF_CDRR.all = ReadE2prom(KeyPad_reg.E2PROM_ADRS.bit.Adrs_H,
                                                         KeyPad_reg.E2PROM_ADRS.bit.Adrs_L-2);//adrsH,adrsL,numbyte
            *PntrDataE2Prom = KeyPad_reg.E2PROM_BUFF_CDRR.all;
            goto reread;
       }
//----load fault from e2prom-------------------------------------------------
       for(i=0,j=0x0A; i<=4 && KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0; i++,j-=0x02)
       {
        KeyPad_reg.E2PROM_BUFF_CDRR.all = ReadE2prom(0x00,j);//adrsH,adrsL,numbyte
        ar_fault[i] = KeyPad_reg.E2PROM_BUFF_CDRR.all;
       }
    }

    else//---------Hard reset
    {
harddreset:
        LoadDefaultE2(0);//write default all value to e2prom start from group0
    }
    init_keypad();//initial value&condition
} 
//////////////////////////////////////////////////////////////////
void ConvIQ(_iq data, Uint16 maxdec,Uint16 mindec)
{
_iq buffdata       =_IQ17abs(data);
Uint16 dec=0,pow_dec = 0;//,cnt_dec=1
int32 int_part     = 0;
_iq buff1          = 0;
float32 pow_maxdec = 0.0;

    pow_dec     = pow(10,dec);
    pow_maxdec  = pow(10.0,maxdec);//pow_maxdec = 10^maxdec
    buff1       = _IQ17frac(buffdata);//extract fraction part for checking <>0.5
    buff1       = _IQ17mpy(buff1,_IQ17(pow_maxdec));
    buff1       = _IQ17frac(buff1);
    if(buff1>=_IQ17(0.5))
    {
        buffdata = buffdata-_IQ17div(buff1,_IQ17(pow_maxdec));//1863.999878->1863.90000 if max dec = 1
        buffdata = buffdata+_IQ17div(_IQ17(1.01),_IQ17(pow_maxdec));//�Ѵ��� ��ҷȹ��� decmax >5
    }

    int_part = _IQ17int(buffdata);

    if(int_part<10)
    {
        dec = 3;
    }else if(int_part<100)
    {
        dec = 2;
    }else if(int_part<1000)
    {
        dec = 1;
    }else
    {
        dec = 0;
    }

    if(dec>=maxdec)
    {
        dec = maxdec;
    }else if(dec<=mindec)
    {
        dec = mindec;
    }

//run_data_int:
    pow_dec         = pow(10,dec);
    buffdata        = _IQ17mpy(buffdata,_IQ17(pow_dec));//convert iq to uint16 no dec for display and dec controlled dec
    int_part        = _IQ17int(buffdata);
    Norm_Val_Disp   = int_part;
    Norm_Dec_Disp   = dec;
}
//////////////////////////////////////////////////////////////////

//int CountTime = 0;
void Normal_mode(void)
{
    _iq PUHyst  = _IQ17(0.05);
    //_iq BUFF_ANA_A12 = 0;
    //_iq Buff_NewValue_ANA_A12;
    //uint8_t ANA12_Adjust_Flag;


/*    //long Tp1,Tp2;
    float fp1;
Uint16 buffGrp=0,buffSubG=0,buff=0;
struct {
    Uint16 save_data:1;
    Uint16 activeupdn:1;
}flagnorm={0};
*/
    if(AutoTuneFinish == 1)
    {

//      Rr = 2.345;
//      Ls = 1.111;
//      Rs = 3.098;
//      Lm = 0.234;
//      SigmaLs = 0.123;

        CountTime++;

        switch(CountTime)
        {
            case 1://F109--->Lr
                grp_num[1].value[8] = _IQ17(Ls)*1000>>17;

                ClrStSetting1();
                WriteUserE2prom(1,9,grp_num[1].value[8],2033);
                ConvAllKeyPad2IQ(1,8);
                ClrStSetting1();
                break;
            case 2://F110--->Rr
                grp_num[1].value[9] = _IQ17(Rr)*1000>>17;

                WriteUserE2prom(1,10,grp_num[1].value[9],2033);
                ConvAllKeyPad2IQ(1,9);
                ClrStSetting1();
                break;
            case 3://F111--->Ls
                grp_num[1].value[10] = _IQ17(Ls)*1000>>17;

                WriteUserE2prom(1,11,grp_num[1].value[10],2033);
                ConvAllKeyPad2IQ(1,10);
                ClrStSetting1();
                break;
            case 4://F112--->Rs
                grp_num[1].value[11] = _IQ17(Rs)*1000>>17;

                WriteUserE2prom(1,12,grp_num[1].value[11],2033);
                ConvAllKeyPad2IQ(1,11);
                ClrStSetting1();
                break;
            case 5://F113--->Lm
                grp_num[1].value[12] = _IQ17(Lm)*1000>>17;

                WriteUserE2prom(1,13,grp_num[1].value[12],2033);
                ConvAllKeyPad2IQ(1,12);
                ClrStSetting1();
                break;
            case 6://F114--->Sigma
                grp_num[1].value[13] = _IQ1div(_IQ17(SigmaLs)*1000,_IQ17(Ls))>>1;//F114

                WriteUserE2prom(1,14,grp_num[1].value[13],2033);
                ConvAllKeyPad2IQ(1,13);
                ClrStSetting1();
                break;
            case 7://F514 Calculate P control for current loop  (SigmaLs*Ibase)/(2*Ts*Vbase)
//              Tp1 = _IQ17mpy(_IQ17(SigmaLs),MainIQ_Variable.I_base);//                (SigmaLs*Ibase)
//              Tp2 = _IQ26mpy(26844,MainIQ_Variable.V_base);//26844 = 200us Q27::::::: (2*Ts*Vbase)
//              fp1 = _IQ17toF(_IQ17div(Tp1,Tp2));

                fp1 = Ls*417;   //L*625 = L/(2*4*Damp^2*T), Damp = 1, T = 200e-6, 2 is a compensation factor.
                                //L*312 = L/(4*4*Damp^2*T), Damp = 1, T = 200e-6, 4 is a compensation factor.
                                //L*417 = L/(3*4*Damp^2*T), Damp = 1, T = 200e-6, 3 is a compensation factor.
                                //L*156 = L/(8*4*Damp^2*T), Damp = 1, T = 200e-6, 8 is a compensation factor.

                if(fp1>=9.99)//----->Limit
                    fp1=9.99;
                else
                {
                    if(fp1<=0.0)
                        fp1 = 0.0;
                }
                grp_num[5].value[13] = _IQ17(fp1)*1000>>17;//F114

                WriteUserE2prom(5,14,grp_num[5].value[13],grp_num[5].factor[13]);
                ConvAllKeyPad2IQ(5,13);
                ClrStSetting1();

                break;
            case 8://F515 Calculate I control for current loop  K = (Rs*Ibase)/(2*Ts*Vbase), T = 1/K
//              Tp1 = _IQ17mpy(_IQ17(Rs),MainIQ_Variable.I_base);//(SigmaLs*Ibase)
//              Tp2 = _IQ26mpy(26844,MainIQ_Variable.V_base);//26844 = 200us Q27::::::: (2*Ts*Vbase)
//              fp1 = _IQ17toF(_IQ17div(Tp2,Tp1));// T = (2*Ts*Vbase)/(Rs*Ibase)

                //fp1 = Ls/Rs;
                fp1 = Ls/Rs*4;


                if(fp1>=9.99)//----->Limit
                    fp1=9.99;
                else
                {
                    if(fp1<=0.0)
                        fp1 = 0.0;
                }
                grp_num[5].value[14] = _IQ17(fp1)*1000>>17;//F114

                WriteUserE2prom(5,15,grp_num[5].value[14],grp_num[5].factor[14]);
                ConvAllKeyPad2IQ(5,14);
                ClrStSetting1();
                break;
            case 9://F518--->IdCom
                grp_num[5].value[17] = (CurrentMotor->I_Motor*100)>>18;// Irate/2

                WriteUserE2prom(5,18,grp_num[5].value[17],grp_num[5].factor[17]);
                ConvAllKeyPad2IQ(5,17);
                ClrStSetting1();
                break;
            case 10://F512--->IdMax
                grp_num[5].value[11] = (CurrentMotor->I_Motor*100)>>17;// IMax = Irate

                WriteUserE2prom(5,12,grp_num[5].value[11],grp_num[5].factor[11]);
                ConvAllKeyPad2IQ(5,11);
                ClrStSetting1();
                break;
            case 11://F513--->IdMin
                grp_num[5].value[12] = (CurrentMotor->I_Motor*100)>>17;// IMax = Irate

                WriteUserE2prom(5,13,grp_num[5].value[12],grp_num[5].factor[12]);
                ConvAllKeyPad2IQ(5,12);
                ClrStSetting1();
                break;
            case 12://9
                AutoTuneFinish = 0;
                break;
        }


    }
    else
        CountTime = 0;

    if(KeyPad_reg.MODE_FLAG.bit.setting_mode == 0)
    {
        //----------------------------------
        //Initial Function
        if(grp_num[7].value[0]==1)
        {
            if(grp_num[7].value[3]==0)
            {
                flagnorm.activeupdn = 1;
                buffGrp  = 7;
                buffSubG = 19;
                flagnorm.save_data = grp_num[7].value[19];
            }

        }
        else
        {
            if(grp_num[3].value[0]==0||
               grp_num[3].value[0]==5||
               grp_num[3].value[0]==6||
               grp_num[3].value[0]==7||
               grp_num[3].value[0]==8)
            {
                flagnorm.activeupdn = 1;
                buffGrp  = 3;
                buffSubG = 8;
                flagnorm.save_data = grp_num[3].value[9];
            }
            //-------------------------Gus--------------------------
            /*else if (grp_num[3].value[0] == 1
                    && (ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemAccel
                            || ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemDecel))
            {
                KeyPad_reg.DISP_FLAG1.bit.BufDispNorm   = 0;
                KeyPad_reg.DISP_FLAG1.bit.Bckup_DispSel = KeyPad_reg.DISP_FLAG1.bit.BufDispNorm;
            }*/
            /*else if ((grp_num[3].value[0]==1)
                    && (ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemAccel
                    || ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemDecel))
            {
                flagnorm.ActiveVolumn = 1;
                Tmr_Norm_Set = tm_delay_norm;//clear timer 3sec after setting auto save
                KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 1;
                KeyPad_reg.MODE_FLAG.bit.norm_state = 1;

                if(KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg==0)
                {
                    KeyPad_reg.MODE_FLAG.bit.norm_state = 0;
                }
            }

            else if(KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg==0)
            {
                KeyPad_reg.MODE_FLAG.bit.norm_state = 0;
            }*/
            //-------------------------Gus--------------------------
        }

        //change display online after user change
        if(KeyPad_reg.DISP_FLAG1.bit.Bckup_DispSel != grp_num[6].value[0])
        {
            KeyPad_reg.DISP_FLAG1.bit.BufDispNorm   = grp_num[6].value[0];
            KeyPad_reg.DISP_FLAG1.bit.Bckup_DispSel = KeyPad_reg.DISP_FLAG1.bit.BufDispNorm;
        }
        //----------------------------------
        //if(KeyPad_reg.KEY_FLAG.bit.key_up ==1 || KeyPad_reg.KEY_FLAG.bit.key_dwn==1)
        if((KeyPad_reg.KEY_FLAG.bit.key_up == 1
                || KeyPad_reg.KEY_FLAG.bit.key_dwn == 1)
                &&(ND2System_Reg.STATE_FLAG.bit.State == state_run
                    ||ND2System_Reg.STATE_FLAG.bit.State == state_start
                    ||ND2System_Reg.STATE_FLAG.bit.State == state_stopping
                    ||flagnorm.save_data == 1))
        {

            if(flagnorm.activeupdn == 1)
            {
             /*
                PntrVal  = &grp_num[buffGrp].value[buffSubG-1];    //load data PID setpoint keypad
                PntrFact = &grp_num[buffGrp].factor[buffSubG-1];
                MaxSet   = *(Pntr_max[buffGrp]+(buffSubG-1));
                MinSet   = *(Pntr_min[buffGrp]+(buffSubG-1));

                Tmr_Norm_Set = tm_delay_norm;//clear timer 3sec after setting auto save
                KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 1;

                if(KeyPad_reg.MODE_FLAG.bit.norm_state == 0)
                {
                    KeyPad_reg.MODE_FLAG.bit.norm_state = 1;

                    PreBufCtrSetting = *PntrVal;//load original value to buffer for setting
                    BufferPreDECIB   = (*PntrFact & 0x0300)>>8;//load original value to buffer for setting
                    BufferPreSIGN    = (*PntrFact & 0x3000)>>12;//load original value to buffer for setting
                }

                RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad frequency
                buff = (BufferPreDECIB & 0x0003)<<8;
                buff = ((BufferPreSIGN & 0x0003)<<12)|buff;
                KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);

             */

                //-------------------------------Gus---------------------------------
                if (flagnorm.save_data == 1)
                {
                    PntrVal     = &grp_num[buffGrp].value[buffSubG-1];    //load data PID setpoint keypad
                    PntrMaxset  = &grp_num[buffGrp].value[2];
                    PntrMinset  = &grp_num[buffGrp].value[1];
                    PntrFact    = &grp_num[buffGrp].factor[buffSubG-1];
                    PntrFact_Maxset   = &grp_num[buffGrp].factor[2];
                    MaxSet      = *(Pntr_max[buffGrp]+(buffSubG-1));
                    MinSet      = *(Pntr_min[buffGrp]+(buffSubG-1));
                    MaxSet2     = ConvStg(*PntrMaxset, *PntrFact_Maxset);

                    Tmr_Norm_Set = tm_delay_norm;//clear timer 3sec after setting auto save
                    KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 1;

                    if(KeyPad_reg.MODE_FLAG.bit.norm_state == 0)
                    {
                        KeyPad_reg.MODE_FLAG.bit.norm_state = 1;

                        PreBufCtrSetting = *PntrVal;//load original value to buffer for setting
                        BufferPreDECIB   = (*PntrFact & 0x0300)>>8; //load original value to buffer for setting
                        BufferPreSIGN    = (*PntrFact & 0x3000)>>12;//load original value to buffer for setting

                        BufFMax = *PntrMaxset;//load original value to buffer for setting
                        BufferPreDECIB_FMax   = (*PntrFact_Maxset & 0x0300)>>8;//load original value to buffer for setting
                        BufferPreSIGN_FMax    = (*PntrFact_Maxset & 0x3000)>>12;//load original value to buffer for setting
                    }

                    if ((KeyPad_reg.Sp_Norm >= MaxSet2))
                    {
                        if (BufferPreDECIB <= BufferPreDECIB_FMax)
                        {
                            PreBufCtrSetting = BufFMax;
                            BufferPreDECIB = BufferPreDECIB_FMax;
                            RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact_Maxset);//load with max/min/factor of keypad frequency
                            buff = (BufferPreDECIB_FMax & 0x0003)<<8;
                            buff = ((BufferPreSIGN_FMax & 0x0003)<<12)|buff;
                            KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);
                            KeyPad_reg.DISP_TAB.bit.Disp_Table = hi;
                        }

                    }

                    else
                    {
                        //RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad frequency
                        RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact_Maxset);//load with max/min/factor of keypad frequency
                        buff = (BufferPreDECIB & 0x0003)<<8;
                        buff = ((BufferPreSIGN & 0x0003)<<12)|buff;
                        KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);
                    }
/*
                    //RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad frequency
                    RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact2);//load with max/min/factor of keypad frequency
                    buff = (BufferPreDECIB & 0x0003)<<8;
                    buff = ((BufferPreSIGN & 0x0003)<<12)|buff;
                    KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);
*/
                }
                else
                {
                    Tmr_Norm_Set = tm_delay_norm;//clear timer 3sec after setting auto save
                    KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 1;

                    if(KeyPad_reg.MODE_FLAG.bit.norm_state == 0)
                    {
                        KeyPad_reg.MODE_FLAG.bit.norm_state = 1;
                    }

                    if ((KeyPad_reg.Sp_Norm >= MaxSet2))
                    {
                        if (BufferPreDECIB <= BufferPreDECIB_FMax)
                        {
                            PreBufCtrSetting = BufFMax;
                            BufferPreDECIB = BufferPreDECIB_FMax;
                            RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact_Maxset);//load with max/min/factor of keypad frequency
                            buff = (BufferPreDECIB_FMax & 0x0003)<<8;
                            buff = ((BufferPreSIGN_FMax & 0x0003)<<12)|buff;
                            KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);
                            KeyPad_reg.DISP_TAB.bit.Disp_Table = hi;
                        }

                    }

                    else
                    {
                        //RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad frequency
                        RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact_Maxset);//load with max/min/factor of keypad frequency
                        buff = (BufferPreDECIB & 0x0003)<<8;
                        buff = ((BufferPreSIGN & 0x0003)<<12)|buff;
                        KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);
                    }
/*
                    //RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad frequency
                    RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact2);//load with max/min/factor of keypad frequency
                    buff = (BufferPreDECIB & 0x0003)<<8;
                    buff = ((BufferPreSIGN & 0x0003)<<12)|buff;
                    KeyPad_reg.Sp_Norm = ConvStg(PreBufCtrSetting, buff);
*/
                }
                //-------------------------------Gus---------------------------------
            }

            else
            {
                KeyPad_reg.KEY_FLAG.bit.key_up  = 0;
                KeyPad_reg.KEY_FLAG.bit.key_dwn = 0;
                goto ClrTmrNrm;
            }
        }

        else if(KeyPad_reg.KEY_FLAG.bit.key_data == 1)
        {
            ClearKey();
            Tmr_Norm_Set = 0;
            KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 0;

            if(KeyPad_reg.MODE_FLAG.bit.norm_state == 1)
            {
                goto LdDataNorm;
            }

            else //state=0 data key for change display
            {
                //------------------Gus-------------------
                Tmr_Data_Set = tm_delay_data;
                KeyPad_reg.TRIGGER_BIT.bit.Data_Set_Trg = 1;
                //------------------Gus-------------------
                //if(KeyPad_reg.DISP_FLAG1.bit.BufDispNorm<8)
                if(KeyPad_reg.DISP_FLAG1.bit.BufDispNorm<6)//Hide data 7th(actual speed) to 12th
                {
                    KeyPad_reg.DISP_FLAG1.bit.BufDispNorm++;
                }
                else if(KeyPad_reg.GENERAL_FLAG.bit.MinMainMode==1)
                {
                    KeyPad_reg.DISP_FLAG1.bit.BufDispNorm = 0;
                }

                else if(KeyPad_reg.DISP_FLAG1.bit.BufDispNorm<12)
                {
                    KeyPad_reg.DISP_FLAG1.bit.BufDispNorm++;
                }

                else
                {
                    KeyPad_reg.DISP_FLAG1.bit.BufDispNorm = 0;
                }
            }
        }
        //----------------------------------Gus--------------------------------
        else if (ND2System_Reg.STATE_FLAG.bit.State == state_stop && flagnorm.save_data == 0)
        {
            PntrVal  = &grp_num[buffGrp].value[buffSubG-1];    //load data PID setpoint keypad
            PntrMaxset = &grp_num[buffGrp].value[2];
            PntrMinset = &grp_num[buffGrp].value[1];
            PntrFact = &grp_num[buffGrp].factor[buffSubG-1];
            PntrFact_Maxset= &grp_num[buffGrp].factor[2];
            MaxSet   = *(Pntr_max[buffGrp]+(buffSubG-1));
            MinSet   = *(Pntr_min[buffGrp]+(buffSubG-1));

            //RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad freq.
            RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact_Maxset);//load with max/min/factor of keypad freq.
            Freq_Reg.F_Key     = ConvStg(*PntrVal,*PntrFact);
            KeyPad_reg.Sp_Norm = Freq_Reg.F_Key;

            PreBufCtrSetting = *PntrVal;//load original value to buffer for setting
            BufferPreDECIB   = (*PntrFact & 0x0300)>>8;//load original value to buffer for setting
            BufferPreSIGN    = (*PntrFact & 0x3000)>>12;//load original value to buffer for setting

            BufFMax = *PntrMaxset;//load original value to buffer for setting
            BufferPreDECIB_FMax   = (*PntrFact_Maxset & 0x0300)>>8;//load original value to buffer for setting
            BufferPreSIGN_FMax    = (*PntrFact_Maxset & 0x3000)>>12;//load original value to buffer for setting


            if (KeyPad_reg.TRIGGER_BIT.bit.Data_Set_Trg == 0 || KeyPad_reg.KEY_FLAG.bit.key_stop == 1)
            {
                KeyPad_reg.DISP_FLAG1.bit.BufDispNorm   = grp_num[6].value[0];
                KeyPad_reg.DISP_FLAG1.bit.Bckup_DispSel = KeyPad_reg.DISP_FLAG1.bit.BufDispNorm;
            }

        }
        else if ((grp_num[6].value[3] == 1)&&(grp_num[3].value[0] == 1
                ||(grp_num[3].value[0] == 2 && grp_num[0].value[14] == 0)
                ||grp_num[3].value[0] == 15)) //Switch display with the analog input12 or Keypad's volume
        {
            if((ND2System_Reg.STATE_FLAG.bit.State == state_run)
                    &&(_IQ17abs((_IQ17abs(MainIQ_Variable.FCommand)-_IQ17abs(MainIQ_Variable.FSet)))
                    >PUHyst))
            {
                flagnorm.activeA12 = 1; //Display frequency
                Tmr_Norm_Set = tm_delay_norm;//clear timer
                KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 1;
            }
            else if ((ND2System_Reg.STATE_FLAG.bit.State == state_run)
                    &&((_IQ17abs((_IQ17abs(MainIQ_Variable.FCommand)-_IQ17abs(MainIQ_Variable.FSet)))
                    <= PUHyst)&&(KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg == 0)))
            {
                flagnorm.activeA12 = 0; //Display main data
            }
            else if (ND2System_Reg.STATE_FLAG.bit.State == state_stop
                    ||ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
            {
                flagnorm.activeA12 = 0;

                if (KeyPad_reg.KEY_FLAG.bit.key_stop == 1)
                {
                    KeyPad_reg.DISP_FLAG1.bit.BufDispNorm   = grp_num[6].value[0];
                    KeyPad_reg.DISP_FLAG1.bit.Bckup_DispSel = KeyPad_reg.DISP_FLAG1.bit.BufDispNorm;
                }
            }

        }
        //----------------------------------Gus--------------------------------

        else
        {

            //load data or exit with no operation
            if(KeyPad_reg.MODE_FLAG.bit.norm_state == 1)
            {
                if(KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg == 0)
                {
LdDataNorm: KeyPad_reg.MODE_FLAG.bit.norm_state = 0;

                 /*
                    *PntrVal  = PreBufCtrSetting;
                    *PntrFact = ((BufferPreDECIB & 0x0003)<<8)|(*PntrFact & 0xFCFF);//load current decib
                    *PntrFact = ((BufferPreSIGN & 0x0003)<<12)|(*PntrFact & 0x0FFF);//load current sign
                    ConvAllKeyPad2IQ(buffGrp,(buffSubG-1));

                    if(flagnorm.save_data==1)
                    {
                        WriteUserE2prom(buffGrp,buffSubG,*PntrVal,*PntrFact);
                    }
                 */

                    //-----------------------------Gus--------------------------------
                    if(flagnorm.save_data == 1)
                    {
                        *PntrVal  = PreBufCtrSetting;
                        *PntrFact = ((BufferPreDECIB & 0x0003)<<8)|(*PntrFact & 0xFCFF);//load current decib
                        *PntrFact = ((BufferPreSIGN & 0x0003)<<12)|(*PntrFact & 0x0FFF);//load current sign
                        ConvAllKeyPad2IQ(buffGrp,(buffSubG-1));

                        WriteUserE2prom(buffGrp,buffSubG,*PntrVal,*PntrFact);
                    }
                    else
                    {
                        //RunCntrSetting(MaxSet,MinSet,*PntrFact);//load with max/min/factor of keypad freq.
                        RunCntrSetting(*PntrMaxset,*PntrMinset,*PntrFact_Maxset);//load with max/min/factor of keypad freq.
                        buff = (BufferPreDECIB & 0x0003)<<8;
                        buff = ((BufferPreSIGN & 0x0003)<<12)|buff;
                        Freq_Reg.F_Key = ConvStg(PreBufCtrSetting,buff);
                    }
                    //-----------------------------Gus--------------------------------
                }
            }
            //----------------------------------Gus--------------------------------
            else if (KeyPad_reg.TRIGGER_BIT.bit.Data_Set_Trg == 0
                    || KeyPad_reg.KEY_FLAG.bit.key_stop == 1)
            {
                KeyPad_reg.DISP_FLAG1.bit.BufDispNorm   = grp_num[6].value[0];
                KeyPad_reg.DISP_FLAG1.bit.Bckup_DispSel = KeyPad_reg.DISP_FLAG1.bit.BufDispNorm;
            }
            //----------------------------------Gus--------------------------------
        }
        Arrng_DataDisp();//Arrange data(integer,decmal,sign) to display in normal mode
    }

    else
    {
ClrTmrNrm://clear timer normal mode
        Tmr_Norm_Set = 0;
        KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 0;

    }
}
//////////////////////////////////////////////////////////////////
void Arrng_DataDisp(void)
{
//_iq bufffcmd     = _IQ17mpy(ar_buffdisp[0],MainIQ_Variable.FRef);
_iq bufffcmd     = _IQ17mpy(_IQ17abs(MainIQ_Variable.FCommand),MainIQ_Variable.FRef);
_iq buffFset     = _IQ17mpy(MainIQ_Variable.FSet,MainIQ_Variable.FRef);
_iq buffdataiq   = 0;
//_iq16 buffdataiq16 = 0;
Uint16 dispminus = 0;

    //---Initial data
    Norm_Unit_Disp = Table_UnitL[0];//initial as Hz
    KeyPad_reg.DISP_FLAG.bit.NoBlink = 0;

    if(KeyPad_reg.MODE_FLAG.bit.norm_state == 1)
    {
        Norm_Val_Disp = PreBufCtrSetting;//integer part
        Norm_Dec_Disp = (BufferPreDECIB & 0x0003);//dec part
        dispminus     = (BufferPreSIGN & 0x0003)>>1;
    }

    else if(DgtlPrt_Reg.FLAG_DGTLPRT.bit.UpDwnDIN==1)//under setting by DIN
    {
        Norm_Val_Disp = CurrentCntr->BuffVal;               //integer part
        Norm_Dec_Disp = (CurrentCntr->BuffDEC & 0x0003);    //dec part
        dispminus     = (CurrentCntr->BuffSign & 0x0003)>>1;
    }
    else if(flagnorm.activeA12 == 1 && KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg == 1)
    {
        if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
        {
            buffdataiq = buffFset;
            ConvIQ(buffdataiq,2,0);
        }else
        {
            KeyPad_reg.DISP_FLAG.bit.NoBlink = 1;
            buffdataiq = bufffcmd;
            ConvIQ(buffdataiq,2,0);
        }

    }

    else
    {
        KeyPad_reg.DISP_FLAG.bit.NoBlink = (ND2System_Reg.STATE_FLAG.bit.State == state_stop)?0:1;
        Norm_Unit_Disp = Table_UnitL[KeyPad_reg.DISP_FLAG1.bit.BufDispNorm];//initial as Hz

        switch(KeyPad_reg.DISP_FLAG1.bit.BufDispNorm)
        {
            case 1://I
                buffdataiq = _IQ17mpy(ar_buffdisp[1],MainIQ_Variable.I_Rate);
                //buffdataiq = _IQ17mpy(ar_buffdisp[1],MainIQ_Variable.I_base);
                //-------------------------------------Gus---------------------------------------------------

                if ((ND2System_Reg.STATE_FLAG.bit.State == state_start)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_run)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
                        ||(ND2System_Reg.STATE_FLAG.bit.State == state_stopping))
                {
                    buffdataiq = (ar_buffdisp[1]>_IQ17(0.1))?buffdataiq:0;
                    ConvIQ(buffdataiq,1,0);
                 /*
                    if (grp_num[0].value[11]==0)//Small
                    {
                        buffdataiq = (ar_buffdisp[1]>_IQ17(0.06))?buffdataiq:0;
                        ConvIQ(buffdataiq,1,0);
                    }
                    else if (grp_num[0].value[11]==1)//Big
                    {
                        buffdataiq = (ar_buffdisp[1]>_IQ17(0.06))?buffdataiq:0;
                        ConvIQ(buffdataiq,1,0);
                    }
                 */
                }

                //--------------------------------------Gus---------------------------------------------------

                break;
            case 2://VDC
                ConvIQ(ar_buffdisp[2],1,0);
                break;
            case 3://Sync.Speed
                //            120*f
                //      Ns =  -----
                //              p
                //buffdataiq = _IQ17div(_IQ17(120),CurrentMotor->Poles_Motor);
                //buffdataiq = _IQ17mpy(buffdataiq,bufffcmd);

                //buffdataiq = _IQ17mpy(_IQ17(120),ar_buffdisp[0]); //Overflow
                //buffdataiq = _IQ17div(buffdataiq,CurrentMotor->Poles_Motor);

                if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
                {
                    buffdataiq = _IQ17div(_IQ17(120),CurrentMotor->Poles_Motor);
                    buffdataiq = _IQ17mpy(buffdataiq,buffFset);
                }else
                {
                    buffdataiq = _IQ17div(_IQ17(120),CurrentMotor->Poles_Motor);
                    buffdataiq = _IQ17mpy(buffdataiq,bufffcmd);
                }

                ConvIQ(buffdataiq,0,0);
                break;
            case 4://Line Speed
                //buffdataiq = _IQ17mpy(bufffcmd,KeyPad_reg.LineSpdCoef);

                if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
                {
                    buffdataiq = _IQ17mpy(buffFset,KeyPad_reg.LineSpdCoef);
                }else
                {
                    buffdataiq = _IQ17mpy(bufffcmd,KeyPad_reg.LineSpdCoef);
                }

                ConvIQ(buffdataiq,2,0);
                break;
            case 5://Vout
                buffdataiq = _IQ17mpy(ar_buffdisp[3],MainIQ_Variable.V_base);
                ConvIQ(buffdataiq,1,0);
                //--------Gus-----------
                if (flag_PWMEnable == 2)
                {
                    buffdataiq = 0;
                    ConvIQ(buffdataiq,1,0);
                }
                //--------Gus-----------
                break;
            case 6://Machine Speed
                buffdataiq = _IQ17mpy(bufffcmd,KeyPad_reg.LineSpdCoef);
                ConvIQ(buffdataiq,2,0);
                break;
            case 7://Actual Speed
                ConvIQ(ar_buffdisp[4],2,0);
                break;
            case 8://Torque command
                if(ND2System_Reg.STATE_FLAG.bit.State == state_stop && grp_num[6].value[1]==0)
                {   //stop and display setting when stop
                    buffdataiq = _IQ17abs(TrqCmnd_Reg.TrqSp);
                }else
                {
                    buffdataiq = _IQ17abs(TrqCmnd_Reg.TrqCommand);
                }
                buffdataiq = _IQ17mpy(buffdataiq,Vecter_Reg.Id_Command);
                ConvIQ(buffdataiq,2,0);
                break;
            case 9://Temp.1
                ConvIQ(ar_buffdisp[6],1,1);
                dispminus = (arTemperature[0]<0)?1:0;
                break;
            case 10://Temp.2
                ConvIQ(ar_buffdisp[7],1,1);
                dispminus = (arTemperature[1]<0)?1:0;
                break;
            case 11://Temp.3
                ConvIQ(ar_buffdisp[8],1,1);
                dispminus = (arTemperature[2]<0)?1:0;
                break;
            case 12://Temp.4
                ConvIQ(ar_buffdisp[9],1,1);
                dispminus = (arTemperature[3]<0)?1:0;
                break;
            /*case 13://Power
                ConvIQ(ar_buffdisp[10], 1, 0);
                break;*/
            default://Freq.
                if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
                {
                    buffdataiq = buffFset;
                }else
                {
                    buffdataiq = bufffcmd;
                }
                ConvIQ(buffdataiq,2,0);
            }
    }
    KeyPad_reg.DISP_FLAG.bit.SignNormDisp = dispminus;
} 
//////////////////////////////////////////////////////////////////
void DisplayNormal(void)
{   //prepare buffer for display
    if(KeyPad_reg.DISP_FLAG1.bit.BckBufDispNorm!=KeyPad_reg.DISP_FLAG1.bit.BufDispNorm)
    {
        KeyPad_reg.DISP_FLAG1.bit.BckBufDispNorm = KeyPad_reg.DISP_FLAG1.bit.BufDispNorm;

        ar_buffdisp[0] = _IQ17abs(MainIQ_Variable.FCommand);
        //ar_buffdisp[0] = _IQ17abs(MainIQ_Variable.FSet);
        ar_buffdisp[1] = IV_Read_reg.I_Peak; //default
        //ar_buffdisp[1] = ChkFault_Reg.I_Fault;
        //ar_buffdisp[2] = VDC_Read_reg.VdcBus;
        ar_buffdisp[2] = VDC_Read_reg.VdcDisplay;

        ar_buffdisp[3] = IV_Read_reg.Vout_Rms;
        ar_buffdisp[4] = QEP_Reg.ABSRPM;
        ar_buffdisp[5] = _IQ17abs(TrqCmnd_Reg.TrqCommand);
        ar_buffdisp[6] = _IQ17abs(arTemperature[0]);
        ar_buffdisp[7] = _IQ17abs(arTemperature[1]);
        ar_buffdisp[8] = _IQ17abs(arTemperature[2]);
        ar_buffdisp[9] = _IQ17abs(arTemperature[3]);
        //ar_buffdisp[10] = _IQmpy(IV_Read_reg.Filtered_P, MainIQ_Variable.I_Rate);

        KeyPad_reg.BckDispNorm = 0;
    }

    else
    {
        switch(KeyPad_reg.DISP_FLAG1.bit.BufDispNorm)
        {
            case 1://Ipeak

                if (ar_buffdisp[1] != IV_Read_reg.I_Peak)
                {
                    ar_buffdisp[1] = LowPassFilter(IV_Read_reg.I_Peak,ar_buffdisp[1],KeyPad_reg.ToaDispNorm,IQ17_1);
                }
                /*
                if (ar_buffdisp[1] != ChkFault_Reg.I_Fault)
                {
                    ar_buffdisp[1] = LowPassFilter(ChkFault_Reg.I_Fault,ar_buffdisp[1],KeyPad_reg.ToaDispNorm,IQ17_1);
                }
                */
                break;
            case 2://VDC
/*
                if(ar_buffdisp[2] != VDC_Read_reg.VdcBus)
                {
                    ar_buffdisp[2] = LowPassFilter(VDC_Read_reg.VdcBus,ar_buffdisp[2],KeyPad_reg.ToaDispNorm,IQ17_1);
                }
*/
                if(ar_buffdisp[2] != VDC_Read_reg.VdcDisplay)
                {
                    ar_buffdisp[2] = LowPassFilter(VDC_Read_reg.VdcDisplay,ar_buffdisp[2],KeyPad_reg.ToaDispNorm,IQ17_1);
                }

                break;
            case 5://Vout
                if(ar_buffdisp[3] != IV_Read_reg.Vout_Rms)
                {
                    ar_buffdisp[3] = LowPassFilter(IV_Read_reg.Vout_Rms,ar_buffdisp[3],KeyPad_reg.ToaDispNorm,IQ17_1);
                    if(KeyPad_reg.BckDispNorm == ar_buffdisp[3])
                    {
                        ar_buffdisp[3] = IV_Read_reg.Vout_Rms;
                    }
                    else if (flag_PWMEnable == 2)
                    {
                        ar_buffdisp[3] = 0;
                    }
                }
                KeyPad_reg.BckDispNorm = ar_buffdisp[3];
                break;
            case 7://speed feedback
                if(ar_buffdisp[4] != QEP_Reg.ABSRPM)
                {
                    ar_buffdisp[4] = LowPassFilter(QEP_Reg.ABSRPM,ar_buffdisp[4],KeyPad_reg.ToaDispNorm,IQ17_1);
                }
                break;
            case 8://torque
                if(ar_buffdisp[5]!=_IQ17abs(TrqCmnd_Reg.TrqCommand))
                {
                    ar_buffdisp[5] = LowPassFilter(_IQ17abs(TrqCmnd_Reg.TrqCommand),ar_buffdisp[5],KeyPad_reg.ToaDispNorm,IQ17_1);
                    if(KeyPad_reg.BckDispNorm == ar_buffdisp[5])
                    {
                        ar_buffdisp[5] = _IQ17abs(TrqCmnd_Reg.TrqCommand);
                    }
                }
                KeyPad_reg.BckDispNorm = ar_buffdisp[5];
                break;
            case 9://Temp1
                ar_buffdisp[6] = LowPassFilter(_IQ17abs(arTemperature[0]),ar_buffdisp[6],KeyPad_reg.ToaDispNorm,IQ17_1);
                break;
            case 10://Temp2
                ar_buffdisp[7] = LowPassFilter(_IQ17abs(arTemperature[1]),ar_buffdisp[7],KeyPad_reg.ToaDispNorm,IQ17_1);
                break;
            case 11://Temp3
                ar_buffdisp[8] = LowPassFilter(_IQ17abs(arTemperature[2]),ar_buffdisp[8],KeyPad_reg.ToaDispNorm,IQ17_1);
                break;
            case 12://Temp4
                ar_buffdisp[9] = LowPassFilter(_IQ17abs(arTemperature[3]),ar_buffdisp[9],KeyPad_reg.ToaDispNorm,IQ17_1);
                break;
            /*case 13:
                if(ar_buffdisp[10]!=IV_Read_reg.Filtered_P)
                {
                    ar_buffdisp[10] = LowPassFilter(IV_Read_reg.Filtered_P,ar_buffdisp[10],KeyPad_reg.ToaDispNorm,IQ17_1);
                }
                break;*/
            default:
                if(ar_buffdisp[0]!=_IQ17abs(MainIQ_Variable.FCommand))
                {
                    ar_buffdisp[0] = LowPassFilter(_IQ17abs(MainIQ_Variable.FCommand),ar_buffdisp[0],KeyPad_reg.ToaDispNorm,IQ17_1);

                    if(KeyPad_reg.BckDispNorm==ar_buffdisp[0])
                    {
                        ar_buffdisp[0] = _IQ17abs(MainIQ_Variable.FCommand);
                    }
                }
                KeyPad_reg.BckDispNorm = ar_buffdisp[0];
        }
    }

}
//////////////////////////////////////////////////////////////////
void Check_Range(void)
{
    for(KeyPad_reg.PNTR_MODE.bit.PntrGrp = 1;
            KeyPad_reg.PNTR_MODE.bit.PntrGrp<GrpNumber-1;
            KeyPad_reg.PNTR_MODE.bit.PntrGrp++)
    {
        for(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc = 1;
                KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1<Table_MaxNumGrp[KeyPad_reg.PNTR_MODE.bit.PntrGrp];
                KeyPad_reg.PNTR_MODE.bit.PntrSubFunc++)
        {
recheck_data:
            ServiceDog();
            PntrVal = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].value[KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1];
            PntrFact= &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].factor[KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1];
            MaxSet  = *(Pntr_max[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));
            MinSet  = *(Pntr_min[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));

//          if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==3)
//          {
//              Run_SpectGrpFreq();
//          }
//check blank----------------
            if(*PntrVal==0xFFFF || *PntrFact==0xFFFF)
            {
                ChkFault_Reg.Fault_Buffer               = eep2;
                ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault = 1;
                KeyPad_reg.GENERAL_FLAG.bit.new_fault   = 1;
                ChkFault_Reg.Fault_Buffer               = eep2;
                Record_fault();
                Correct_Data();//correct data e2prom

                goto recheck_data;
            }
//check max------------------
            if(((*PntrFact & 0x0300)>>8)==((*PntrFact & 0x00C0)>>6))//compare current DEC with minDEC
            {
                if(*PntrVal>MaxSet)
                {
                    goto correct_eep1;
                }
            }

            if(*PntrVal/pow(10,((*PntrFact & 0x0300)>>8))>MaxSet/pow(10,((*PntrFact & 0x00C0)>>6)))
            {
                goto correct_eep1;
            }
//check min------------------
            if(((*PntrFact & 0x0300)>>8)==((*PntrFact & 0x0030)>>4))//compare current DEC with maxDEC
            {
                if(*PntrVal<MinSet)
                {
                    goto correct_eep1;
                }
            }

            else if(((*PntrFact & 0x0300)>>8)>(*PntrFact & 0x0030)>>4)
            {
correct_eep1:   ChkFault_Reg.Fault_Buffer               = eep1;
                ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault = 1;
                KeyPad_reg.GENERAL_FLAG.bit.new_fault   = 1;
                ChkFault_Reg.Fault_Buffer               = eep1;
                Record_fault();
                Correct_Data();//correct data e2prom
                goto recheck_data;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
void Correct_Data(void)
{
    //load default before correct the data
    *PntrVal  = *(Pntr_def[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));//load defaul value
    *PntrFact = *(Pntr_factor[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));//load defaul factor

    PreBufCtrSetting = *PntrVal;//load original value to buffer for setting
    BufferPreDECIB   = (*PntrFact & 0x0300)>>8;//load original value to buffer for setting
    BufferPreSIGN    = (*PntrFact & 0x3000)>>12;//load original value to buffer for setting

    KeyPad_reg.MODE_FLAG.bit.mode_state   = state_subfunct;
    KeyPad_reg.MODE_FLAG.bit.setting_mode = 0;
    KeyPad_reg.E2PROM_FLAG.bit.EEP_error  = 1;
    ND2System_Reg.STATE_FLAG.bit.State    = state_stop;//enabling unlock setting, when protected parameter need to setting

    do{
        ServiceDog();
        CheckKeyIn();//which key is pressed and up keystate
        DataDisp();//arrange data to keypad

        if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault==0)
        {
            KeyPad_reg.MODE_FLAG.bit.setting_mode = 1;
            KeyPad_reg.KEY_FLAG.all = KeyPad_reg.KEY_FLAG.all & 0x4FF;//disable the dont needed
            SettingSubFNC();//entry only setting mode and >=sub function
        }
        else
        {
            KeyPad_reg.KEY_FLAG.all = KeyPad_reg.KEY_FLAG.all & 0x1A01;//clear all key exept stop
        }
    }
    while(KeyPad_reg.E2PROM_FLAG.bit.EEP_error==1);
    KeyPad_reg.KEY_FLAG.all = KeyPad_reg.KEY_FLAG.all & 0x0001;
}
//////////////////////////////////////////////////////////////////
void Operation_GrpPrd_Srvc(void)//run group producton and service mode
{
    if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==8)
    {
        if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==1)     //Load default
        {
            if(*PntrVal==1)
            {
                KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd = 0;
                ND2System_Reg.STATE_FLAG.bit.InitPU        = 0;//need to re-cal PU again

                DispTab(53);

                KeyPad_reg.PNTR_MODE.bit.PntrGrp           = 1;     //first group for load default in user
                KeyPad_reg.E2PROM_ADRS.all                 = 0x0300;    //start e2prom adrs at value group1

                LoadDefaultE2(1);

                KeyPad_reg.PNTR_MODE.bit.PntrGrp           = 8;
                KeyPad_reg.GENERAL_FLAG.bit.MinMainMode    = grp_num[8].value[5];

                grp_num[0].value[1] = grp_num[8].value[2];
                PntrVal             = &grp_num[0].value[1];

                WriteUserE2prom(0,2,*PntrVal,*PntrFact);

                PntrVal = &grp_num[8].value[0];
                //grp_num[8].value[4]  = SWV;
                //grp_num[8].factor[4] = 0x0001;

                ConvAllKeyPad2IQ(0,0);

                KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd = 1;
            }
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==2)   //Lock setting
        {
            KeyPad_reg.GENERAL_FLAG.bit.LockSetting = *PntrVal;
            grp_num[0].value[0] = *PntrVal;
            WriteUserE2prom(0,1,*PntrVal,*PntrFact);
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==3)   //User passward
        {
            grp_num[0].value[1]  = *PntrVal;
            grp_num[0].factor[1] = *PntrFact;
            WriteUserE2prom(0,2,*PntrVal,*PntrFact);
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 4)   //Fault memory
        {
            KeyPad_reg.DISP_TAB.bit.Disp_fault = ~KeyPad_reg.DISP_TAB.bit.Disp_fault;
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==6)   //Factory passward grp0
        {
            KeyPad_reg.GENERAL_FLAG.bit.MinMainMode = *PntrVal;
        }

        if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc!=4 && KeyPad_reg.DISP_FLAG.bit.NoBlink==0)
        {
            ClrStSetting();
        }
    }else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 0)
    {
        if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 1)//Data protection enable in production group
        {
            KeyPad_reg.GENERAL_FLAG.bit.LockSetting = *PntrVal;
            grp_num[8].value[1] = *PntrVal;

            WriteUserE2prom(8,2,*PntrVal,*PntrFact);
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==2)//user passward in production group
        {
            grp_num[8].value[2]  = *PntrVal;
            grp_num[8].factor[2] = *PntrFact;

            WriteUserE2prom(8,3,*PntrVal,*PntrFact);
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==5)//inverter volatge range
        {
            Update_Range(*PntrVal);
        }
        else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==20 && Bckup_reg.BCKUP_FLAG.bit.UpldFail==0)//backup data e2prom
        {
            if(*PntrVal == 1)//uploading enabled
            {
                *PntrVal = 0;
                Uploading();
            }
        }else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 21 && Bckup_reg.BCKUP_FLAG.bit.DnldFail == 0)
        {
            if(*PntrVal == 1)//downloading enabled
            {
                *PntrVal = 0;
                Downloading();
            }
        }

        ClrStSetting();
    }
}  
//////////////////////////////////////////////////////////////////
void DispTab(Uint16 datatab)
{
    KeyPad_reg.BUFFERDATAH.all          = 0;
    KeyPad_reg.BUFFERDATAL.bit.fmode    = tab_disp;
    KeyPad_reg.BUFFERDATAL.bit.data1    = datatab;

    MSG_MBX1DataL = KeyPad_reg.BUFFERDATAL.all;
    MSG_MBX1DataH = KeyPad_reg.BUFFERDATAH.all;
}
//////////////////////////////////////////////////////////////////
void Update_Range(Uint16 range_value)
{
Uint16 i=0;

    if(range_value==0)
    {
        for(i=0; i < GrpNumber; i++)
        {
            Pntr_min[i]=Pntr_minA[i];
        }
        for(i=0; i < GrpNumber; i++)
        {
            Pntr_max[i]=Pntr_maxA[i];
        }
        for(i=0; i < GrpNumber; i++)
        {
            Pntr_def[i]=Pntr_defA[i];
        }
    }else
    {
        for(i=0; i < GrpNumber; i++)
        {
            Pntr_min[i]=Pntr_minB[i];
        }
        for(i=0; i < GrpNumber; i++)
        {
            Pntr_max[i]=Pntr_maxB[i];
        }
        for(i=0; i < GrpNumber; i++)
        {
            Pntr_def[i]=Pntr_defB[i];
        }
    }
}

void LoadDefaultE2(Uint16 StartGrp)
{
        KeyPad_reg.E2PROM_FLAG.bit.Loading_def = 1;
        DispTab(53);
        LoadDefault(StartGrp);//load default to RAM

        WriteE2prom(0x00, 0x00, 0xCCCC);

        do//write data function group
        {
            if(KeyPad_reg.E2PROM_FLAG1.bit.W_Status==I2C_MSGSTAT_SEND_WITHSTOP)
            {
                WriteE2prom(KeyPad_reg.E2PROM_ADRS.bit.Adrs_H, KeyPad_reg.E2PROM_ADRS.bit.Adrs_L-2, *PntrDataE2Prom);

            }

            else
            {
                if(KeyPad_reg.E2PROM_FLAG1.bit.W_Status==I2C_MSGSTAT_RESTART)
                {
                    Run_AdrsGRP();
                    KeyPad_reg.E2PROM_FLAG1.bit.W_Status=I2C_MSGSTAT_SEND_WITHSTOP;
                }
            }
        }

        while(KeyPad_reg.PNTR_MODE.bit.PntrGrp<GrpNumber && KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0);
        KeyPad_reg.E2PROM_FLAG.bit.Loading_def = 0;
} 
//////////////////////////////////////////////////////////////////
void WriteUserE2prom(Uint16 AdrsHE2, Uint16 AdrsLE2, Uint16 ValE2, Uint16 FactE2)

//AdrsHE2=Grp, AdrsLE2=SubGrp, ValE2=Val, FactE2=Factor
{
    KeyPad_reg.E2PROM_ADRS.bit.Adrs_H = (AdrsHE2 * 2)+1;//write value
    KeyPad_reg.E2PROM_ADRS.bit.Adrs_L = (AdrsLE2-1) * 2;

    WriteE2prom(KeyPad_reg.E2PROM_ADRS.bit.Adrs_H, KeyPad_reg.E2PROM_ADRS.bit.Adrs_L, ValE2);

    while(KeyPad_reg.E2PROM_FLAG1.bit.W_Status!=I2C_MSGSTAT_RESTART
            &&KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)//waiting for re-writting 7ms
    {
        ServiceDog();
    }
    KeyPad_reg.E2PROM_ADRS.bit.Adrs_H = KeyPad_reg.E2PROM_ADRS.bit.Adrs_H+1;//write factor

    WriteE2prom(KeyPad_reg.E2PROM_ADRS.bit.Adrs_H, KeyPad_reg.E2PROM_ADRS.bit.Adrs_L, FactE2);
}
//////////////////////////////////////////////////////////////////
void Run_AdrsGRP(void)
{
    ServiceDog();
    if((KeyPad_reg.E2PROM_ADRS.bit.Adrs_L/2)>Table_MaxNumGrp[KeyPad_reg.PNTR_MODE.bit.PntrGrp]-1)
    {
        if((KeyPad_reg.E2PROM_ADRS.bit.Adrs_H%2)==0)
        {
            KeyPad_reg.PNTR_MODE.bit.PntrGrp++;
        }
        KeyPad_reg.E2PROM_ADRS.bit.Adrs_H++;
        KeyPad_reg.E2PROM_ADRS.bit.Adrs_L = 0;
    }
    if((KeyPad_reg.E2PROM_ADRS.bit.Adrs_H%2)!=0)    //AdrsE2Prom of value is odd value
    {
        PntrDataE2Prom = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].value[KeyPad_reg.E2PROM_ADRS.bit.Adrs_L/2];
    }else   //AdrsE2Prom of factor is even value
    {
        PntrDataE2Prom = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].factor[KeyPad_reg.E2PROM_ADRS.bit.Adrs_L/2];
    }

    KeyPad_reg.E2PROM_ADRS.bit.Adrs_L+=2;
}

//////////////////////////////////////////////////////////////////
void SettingSubFNC(void)
{
Uint16 buffmax=0;

    if(KeyPad_reg.MODE_FLAG.bit.mode_state>=state_subfunct)//state_subfunct=2
    {
        PntrVal  = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].value[KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1];
        PntrFact = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].factor[KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1];
        buffmax  = *(Pntr_max[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));

        if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==6 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==1)
        {
            buffmax = buffmax-KeyPad_reg.GENERAL_FLAG.bit.MinMainMode;
        }
        MaxSet  = buffmax;
        MinSet  =*(Pntr_min[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));

        RunSubGrp(Table_MaxNumGrp[KeyPad_reg.PNTR_MODE.bit.PntrGrp], 1);
    }
}
////////////////////////////////////////////////////////////////// 
void Run_SpectGrpSrvc(void)
{
//--Load password tester---------------------------------------------------------------
    if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 2 || KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 3)
    {
        ar_PSW_Tester[0] = grp_num[8].value[2];//user passward
        PSW_Tester       = ar_PSW_Tester[0];
    }
    else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==6)
    {
        PSW_Tester = ar_PSW_Tester[1];          //factory passward 9944
    }
    //----------------------------------------------------------------------------------
    if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==2||KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==3)//Passward
    {
        goto chck_PSW;
    }else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==6)//Factory passward(Grp0)
    {
        if(*PntrVal==1)
        {
chck_PSW:   if(KeyPad_reg.GENERAL_FLAG.bit.StateDig<4)
            {
                if(KeyPad_reg.MODE_FLAG.bit.mode_st3_2==0)
                {
                    KeyPad_reg.MODE_FLAG.bit.mode_st3_2  = 1;
                    KeyPad_reg.GENERAL_FLAG.bit.StateDig = 0;
                }
                KeyPad_reg.MODE_FLAG.bit.mode_state = state_subfunct;
                UpDigit();
            }else
            {
                if(PreBufCtrSetting==PSW_Tester)
                {
                    ClrStSetting1();
                }else
                {
                    ClrStSetting();
                }
            }
        }
    }else if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==5)//Firmware version read only
    {
        //KeyPad_reg.DISP_FLAG.bit.NoBlink=1;
        //ClrStSetting();
        //--------------------------------Gus----------------------------------------------
        KeyPad_reg.MODE_FLAG.bit.mode_state    = state_setting;
        KeyPad_reg.GENERAL_FLAG.bit.StateDig   = 0;
        //--------------------------------Gus----------------------------------------------
    }
}
////////////////////////////////////////////////////////////////// 
Uint16 HideSubFnct(Uint16 SubGrp)
{
    if (grp_num[0].value[33] == 1)//Hide vector functions
    {
        if (KeyPad_reg.PNTR_MODE.bit.PntrGrp == 1)
        {
            //Vector control functions
            if(SubGrp>=9 && SubGrp<=16)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?17:8;
            }
            else if(SubGrp>=25 && SubGrp<=32)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?1:24;
            }
        }

        //terminal functions (<7.5hp)
        //else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 2)
        if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 2)
        {
            if(SubGrp==7 || SubGrp==8)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?9:6;
            }else if(SubGrp==14 || SubGrp==15)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?16:13;
            }else if(SubGrp==18 || SubGrp==19)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?20:17;
            }else if(SubGrp==21 || SubGrp==22)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?23:20;
            }else if(SubGrp>=29 && SubGrp<=34)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?35:28;
            }else if(SubGrp==42)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?43:41;
            }else if(SubGrp>=49 && SubGrp<=54)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?55:48;
            }
        }

        //Vector control functions
        if (KeyPad_reg.PNTR_MODE.bit.PntrGrp == 5)
        {
            if(SubGrp>=10 && SubGrp<=20)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?21:9;
            }
            else if(SubGrp>=32 && SubGrp<=42)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?1:31;
            }
        }
    }
    else
    {
        //terminal functions (<7.5hp)
        //else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 2)
        if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 2)
        {
            if(SubGrp==7 || SubGrp==8)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?9:6;
            }else if(SubGrp==14 || SubGrp==15)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?16:13;
            }else if(SubGrp==18 || SubGrp==19)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?20:17;
            }else if(SubGrp==21 || SubGrp==22)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?23:20;
            }else if(SubGrp>=29 && SubGrp<=34)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?35:28;
            }else if(SubGrp==42)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?43:41;
            }else if(SubGrp>=49 && SubGrp<=54)
            {
                SubGrp = (KeyPad_reg.KEY_FLAG.bit.key_up==1)?55:48;
            }
        }
    }

    return SubGrp;
}
////////////////////////////////////////////////////////////////// 
void RunSubGrp(Uint16 max_grp, Uint16 min_grp)
{
Uint16 buff=0;
struct
{
    Uint16 no_save:1;//no save data to e2prom
}
flagRunSubGrp = {0};

    if(KeyPad_reg.MODE_FLAG.bit.mode_state<state_setting)
    {
        if(KeyPad_reg.KEY_FLAG.bit.key_data==1)
        {
            ClearKey();
            KeyPad_reg.MODE_FLAG.bit.mode_state = state_setting;
            //---------------Gus------------------
            Tmr_Mode_Set = tm_delay_data;
            KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
            //---------------Gus------------------

            if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==8)
            {
                Run_SpectGrpSrvc();
            }//end of check group 7
        }

        else if((KeyPad_reg.KEY_FLAG.bit.key_dwn == 1)||(KeyPad_reg.KEY_FLAG.bit.key_up == 1))
        {
            //---------------Gus------------------
            Tmr_Mode_Set = tm_delay_data;
            KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
            //---------------Gus------------------
            if(KeyPad_reg.E2PROM_FLAG.bit.EEP_error == 1)
            {
                KeyPad_reg.KEY_FLAG.bit.key_dwn = 0;
                KeyPad_reg.KEY_FLAG.bit.key_up  = 0;
            }

            else
            {
                if(KeyPad_reg.DISP_FLAG.bit.DispPSW == 1)
                {
                    RunPassword();
                }

                else
                {
                    buff = run_updwncntr(max_grp, min_grp, KeyPad_reg.PNTR_MODE.bit.PntrSubFunc, 1);
                    if(grp_num[0].value[14]==0)
                    {
                        //    //Vector control functions
                        if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 1)
                        {
                            buff = HideSubFnct(buff);
                        }
                        else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==2)//check hide some sub function in small model
                        {
                            buff = HideSubFnct(buff);
                        }
                        //Vector control functions
                        else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 5)
                        {
                            buff = HideSubFnct(buff);
                        }
                    }
                    else if(grp_num[0].value[14]==1)
                    {
                        //    //Vector control functions
                        if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 1)
                        {
                            buff = HideSubFnct(buff);
                        }
                        //Vector control functions
                        else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp == 5)
                        {
                            buff = HideSubFnct(buff);
                        }
                    }
                    KeyPad_reg.PNTR_MODE.bit.PntrSubFunc = buff;

                }
            }
        }
    }
    else //check condition in setting state(3)
    {
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
        {
            if(ND2System_Reg.STATE_FLAG.bit.State != state_stop)
            {
                KeyPad_reg.GENERAL_FLAG.bit.LockSetting = ((*PntrFact & 0x0400)>>10)|grp_num[8].value[1];
            }

            else
            {
                KeyPad_reg.GENERAL_FLAG.bit.LockSetting = grp_num[8].value[1];//lock key follow F802
            }
        }

        KeyPad_reg.DISP_FLAG.bit.NoBlink = 0;

        if(KeyPad_reg.GENERAL_FLAG.bit.LockSetting==1)
        {
            KeyPad_reg.DISP_FLAG.bit.NoBlink = 1;
            //Data protection must be modified
            if((KeyPad_reg.PNTR_MODE.bit.PntrGrp==8 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==2)
                    ||(KeyPad_reg.PNTR_MODE.bit.PntrGrp==8 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==6)
                    ||(KeyPad_reg.PNTR_MODE.bit.PntrGrp==0 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==1))
            {
                KeyPad_reg.DISP_FLAG.bit.NoBlink = 0;
            }
        }

        if(KeyPad_reg.KEY_FLAG.bit.key_prog==1)//key program
        {
            KeyPad_reg.KEY_FLAG.bit.key_prog = 0;
            ClrStSetting();
        }

        else if(KeyPad_reg.KEY_FLAG.bit.key_data==1)//key data-->save data
        {
            KeyPad_reg.KEY_FLAG.bit.key_data = 0;

            if((*PntrFact & 0x0800)==0x0800)//check setting by digit(0x8) or setting by normal run counting(0x0)
            {
                if(KeyPad_reg.MODE_FLAG.bit.mode_st3_2==0)
                {
                    KeyPad_reg.MODE_FLAG.bit.mode_st3_2 = 1;
                    PreBufCtrSetting = *PntrVal;//load original value to buffer for setting
                    BufferPreDECIB   = (*PntrFact & 0x0300)>>8;//load original value to buffer for setting
                    BufferPreSIGN    = (*PntrFact & 0x3000)>>12;//load original value to buffer for setting
                    BuffDigit        = PreBufCtrSetting%10;
                }

                if(KeyPad_reg.GENERAL_FLAG.bit.StateDig<4)
                {
                    UpDigit();
                }

                else
                {
                    goto loaddata;
                }
            }

            else
loaddata: {
                KeyPad_reg.DISP_FLAG.bit.DispPSW = 0;
                if(KeyPad_reg.MODE_FLAG.bit.mode_st3_2==1||(KeyPad_reg.E2PROM_FLAG.bit.EEP_error==1))//check change data(mode_st3_2=1)or eep error from check range
                {
                    *PntrVal  = PreBufCtrSetting;
                    *PntrFact = ((BufferPreDECIB & 0x0003)<<8)|(*PntrFact & 0xFCFF);//load current decib
                    *PntrFact = ((BufferPreSIGN & 0x0003)<<12)|(*PntrFact & 0x0FFF);//load current sign

                    if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==8)
                    {
                        if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 4)
                        {
                            flagRunSubGrp.no_save = 1;
                        }
                    }

                    else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==0)
                    {
                        if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 20||
                           KeyPad_reg.PNTR_MODE.bit.PntrSubFunc == 21)//upload/download function
                        {
                            flagRunSubGrp.no_save = 1;
                        }
                    }

                    else if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==5)
                    {
                        if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==32)
                        {
                            flagRunSubGrp.no_save = 1;
                        }
                    }

                    if(flagRunSubGrp.no_save == 0)
                    {
                        WriteUserE2prom(KeyPad_reg.PNTR_MODE.bit.PntrGrp,
                                        KeyPad_reg.PNTR_MODE.bit.PntrSubFunc,*PntrVal,*PntrFact);
                        ConvAllKeyPad2IQ(KeyPad_reg.PNTR_MODE.bit.PntrGrp,
                                         KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1);
                    }
                }

                KeyPad_reg.E2PROM_FLAG.bit.EEP_error=0; //clear flag after correct data when eep out of range

                if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==0||KeyPad_reg.PNTR_MODE.bit.PntrGrp==8)
                {
                    Operation_GrpPrd_Srvc();
                }

                else
                {
                    ClrStSetting();
                }
            }
        }

        else if((KeyPad_reg.KEY_FLAG.bit.key_up==1)||(KeyPad_reg.KEY_FLAG.bit.key_dwn==1))//key up/down
        {

            if(KeyPad_reg.DISP_TAB.bit.Disp_fault==1
                    || KeyPad_reg.DISP_FLAG.bit.NoBlink==1
                    ||KeyPad_reg.GENERAL_FLAG.bit.LockSetting==1)
            {
                if((KeyPad_reg.PNTR_MODE.bit.PntrGrp==8 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==2)
                        ||(KeyPad_reg.PNTR_MODE.bit.PntrGrp==8 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==6)
                        ||(KeyPad_reg.PNTR_MODE.bit.PntrGrp==0 && KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==1))
                {
                    goto setting;
                }

                KeyPad_reg.KEY_FLAG.bit.key_up  = 0;
                KeyPad_reg.KEY_FLAG.bit.key_dwn = 0;

                if(KeyPad_reg.GENERAL_FLAG.bit.LockSetting==1)
                {
                    KeyPad_reg.DISP_TAB.bit.Disp_Table = dash;
                }
            }

            else
            {
setting:     if(KeyPad_reg.MODE_FLAG.bit.mode_st3_2==0)
                 {
                    KeyPad_reg.MODE_FLAG.bit.mode_st3_2 = 1;

                    PreBufCtrSetting = *PntrVal;//load original value to buffer for setting
                    BufferPreDECIB   = (*PntrFact & 0x0300)>>8;//load original value to buffer for setting
                    BufferPreSIGN    = (*PntrFact & 0x3000)>>12;//load original value to buffer for setting
                    BuffDigit        = PreBufCtrSetting%10;
                 }

                if((*PntrFact & 0x0800)!=0)//check setting in password type
                {
                    RunPassword();
                }

                else
                {
                    KeyPad_reg.TRIGGER_BIT.bit.open_loop_cntr=1;
                    RunCntrSetting(MaxSet,MinSet,*PntrFact);
                }
                KeyPad_reg.DISP_FLAG.bit.DispPreBuf = 1;
            }
        }

        else if(KeyPad_reg.KEY_FLAG.bit.key_shift==1)
        {
            KeyPad_reg.KEY_FLAG.bit.key_shift = 0;

            if((*PntrFact & 0x0800)==0)//check setting in password type
            {
                if(KeyPad_reg.GENERAL_FLAG.bit.StateDig < 4-((*PntrFact & 0x000C)>>2))
                {
                    KeyPad_reg.GENERAL_FLAG.bit.StateDig++;
                }

                else
                {
                    KeyPad_reg.GENERAL_FLAG.bit.StateDig = 1;
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
void UpDigit(void)
{
    KeyPad_reg.GENERAL_FLAG.bit.StateDig++;

    DevideDigit = pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig);
    BuffDigit   = PreBufCtrSetting%DevideDigit;
    BuffDigit   = BuffDigit/pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1);

    KeyPad_reg.DISP_FLAG.bit.DispPreBuf = 1;
    KeyPad_reg.DISP_FLAG.bit.DispPSW    = 1;
}
//////////////////////////////////////////////////////////////////
void RunPassword(void)
{
Uint16  xxx=0;

KeyPad_reg.TRIGGER_BIT.bit.open_loop_cntr = 0;
BuffDigit = run_updwncntr(9,0,BuffDigit, 1);

DevideDigit = pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig);
xxx = PreBufCtrSetting%DevideDigit;
xxx = xxx/pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1);
xxx = xxx * pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1);
PreBufCtrSetting = (PreBufCtrSetting-xxx)+
        (BuffDigit * pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1));
}
//////////////////////////////////////////////////////////////////
void RunCntrSetting(Uint16 max_val, Uint16 min_val, Uint16 factor_val)
{
    Uint16  x=0,y=0,Step_Cnt=0;

    x = 1000/pow(10,((factor_val & 0x000C)>>2));//mark1100 at first byte for checking how many digit
    y = 9999/pow(10,((factor_val & 0x000C)>>2));//mark1100 at first byte for checking how many digit

    if(KeyPad_reg.GENERAL_FLAG.bit.StateDig==1)
    {
        Step_Cnt = factor_val & 0x0003;
    }
    else
    {
        Step_Cnt = ((factor_val & 0x0003) * pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1))/(factor_val & 0x0003); //shift button
    }

    KeyPad_reg.DISP_TAB.all=0;
//-----------enter for key down-----------
    if(KeyPad_reg.KEY_FLAG.bit.key_dwn==1)
    {
        //---------------Gus------------------
        Tmr_Mode_Set = tm_delay_data;
        KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
        //---------------Gus------------------

        if(BufferPreSIGN==1)
        {
            if(PreBufCtrSetting<pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1))
            {
                if(KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg==0)
                {
                    Tmr_CNTR_UpDown1 = Tmr_CNTR_UpDown;
                    KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg = 1;
                    PreBufCtrSetting = pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1)-PreBufCtrSetting;
                    if(PreBufCtrSetting != 0)
                    {
                        BufferPreSIGN = 3;
                    }else
                    {
                        BufferPreDECIB = (factor_val & 0x0030)>>4;
                    }
                }
            }
        }

        if(BufferPreSIGN == 3)//check sign (-)
        {
            goto    Up_value;
        }else//no sign data
        {
Dn_value:   if(Step_Cnt<=PreBufCtrSetting)
            {
                if(BufferPreDECIB<((factor_val & 0x0030)>>4))//check max decib
                {
                    PreBufCtrSetting = run_updwncntr(max_val, x-1 ,PreBufCtrSetting, Step_Cnt);
                    ChkDEC(x,y,UpDEC);
                    if(BufferPreDECIB>=(factor_val & 0x0030)>>4)
                    {
                        if(PreBufCtrSetting<min_val)
                        {
                            Min_Value(min_val, factor_val, load_min);
                        }
                    }
                }else if(PreBufCtrSetting>min_val)
                {
                    if((PreBufCtrSetting-Step_Cnt)<min_val)
                    {
                        Min_Value(min_val, factor_val, load_min);
                    }else
                    {
                        PreBufCtrSetting = run_updwncntr(y, min_val,PreBufCtrSetting, Step_Cnt);

                        if(PreBufCtrSetting==0 && BufferPreSIGN==3)
                        {
                            BufferPreSIGN = 1;
                        }
                    }
                }else
                {
                    Min_Value(min_val, factor_val, load_min);
                }
            }else
            {
                Min_Value(min_val, factor_val, no_load_min);
            }
        }
    }else
//-----------enter for key up-----------
    {
        //---------------Gus------------------
        Tmr_Mode_Set = tm_delay_data;
        KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
        //---------------Gus------------------

        if(BufferPreSIGN==3)
        {
            if(PreBufCtrSetting < pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1))
            {
                if(KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg==0)
                {
                    Tmr_CNTR_UpDown1 = Tmr_CNTR_UpDown;
                    KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg = 1;
                    PreBufCtrSetting = pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1) - PreBufCtrSetting;
                    BufferPreSIGN    = 1;

                    if(PreBufCtrSetting == 0)
                    {
                        BufferPreDECIB = (factor_val & 0x0030)>>4;
                    }
                }
            }
        }
        if(BufferPreSIGN == 3)//check sign-
        {
            goto Dn_value;
        }else//no sign data
        {
Up_value:   if(BufferPreDECIB>((factor_val & 0x00C0)>>6))//check min decib
            {
                PreBufCtrSetting = run_updwncntr(y+1, min_val,PreBufCtrSetting, Step_Cnt);
                ChkDEC(x,y,DownDEC);
                if(BufferPreDECIB<=(factor_val & 0x00C0)>>6)
                {
                    if(PreBufCtrSetting>max_val)
                    {
                        Max_Value(max_val, factor_val, load_max);
                    }
                }
            }else if(Step_Cnt>max_val)
            {
                Max_Value(max_val,factor_val,no_load_max);
            }else
            {
                if(PreBufCtrSetting>=max_val)
                {
                    Max_Value(max_val,factor_val,load_max);
                }else
                {
                    PreBufCtrSetting = run_updwncntr(max_val, min_val, PreBufCtrSetting, Step_Cnt);
                    if(PreBufCtrSetting>max_val)
                    {
                        Max_Value(max_val,factor_val,load_max);
                    }
                }
            }
        }
    }
} 
//////////////////////////////////////////////////////////////////
void Max_Value(Uint16 max_val1, Uint16 factor_val1,Uint16 load_max1)
{
    if(load_max1==load_max)
    {
        PreBufCtrSetting = max_val1;
        BufferPreDECIB   = (factor_val1 & 0x00C0)>>6;
    }
    if((KeyPad_reg.KEY_FLAG.bit.state_speed_updwn>1)||(Tmr_key_next_step<=(t_key_next_step-200)))
    {
        KeyPad_reg.KEY_FLAG.bit.key_up  = 0;
        KeyPad_reg.KEY_FLAG.bit.key_dwn = 0;
        if(BufferPreSIGN==3)
        {
            KeyPad_reg.DISP_TAB.bit.Disp_Table = lo;
        }else
        {
            KeyPad_reg.DISP_TAB.bit.Disp_Table = hi;
            PreBufCtrSetting = max_val1;
            BufferPreDECIB   = (factor_val1 & 0x00C0)>>6;
        }
    }
}
//////////////////////////////////////////////////////////////////
void Min_Value(Uint16 min_val1, Uint16 factor_val1, Uint16 load_min1)
{
    if(BufferPreSIGN==0)
    {
        if((KeyPad_reg.KEY_FLAG.bit.state_speed_updwn>1)||(Tmr_key_next_step<=(t_key_next_step-250)))
        {
            KeyPad_reg.DISP_TAB.bit.Disp_Table     = lo;//********************
            KeyPad_reg.KEY_FLAG.bit.key_dwn        = 0;
            KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg = 0;
            Tmr_CNTR_UpDown1 = 0;
            PreBufCtrSetting = min_val1;
            BufferPreDECIB   = (factor_val1 & 0x0030)>>4;
        }
        if(load_min1 == load_min)//if((load_min1 == load_min)&&(KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg==0))
        {
            PreBufCtrSetting = min_val1;
            BufferPreDECIB   = (factor_val1 & 0x0030)>>4;
        }
    }else
    {
        if(KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg==0)
        {
            Tmr_CNTR_UpDown1 = Tmr_CNTR_UpDown;
            KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg = 1;
            PreBufCtrSetting = pow(10,KeyPad_reg.GENERAL_FLAG.bit.StateDig-1)-PreBufCtrSetting;
            if(PreBufCtrSetting != 0)
            {
                BufferPreSIGN = 3;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
void ChkDEC(Uint16 x1,Uint16 y1, Uint16 DECDir)
{
    if(DECDir==UpDEC)
    {
        if(PreBufCtrSetting < x1)//mark for how many digit is used
        {
            BufferPreDECIB++;
            PreBufCtrSetting = PreBufCtrSetting * 10;
            if(KeyPad_reg.GENERAL_FLAG.bit.StateDig == 1)
            {
                PreBufCtrSetting = PreBufCtrSetting+9;
            }
        }
    }
    else
    {
        if(PreBufCtrSetting > y1)
        {
            PreBufCtrSetting = PreBufCtrSetting/10;
            BufferPreDECIB--;//decrease decib
        }
    }
}
//////////////////////////////////////////////////////////////////
void ClrStSetting(void)
{
    ClearKey();
    ClrStSetting1();
    KeyPad_reg.GENERAL_FLAG.bit.LockSetting = 0;
    KeyPad_reg.DISP_TAB.bit.Disp_fault      = 0;

    if(KeyPad_reg.PNTR_MODE.bit.PntrQck_mode==0)
    {
        KeyPad_reg.MODE_FLAG.bit.mode_state = state_subfunct;
    }else
    {
        KeyPad_reg.MODE_FLAG.bit.mode_state = state_norm_qck;
        KeyPad_reg.PNTR_MODE.bit.PntrGrp    = 1;
    }
} 
//////////////////////////////////////////////////////////////////
void ClrStSetting1(void)
{
    KeyPad_reg.MODE_FLAG.bit.norm_state = 0;
    KeyPad_reg.DISP_FLAG.bit.DispPSW    = 0;
    KeyPad_reg.DISP_FLAG.bit.DispPreBuf = 0;
    KeyPad_reg.DISP_FLAG.bit.NoBlink    = 0;
    KeyPad_reg.GENERAL_FLAG.bit.StateDig= 1;
    KeyPad_reg.MODE_FLAG.bit.mode_st3_2 = 0;

    PreBufCtrSetting= 0;
    BufferPreDECIB  = 0;
    BufferPreSIGN   = 0;

    KeyPad_reg.BUFFERDATAL.all = 0;
    KeyPad_reg.BUFFERDATAH.all = 0;
} 
////////////////////////////////////////////////////////////////// 
void init_keypad(void)
{
    Update_Range(grp_num[0].value[4]);//point the table with correct voltage range F005
    Check_Range();//check over/under range for grp1-8

    KeyPad_reg.GENERAL_FLAG.bit.MinMainMode = grp_num[8].value[5]; //F806
    KeyPad_reg.PNTR_MODE.bit.PntrSubFunc    = 1;
    KeyPad_reg.PNTR_MODE.bit.PntrGrp        = KeyPad_reg.GENERAL_FLAG.bit.MinMainMode;
    KeyPad_reg.MODE_FLAG.bit.setting_mode   = 0;
    KeyPad_reg.MODE_FLAG.bit.mode_state     = state_norm_qck;
    KeyPad_reg.GENERAL_FLAG.bit.LockSetting = grp_num[8].value[1]; //F802
    KeyPad_reg.DISP_FLAG1.bit.BufDispNorm   = grp_num[6].value[0];//initial data to display in normal mode F601
    //grp_num[8].value[4]  = SWV; //F805
    //grp_num[8].factor[4] = 0x0001;

    ConvAllKeyPad2IQ(0,0);//converse all data
    KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd = 1;
}
////////////////////////////////////////////////////////////////// 
void ConvAllKeyPad2IQ(Uint16 grp, Uint16 sub_grp)
{
_iq temp_data=0;
Uint16 i=0,y=0;
_iq *pntr=&Freq_Reg.InitAcc;

//Group0
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==0)
    {
        for(i=2;i<16;i++)
        {
            pntr = &MainIQ_Variable.Time_RelayRC;
            if(i==2)
            {
                y=2;
            }else if(i==4)
            {
                i=6;
                y=4;
            }else if(i==8)
            {
                i=15;
                y=11;
            }
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[0].value[i],grp_num[0].factor[i]);
                *(pntr+i-y) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp0;
                }
            }
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==12)
        {
            KeyPad_reg.ToaDispNorm = ConvStg(grp_num[0].value[12],grp_num[0].factor[12]);
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==18)
        {
            VDC_Read_reg.VDCGain = ConvStg(grp_num[0].value[18],grp_num[0].factor[18]);
        }
        for(i=22;i<25;i++)
        {
            pntr = &Lift_Reg.InitLiftDec;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[0].value[i],grp_num[0].factor[i]);
                *(pntr+i-22) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp0;
                }
            }
        }
        for(i=29;i<33;i++)
        {
            pntr = &ChkFault_Reg.TempCutInOH1;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[0].value[i],grp_num[0].factor[i]);
                *(pntr+i-29) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp0;
                }
            }
        }
exit_conv_grp0:{}
    }
//Group1
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==1)
    {
        for(i=0;i<16;i++)
        {
            pntr = &DataMotor1.F_Motor;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[1].value[i],grp_num[1].factor[i]);
                *(pntr+i-0) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp1;
                }
            }
        }
        for(i=16;i<32;i++)
        {
            pntr = &DataMotor2.F_Motor;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[1].value[i],grp_num[1].factor[i]);
                *(pntr+i-16) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp1;
                }
            }
        }
exit_conv_grp1:{}
    }
//Group2
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==2)
    {
        for(i=22;i<40;i++)
        {
            pntr = &Analog_reg.FilterAIN12;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[2].value[i],grp_num[2].factor[i]);
                *(pntr+i-22) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp2;
                }
            }
        }
        for(i=42;i<54;i++)
        {
            pntr = &AOUT_Reg.FMA_filter;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[2].value[i],grp_num[2].factor[i]);
                if(i==43||i==45||i==47||i==49||i==51||i==53)
                {
                    temp_data = _IQ17div(temp_data,IQ17_100);
                }
                *(pntr+i-42) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp2;
                }
            }
        }
        for(i=54;i<60;i++)
        {
            pntr = &MainIQ_Variable.FAR_Hyst;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[2].value[i],grp_num[2].factor[i]);
                *(pntr+i-54) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp2;
                }
            }
        }
exit_conv_grp2:{}
    }
//Group3
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==3)
    {
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==1)
        {
            MainIQ_Variable.FMin = ConvStg(grp_num[3].value[1],grp_num[3].factor[1]);
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==2)
        {
            MainIQ_Variable.FMax = ConvStg(grp_num[3].value[2],grp_num[3].factor[2]);
        }

        for(i=3;i<8;i++)
        {
            pntr = &Freq_Reg.FJogRev;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[3].value[i],grp_num[3].factor[i]);
                *(pntr+i-3) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp3;
                }
            }
        }

        for(i=10;i<25;i++)
        {
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[3].value[i],grp_num[3].factor[i]);
                arrFreq_MulF[i-10] = (grp_num[3].value[i+15]==1)?_IQ17mpy(temp_data,MinusIQ):temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp3;
                }
            }
        }

        for(i=25;i<40;i++)
        {
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = _IQ17abs(arrFreq_MulF[i-25]);
                if(grp_num[3].value[i]==1)
                {
                    arrFreq_MulF[i-25] = _IQ17mpy(temp_data,MinusIQ);
                }else
                {
                    arrFreq_MulF[i-25] = temp_data;
                }
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp3;
                }
            }
        }

        for(i=40;i<52;i++)
        {
            pntr = &Freq_Reg.InitAcc;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data    = ConvStg(grp_num[3].value[i],grp_num[3].factor[i]);
                *(pntr+i-40) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp3;
                }
            }
        }

        for(i=54;i<61;i++)
        {
            pntr = &Freq_Reg.StageT1;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data    = ConvStg(grp_num[3].value[i],grp_num[3].factor[i]);
                *(pntr+i-54) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp3;
                }
            }
        }
exit_conv_grp3:{}
    }

//Group4
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==4)
    {
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==3)//Kp stall I
        {
            ND2System_Reg.StallI_KP = ConvStg(grp_num[4].value[3],grp_num[4].factor[3]);
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==4)//Ki stall I
        {
            ND2System_Reg.StallI_Ti = ConvStg(grp_num[4].value[4],grp_num[4].factor[4]);
        }
        for(i=6;i<9;i++)
        {
            pntr = &ND2System_Reg.Over_Volt_Stall;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[4].value[i],grp_num[4].factor[i]);
                *(pntr+i-6) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp4;
                }
            }
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==10)//Dynamic brake level
        {
            ND2System_Reg.DynamicBrkLevel = ConvStg(grp_num[4].value[10],grp_num[4].factor[10]);
        }
        for(i=14;i<18;i++)
        {
            pntr = &MainIQ_Variable.JumpF1;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data    = ConvStg(grp_num[4].value[i],grp_num[4].factor[i]);
                *(pntr+i-14) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp4;
                }
            }
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==19)//Level voltage search
        {
            temp_data = ConvStg(grp_num[4].value[19],grp_num[4].factor[19]);
            FlyStrt_Reg.LevelVSearch = _IQ17div(temp_data,IQ17_100);
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==20)//Dec. search
        {
            FlyStrt_Reg.TDecelSearch = ConvStg(grp_num[4].value[20],grp_num[4].factor[20]);
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==23)//Dec. LU case2
        {
            ChkFault_Reg.DecLU = ConvStg(grp_num[4].value[23],grp_num[4].factor[23]);
        }
exit_conv_grp4:{}
    }

//Group5
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==5)
    {
        for(i=2;i<9;i++)
        {
            pntr = &ND2System_Reg.Vboost;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[5].value[i],grp_num[5].factor[i]);
                if(i==2)
                {
                    temp_data = _IQ17div(temp_data,IQ17_100);
                }
                *(pntr+i-2) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp5;
                }
            }
        }
        for(i=9;i<20;i++)
        {
            pntr = &Vecter_Reg.Kn_SpeedLoop;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[5].value[i],grp_num[5].factor[i]);
                *(pntr+i-9) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp5;
                }
            }
        }
        for(i=21;i<27;i++)
        {
            pntr = &ND2System_Reg.DCBreak_Level;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[5].value[i],grp_num[5].factor[i]);
                if(i==21)
                {
                    temp_data = _IQ17div(temp_data,IQ17_100);
                }
                *(pntr+i-21) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp5;
                }
            }
        }
        for(i=34;i<37;i++)
        {
            pntr = &TrqCmnd_Reg.TrqdAcc;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[5].value[i],grp_num[5].factor[i]);
                if(i==36)
                {
                    temp_data = _IQ17div(temp_data,IQ17_100);
                }
                *(pntr+i-34) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp5;
                }
            }
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==39)//F541
        {
            Hpwm_Reg.FreqEna2Arm = ConvStg(grp_num[5].value[39],grp_num[5].factor[39]);
        }
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==40)//F541
        {
            Hpwm_Reg.FreqDis2Arm = ConvStg(grp_num[5].value[40],grp_num[5].factor[40]);
        }

exit_conv_grp5:{}
    }
//Group6
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==6)
    {
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==2)//Line speed co-efficient
        {
            KeyPad_reg.LineSpdCoef = ConvStg(grp_num[6].value[2],grp_num[6].factor[2]);
        }
    }
//Group7
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || grp==7)
    {
        for(i=4;i<11;i++)
        {
            pntr = &PID_Reg.PID_TAccelSp;
            if(i==7)
            {
                i=8;
            }
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[7].value[i],grp_num[7].factor[i]);
                if(i==6||i==8||i==9||i==10)
                {
                    temp_data = _IQ17div(temp_data,IQ17_100);
                }
                if(i>=8||sub_grp>=8)
                {
                    *(pntr+i-5) = temp_data;
                }else
                {
                    *(pntr+i-4) = temp_data;
                }
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp7;
                }
            }
        }
        for(i=12;i<19;i++)
        {
            pntr = &PID_Reg.Kp;
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[7].value[i],grp_num[7].factor[i]);
                if(i==15||i==16||i==18)
                {
                    temp_data = _IQ17div(temp_data,IQ17_100);
                }
                *(pntr+i-12) = temp_data;
                if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
                {
                    goto exit_conv_grp7;
                }
            }
        }
        for(i=20;i<35;i++)
        {
            if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0 || sub_grp==i)
            {
                temp_data = ConvStg(grp_num[7].value[i],grp_num[7].factor[i]);
                arrPID_MulF[i-20] = _IQ17div(temp_data,IQ17_100);
            }
        }
exit_conv_grp7:{}
    }

    //converse general data
    MainIQ_Variable.PU_FMax = _IQ17div(MainIQ_Variable.FMax,MainIQ_Variable.FRef);
    MainIQ_Variable.PU_FMin = _IQ17div(MainIQ_Variable.FMin,MainIQ_Variable.FRef);
} 
//////////////////////////////////////////////////////////////////
void DataDisp(void)
{
_iq buff = 0;
//---------------------------------------------------------------
    //      control 7segment

    if(KeyPad_reg.MODE_FLAG.bit.setting_mode == 1)
    {
//chck display normal state--------------------------------------
        if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_norm_qck)
        {
            KeyPad_reg.BUFFERDATAH.bit.Data_Mode = 1;
            KeyPad_reg.BUFFERDATAL.bit.fmode     = func_disp;
            KeyPad_reg.BUFFERDATAL.bit.data1     = KeyPad_reg.PNTR_MODE.bit.PntrQck_mode;
            if(KeyPad_reg.PNTR_MODE.bit.PntrQck_mode!=0)
            {
                DspDataFnt();
            }
        }
//chck display group state-----------------------------------------------
        else if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_grp)
        {
            KeyPad_reg.BUFFERDATAH.bit.Data_Mode = 0;
            KeyPad_reg.BUFFERDATAL.bit.fmode     = grp_disp;
            KeyPad_reg.BUFFERDATAL.bit.data1     = KeyPad_reg.PNTR_MODE.bit.PntrGrp;
        }
//chck display sub group state-------------------------------------------
        else if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_subfunct)
        {
            DspDataFnt();
            KeyPad_reg.BUFFERDATAL.bit.shpass=0;

            if(KeyPad_reg.PNTR_MODE.bit.PntrGrp==8)
            {
                if(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc==6
                   || (*PntrFact & 0x0800)!=0 )//Factory passward and passward factor
                {
                    KeyPad_reg.BUFFERDATAH.bit.data2      = 0;
                    KeyPad_reg.BUFFERDATAH.bit.Unit_SubFN = UnitNo;
                    KeyPad_reg.BUFFERDATAL.bit.shpass     = 1;
                }
            }

            if(KeyPad_reg.DISP_FLAG.bit.DispPSW==1)
            {
                KeyPad_reg.BUFFERDATAL.bit.shpass    = 1;
                KeyPad_reg.BUFFERDATAH.bit.Data_Mode = 1;
                goto disp_setting;
            }
            KeyPad_reg.BUFFERDATAL.bit.fmode=func_disp;
            KeyPad_reg.BUFFERDATAL.bit.data1= KeyPad_reg.PNTR_MODE.bit.PntrSubFunc+(KeyPad_reg.PNTR_MODE.bit.PntrGrp * 100);//load by pointer in array group
        }
//chck display setting state--------------------------------------------
        else if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_setting)//display in setting up/down state//--->
        {
            KeyPad_reg.BUFFERDATAH.bit.Data_Mode = 1;

            KeyPad_reg.BUFFERDATAL.bit.shpass = (*PntrFact & 0x0800)>>11;
//-----------------------------------------------------------------------
//shw data when setting
disp_setting:
            KeyPad_reg.BUFFERDATAH.bit.data2 =
            KeyPad_reg.PNTR_MODE.bit.PntrSubFunc+(KeyPad_reg.PNTR_MODE.bit.PntrGrp * 100);

            KeyPad_reg.BUFFERDATAH.bit.Unit_SubFN =
             *(Pntr_UnitSubFN[KeyPad_reg.PNTR_MODE.bit.PntrGrp]+(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));

            KeyPad_reg.BUFFERDATAL.bit.fmode=data_disp;

            if(KeyPad_reg.DISP_FLAG.bit.DispPreBuf ==1)
            {

                KeyPad_reg.BUFFERDATAL.bit.data1 = PreBufCtrSetting;
                KeyPad_reg.BUFFERDATAL.bit.decib = BufferPreDECIB;
                KeyPad_reg.BUFFERDATAL.bit.sign  = BufferPreSIGN>>1;
//---------------------------------------------------------------------

            }
//diplay real value
            else
            {
                KeyPad_reg.BUFFERDATAL.bit.data1 = *PntrVal;
                KeyPad_reg.BUFFERDATAL.bit.decib = (*PntrFact & 0x0300)>>8;
                KeyPad_reg.BUFFERDATAL.bit.sign  = (*PntrFact & 0x2000)>>13;
            }
//-----------------------------------------------------------------------
//control blinking
            if(KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg==1 && KeyPad_reg.DISP_FLAG.bit.NoBlink==0)
            {
                KeyPad_reg.BUFFERDATAL.bit.resodisp = KeyPad_reg.GENERAL_FLAG.bit.StateDig;
            }

            else
            {
                KeyPad_reg.BUFFERDATAL.bit.resodisp = 0;
            }
//---------------------------------------------------------------------
        }
    }

    else//end of setting mode
    {
        KeyPad_reg.BUFFERDATAH.bit.Data_Mode = 0;                           //display normal mode
        KeyPad_reg.BUFFERDATAH.bit.Run_Comm  = grp_num[2].value[0];          //display local/remote

        KeyPad_reg.BUFFERDATAH.bit.Status_Run = (ND2System_Reg.STATE_FLAG.bit.State & 0x01);    //display run/stop

        buff = (ND2System_Reg.MAIN_FLAG2.bit.CmdType==0)?MainIQ_Variable.FCommand:TrqCmnd_Reg.TrqCommand;
        if(buff<0)
        {
            KeyPad_reg.BUFFERDATAH.bit.Dir_M = Dir_rev;
        }

        else if(buff>0)
        {
            KeyPad_reg.BUFFERDATAH.bit.Dir_M = Dir_fwd;
        }
//      KeyPad_reg.BUFFERDATAH.bit.Dir_M = (buff<0)?Dir_rev:Dir_fwd;//KeyPad_reg.DISP_FLAG.bit.Fwd_Rev

        if(ChkFault_Reg.FLAG_FAULT.bit.W_OC == 1)
        {
            KeyPad_reg.BUFFERDATAH.bit.Status_InvB = 1;                 //display W-OC
        }

        else if(ChkFault_Reg.FLAG_FAULT.bit.W_OL == 1)
        {
            KeyPad_reg.BUFFERDATAH.bit.Status_InvB = 2;                 //display W-OL
        }

        else
        {
            KeyPad_reg.BUFFERDATAH.bit.Status_InvB = 0;
        }
        //-----------------------------------------------------------------------
        if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready==0)
        {
            if(grp_num[0].value[14]==0)//check model LCD or 7segment
            {//case 7seg
                KeyPad_reg.BUFFERDATAL.bit.fmode = tab_disp;
                KeyPad_reg.BUFFERDATAL.bit.data1 = 52;
            }

            else
            {
                KeyPad_reg.BUFFERDATAH.bit.Status_InvA = 8;                     //display not ready
                goto normaldisp;
            }
        }

        else
        {
            if(DgtlPrt_Reg.DataLDIN.bit.Coast_stop==1)
            {
                KeyPad_reg.BUFFERDATAH.bit.Status_InvA = 3;                     //display FER
            }

            else if(Flag_DCbreak==1)
            {
                KeyPad_reg.BUFFERDATAH.bit.Status_InvA = 1;                     //Display DC
            }

            else if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive==1)
            {
                KeyPad_reg.BUFFERDATAH.bit.Status_InvA = 2;                     //Display flying
            }

            else
            {                                                                   //display ACC/DEC/STD/Ready
                KeyPad_reg.BUFFERDATAH.bit.Status_InvA = ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd+4; //+4 for macthing between system control and keypad variables
            }
normaldisp:
            if(KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg==1 && KeyPad_reg.DISP_FLAG.bit.NoBlink==0)
            {
                KeyPad_reg.BUFFERDATAL.bit.fmode = tab_disp;
                KeyPad_reg.BUFFERDATAL.bit.data1 = 50;                      //no display when blinking
            }

            else
            {
                KeyPad_reg.BUFFERDATAL.bit.fmode    = data_disp;
                KeyPad_reg.BUFFERDATAL.bit.resodisp = 0;
                KeyPad_reg.BUFFERDATAL.bit.data1    = Norm_Val_Disp;
                KeyPad_reg.BUFFERDATAL.bit.decib    = Norm_Dec_Disp;
                KeyPad_reg.BUFFERDATAL.bit.unit     = Norm_Unit_Disp;
                KeyPad_reg.BUFFERDATAL.bit.sign     = KeyPad_reg.DISP_FLAG.bit.SignNormDisp;
            }
        }
    }
    //check display table----------------------------------------------------
    KeyPad_reg.BUFFERDATAL.bit.fault = 0;
    if(KeyPad_reg.DISP_TAB.all != 0 || ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault == 1)
    {
        if(KeyPad_reg.MODE_FLAG.bit.setting_mode == 0)//check current mode setting or normal
        {
            KeyPad_reg.BUFFERDATAH.all       = 0;
            KeyPad_reg.BUFFERDATAL.bit.fmode = tab_disp;

            if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault == 1)
            {
                KeyPad_reg.BUFFERDATAL.bit.fault = 1;//in normal mode and there is fault now
                KeyPad_reg.BUFFERDATAL.bit.data1 = (KeyPad_reg.DISP_FLAG.bit.GeneralBlk==1)? ChkFault_Reg.Fault_Buffer:50;//blinking fault data
            }

            else
            {
                goto table_setting;//table setting is hi/lo/---
            }
        }

        else if(KeyPad_reg.DISP_TAB.all!=0)//case of setting mode
        {
            KeyPad_reg.BUFFERDATAH.all       = 0;
            KeyPad_reg.BUFFERDATAL.bit.fmode = tab_disp;
            if(KeyPad_reg.DISP_TAB.bit.Disp_fault==1)
            {
                KeyPad_reg.BUFFERDATAL.bit.data1 = ar_fault[*PntrVal];
            }

            else
            {
table_setting:
                KeyPad_reg.BUFFERDATAL.bit.data1 = Table_Disp[KeyPad_reg.DISP_TAB.bit.Disp_Table];
            }
        }
    }
    //end of display of 7segment

    //---------------------------------------------------------------
    //      control LED run/stop

    // Gus
    // Add OL and OC flag to generate the blinking signal
    /*if(ND2System_Reg.STATE_FLAG.bit.State == state_stopping
            ||First_Test_ND2 == 1
            ||ND2System_Reg.MAIN_FLAG2.bit.TestNoFault == 1
            ||ND2System_Reg.MAIN_FLAG2.bit.Test == 1
            ||ChkFault_Reg.FLAG_FAULT.bit.W_OL == 1
            ||ChkFault_Reg.FLAG_FAULT.bit.W_OC == 1)*/

    if(ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
    {
        KeyPad_reg.BUFFERDATAL.bit.run = KeyPad_reg.DISP_FLAG.bit.GeneralBlk;
    }
    else if(ChkFault_Reg.FLAG_FAULT.bit.W_OL == 1)
    {
        KeyPad_reg.BUFFERDATAL.bit.run = KeyPad_reg.DISP_FLAG.bit.GeneralBlk;
    }
    else if(ChkFault_Reg.FLAG_FAULT.bit.W_OC == 1)
    {
        KeyPad_reg.BUFFERDATAL.bit.run = KeyPad_reg.DISP_FLAG.bit.GeneralBlk;
    }
    else
    {
        KeyPad_reg.BUFFERDATAL.bit.run = ~(ND2System_Reg.STATE_FLAG.bit.State & 0x1);
    }
    //---------------------------------------------------------------
    //      control LED VR

    //KeyPad_reg.BUFFERDATAL.bit.vrcom = 1; //1
    if (grp_num[0].value[14]==0) //7 segment
    {
        if ((grp_num[3].value[0]==2)||(grp_num[3].value[0]==15))
        {
            KeyPad_reg.BUFFERDATAL.bit.vrcom = 1; //1
            MSG_MBX1DataL = KeyPad_reg.BUFFERDATAL.all;
            MSG_MBX1DataH = KeyPad_reg.BUFFERDATAH.all;
        }
        else
        {
            KeyPad_reg.BUFFERDATAL.bit.vrcom = 0;
            MSG_MBX1DataL = KeyPad_reg.BUFFERDATAL.all;
            MSG_MBX1DataH = KeyPad_reg.BUFFERDATAH.all;
        }
    }
    else if (grp_num[0].value[14]==1)//LCD
    {
        if (grp_num[3].value[0]==15)
        {
            KeyPad_reg.BUFFERDATAL.bit.vrcom = 1; //1
            MSG_MBX1DataL = KeyPad_reg.BUFFERDATAL.all;
            MSG_MBX1DataH = KeyPad_reg.BUFFERDATAH.all;
        }
        else
        {
            KeyPad_reg.BUFFERDATAL.bit.vrcom = 0;
            MSG_MBX1DataL = KeyPad_reg.BUFFERDATAL.all;
            MSG_MBX1DataH = KeyPad_reg.BUFFERDATAH.all;
        }
    }


    //MSG_MBX1DataL = KeyPad_reg.BUFFERDATAL.all;
    //MSG_MBX1DataH = KeyPad_reg.BUFFERDATAH.all;
}
////////////////////////////////////////////////////////////////// 
void DspDataFnt(void)
{
    KeyPad_reg.BUFFERDATAH.bit.data2 = *PntrVal;
    KeyPad_reg.BUFFERDATAL.bit.sign  = (*PntrFact & 0x2000)>>13;
    KeyPad_reg.BUFFERDATAL.bit.decib = (*PntrFact & 0x0300)>>8;
    KeyPad_reg.BUFFERDATAH.bit.Unit_SubFN
    = *(Pntr_UnitSubFN[KeyPad_reg.PNTR_MODE.bit.PntrGrp]
                       +(KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1));
}
////////////////////////////////////////////////////////////////// 
void SelectFunct(void)
{
Uint16 buffcnt=0;
    void RunQckMode(void);

    if(KeyPad_reg.MODE_FLAG.bit.setting_mode == 1)
    {
        if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_norm_qck)
        {
            RunQckMode();//setting mode at state0
        }

        if(KeyPad_reg.KEY_FLAG.bit.key_prog==1)
        {
            if((KeyPad_reg.MODE_FLAG.bit.mode_state > state_norm_qck) && (KeyPad_reg.MODE_FLAG.bit.mode_state < state_setting))
            {
                if(KeyPad_reg.PNTR_MODE.bit.PntrQck_mode == 0 && KeyPad_reg.DISP_FLAG.bit.DispPSW == 0)
                {
                    KeyPad_reg.MODE_FLAG.bit.mode_state--;
                    ClearKey();
                }
                if(KeyPad_reg.DISP_FLAG.bit.DispPSW == 1)
                {
                    ClrStSetting();
                }
            }
        }

        if(KeyPad_reg.PNTR_MODE.bit.PntrQck_mode == 0)//enter to selecting function group only in qck=0
        {
            if(KeyPad_reg.KEY_FLAG.bit.key_data == 1)
            {
                if(KeyPad_reg.MODE_FLAG.bit.mode_state<state_subfunct)
                {
                    KeyPad_reg.MODE_FLAG.bit.mode_state++;
                    ClearKey();
                }
            }else if((KeyPad_reg.KEY_FLAG.bit.key_dwn == 1)||(KeyPad_reg.KEY_FLAG.bit.key_up == 1))
            {
                if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_grp)
                {
                    buffcnt = KeyPad_reg.PNTR_MODE.bit.PntrGrp;
                    buffcnt = run_updwncntr(MaxMainGrp,
                                            KeyPad_reg.GENERAL_FLAG.bit.MinMainMode,
                                            buffcnt,1);
                    KeyPad_reg.PNTR_MODE.bit.PntrGrp     = buffcnt;
                    KeyPad_reg.PNTR_MODE.bit.PntrSubFunc = 1;
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
void CheckMode(void)
{
    if(KeyPad_reg.KEY_FLAG.bit.key_prog == 1)
    {
        //---------------------Gus-----------------------------
        Tmr_Mode_Set = tm_delay_data;
        KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
        //---------------------Gus-----------------------------

        if(KeyPad_reg.MODE_FLAG.bit.mode_state == state_norm_qck)
        {
            KeyPad_reg.MODE_FLAG.bit.setting_mode = ~KeyPad_reg.MODE_FLAG.bit.setting_mode;
            if(KeyPad_reg.PNTR_MODE.bit.PntrQck_mode != 0)
            {
                KeyPad_reg.PNTR_MODE.bit.PntrGrp     = KeyPad_reg.GENERAL_FLAG.bit.MinMainMode;
                KeyPad_reg.PNTR_MODE.bit.PntrSubFunc = 1;
            }
            ClrStSetting1();
            ClearKey();
        }else if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault != 0)
        {
            if(KeyPad_reg.MODE_FLAG.bit.setting_mode == 0)
            {
                KeyPad_reg.MODE_FLAG.bit.setting_mode = 1;
                KeyPad_reg.KEY_FLAG.bit.key_prog      = 0;
            }
        }
    }
    //-------------------Gus-------------------------
    else if (KeyPad_reg.MODE_FLAG.bit.setting_mode == 0)
    {
        KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 0;
    }
    else if (KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg == 0)
    {
        KeyPad_reg.MODE_FLAG.bit.mode_state   = state_norm_qck;
        KeyPad_reg.MODE_FLAG.bit.setting_mode = 0;
    }
    //-------------------Gus-------------------------
}
//////////////////////////////////////////////////////////////////
void CheckKeyIn(void)
{

    if((BufferMBX0L & 0x000000FF)==no_key) //run on no key
    {
        KeyPad_reg.KEY_FLAG.all = KeyPad_reg.KEY_FLAG.all & 0x0300;//clear unpressing
        KeyPad_reg.TRIGGER_BIT.bit.open_loop_cntr = 0;//select bit loop or stop counter
        ClearKey();
    }
    else if((BufferMBX0L & 0x000000FF)==sw_stop)
    {
        if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault == 1)
        {//case of under faulty
            if(KeyPad_reg.KEY_FLAG.bit.state_key1 == 0)
            {
                Tmr_key_next_step = t_key_clrflt; //Press Stop key 2 s.

                KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg = 1;
                KeyPad_reg.KEY_FLAG.bit.state_key1         = 1;
            }
            else
            {
                if(KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg == 0)
                {
                    Clr_Fault();
                }
            }
        }
        else
        {//no fault basic stop key
            if(KeyPad_reg.KEY_FLAG.bit.state_key1 == 0)
            {
                KeyPad_reg.KEY_FLAG.bit.state_key1 = 1;
                KeyPad_reg.KEY_FLAG.bit.key_stop   = 1;
            }
        }
    }
    else if(((BufferMBX0L & 0x000000FF) == sw_prog) && (KeyPad_reg.KEY_FLAG.bit.state_key1 == 0))
    //else if((KeyPad_reg.BUFFERBUTTON.bit.BuffSW_PROG == 1) && (KeyPad_reg.KEY_FLAG.bit.state_key1 == 0))
    {
        KeyPad_reg.KEY_FLAG.bit.key_prog   = 1;
        KeyPad_reg.KEY_FLAG.bit.state_key1 = 1;
//----------------------------------------------------------
    }
    else if((ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault==0)&&(BufferMBX0L & 0x000000FF)==sw_run)
    {
        if(KeyPad_reg.KEY_FLAG.bit.state_key1 == 0)
        {
            KeyPad_reg.KEY_FLAG.bit.state_key1 = 1;
            KeyPad_reg.KEY_FLAG.bit.key_run    = 1;
        }
//----------------------------------------------------------
    }
    else if(KeyPad_reg.MODE_FLAG.bit.setting_mode == 0)
    {
        if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault == 0)
        {
            goto check_key;
        }
    }
    else
    {
check_key:
        if(((BufferMBX0L & 0x000000FF)==sw_data)&&(KeyPad_reg.KEY_FLAG.bit.state_key1 ==0))
        {
            KeyPad_reg.TIMER0.bit.Time_blink    = 0;
            KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg = 0;
            KeyPad_reg.KEY_FLAG.bit.key_data    = 1;
            KeyPad_reg.KEY_FLAG.bit.state_key1  = 1;
//----------------------------------------------------------
        }
        else if((BufferMBX0L & 0x000000FF)==sw_up)
        {
            KeyPad_reg.TIMER0.bit.Time_blink    = 0;
            KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg = 0;
            if(KeyPad_reg.KEY_FLAG.bit.state_key1 == 0)
            {
                KeyPad_reg.KEY_FLAG.bit.state_key1 = 1;
                KeyPad_reg.KEY_FLAG.bit.key_up     = 1;

                Tmr_key_next_step = t_key_next_step;

                KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg = 1;
                KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg     = 0;
            }
            else
            {
                Up_speedUpDown();
            }
        }
//----------------------------------------------------------
        else if((BufferMBX0L & 0x000000FF)==sw_down)
        {
            KeyPad_reg.TIMER0.bit.Time_blink    = 0;
            KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg = 0;
            if(KeyPad_reg.KEY_FLAG.bit.state_key1 == 0)
            {
                KeyPad_reg.KEY_FLAG.bit.state_key1 = 1;
                KeyPad_reg.KEY_FLAG.bit.key_dwn    = 1;

                Tmr_key_next_step = t_key_next_step;

                KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg = 1;
                KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg     = 0;
            }
            else
            {
                Up_speedUpDown();
            }
//----------------------------------------------------------
        }
        else if((BufferMBX0L & 0x000000FF) == sw_shift && (KeyPad_reg.KEY_FLAG.bit.state_key1 == 0))
        {
            KeyPad_reg.KEY_FLAG.bit.key_shift  = 1;
            KeyPad_reg.KEY_FLAG.bit.state_key1 = 1;
        }
//----------------------------------------------------------
    }
}
//////////////////////////////////////////////////////////////////
void Up_speedUpDown(void)
{
    if(KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg == 0)
    {
        if(KeyPad_reg.KEY_FLAG.bit.state_speed_updwn<7)
        {
            if(Tmr_CNTR_UpDown1<2)
            {
                KeyPad_reg.KEY_FLAG.bit.state_speed_updwn++;
                Tmr_CNTR_UpDown = Tmr_CNTR_UpDown -= stepdown;

                Tmr_key_next_step = t_key_next_step;
                KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg = 1;

                Tmr_CNTR_UpDown1 = Tmr_CNTR_UpDown;
                KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg     = 0;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
void ClearKey(void)
{   
    Tmr_key_next_step = 0;

    KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg = 0;
    KeyPad_reg.KEY_FLAG.all = KeyPad_reg.KEY_FLAG.all & 0x0019;

    Tmr_CNTR_UpDown = FirstSpeed;
}
//////////////////////////////////////////////////////////////////
void RunQckMode(void)
{
    KeyPad_reg.MODE_FLAG.bit.PrePntr = 0;

    while(ar_qckmode[KeyPad_reg.MODE_FLAG.bit.PrePntr]!=KeyPad_reg.PNTR_MODE.bit.PntrQck_mode)
    {
        KeyPad_reg.MODE_FLAG.bit.PrePntr++;
    }

    if((KeyPad_reg.KEY_FLAG.bit.key_dwn==1)||(KeyPad_reg.KEY_FLAG.bit.key_up==1))
    {
        //---------------Gus------------------
        Tmr_Mode_Set = tm_delay_data;
        KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
        //---------------Gus------------------

        KeyPad_reg.MODE_FLAG.bit.PrePntr = run_updwncntr(max_qck, min_qck,
                                                         KeyPad_reg.MODE_FLAG.bit.PrePntr, 1);

        KeyPad_reg.PNTR_MODE.bit.PntrGrp     = KeyPad_reg.GENERAL_FLAG.bit.MinMainMode;
        KeyPad_reg.PNTR_MODE.bit.PntrSubFunc = 1;
    }

    KeyPad_reg.PNTR_MODE.bit.PntrQck_mode = ar_qckmode[KeyPad_reg.MODE_FLAG.bit.PrePntr];

    if(KeyPad_reg.PNTR_MODE.bit.PntrQck_mode != 0)
    {
        KeyPad_reg.PNTR_MODE.bit.PntrGrp     = KeyPad_reg.PNTR_MODE.bit.PntrQck_mode/100;
        KeyPad_reg.PNTR_MODE.bit.PntrSubFunc = KeyPad_reg.PNTR_MODE.bit.PntrQck_mode%100;

        PntrVal  = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].value[KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1];
        PntrFact = &grp_num[KeyPad_reg.PNTR_MODE.bit.PntrGrp].factor[KeyPad_reg.PNTR_MODE.bit.PntrSubFunc-1];

        if(KeyPad_reg.KEY_FLAG.bit.key_data==1)
        {
            ClearKey();
            if(KeyPad_reg.PNTR_MODE.bit.PntrGrp!=8)
            {
                KeyPad_reg.MODE_FLAG.bit.mode_state = state_setting;
            }
            else
            {
                KeyPad_reg.MODE_FLAG.bit.mode_state = state_subfunct;
                KeyPad_reg.KEY_FLAG.bit.key_data    = 1;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////
Uint16 run_updwncntr(Uint16 max_valueA, Uint16 min_valueA, Uint16 actuator, Uint16 step_count)
{
        if(KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg==0)
        {
            Tmr_CNTR_UpDown1 = Tmr_CNTR_UpDown;
            KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg=1;

            if(KeyPad_reg.KEY_FLAG.bit.key_up == 1)
            {
                //---------------Gus------------------
                Tmr_Mode_Set = tm_delay_data;
                KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
                //---------------Gus------------------

                if(BufferPreSIGN==3)
                {
                    goto Dn_actuator;
                }
                else
                {
                    goto Up_actuator;
                }
            }
            else if(KeyPad_reg.KEY_FLAG.bit.key_dwn == 1)
            {
                //---------------Gus------------------
                Tmr_Mode_Set = tm_delay_data;
                KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 1;
                //---------------Gus------------------

                if(BufferPreSIGN==3)
                {
Up_actuator:        if(actuator<max_valueA)
                    {
                        actuator = actuator+step_count;
                    }
                    else if(KeyPad_reg.TRIGGER_BIT.bit.open_loop_cntr==0)
                    {
                        actuator = min_valueA;//return to first value
                    }
                }
                else
                {
Dn_actuator:        if(actuator>min_valueA)
                    {
                        actuator = actuator-step_count;
                    }
                    else  if(KeyPad_reg.TRIGGER_BIT.bit.open_loop_cntr==0)
                    {
                        actuator = max_valueA;
                    }
                }
            }
        }
    return actuator;
}
///////////////////////////////////////////////////////////////////
void LoadDefault(Uint16 StartGrp1)
{
    Uint16  i=StartGrp1,j=0;
reload_default :
    if(i<GrpNumber)
    {
        for(j=0; j < Table_MaxNumGrp[i]; j++)
            grp_num[i].value[j] = *(Pntr_def[i]+j);

        for(j=0; j < Table_MaxNumGrp[i]; j++)
            grp_num[i].factor[j] = *(Pntr_factor[i]+j);
        i++;
        goto reload_default;
    }
    //Load current motor parameter from rated inverter
    grp_num[1].value[2]   = grp_num[0].value[3];
    grp_num[1].value[18]  = grp_num[0].value[3];
    grp_num[1].factor[2]  = grp_num[0].factor[3];
    grp_num[1].factor[18] = grp_num[0].factor[3];
    //---------------------------------------Gus---------------------------------------
    if (grp_num[0].value[11]==0)//----------Small mainboard
    {
        grp_num[3].value[0]  = 2;
        grp_num[3].value[44] = 50;
        grp_num[3].value[45] = 50;
        grp_num[5].value[2]  = 50;

        //-----------------Rated power F105,F121------------------------------------------------
            if (grp_num[0].value[4] == 0)//-----------------------------------200V
            {
                if (MainIQ_Variable.I_Rate == _IQ17(3))
                {
                    grp_num[1].value[4]  = 40;
                    grp_num[1].value[20] = 40;
                }
                else if (MainIQ_Variable.I_Rate >= _IQ17(5.3)
                        && MainIQ_Variable.I_Rate < _IQ17(6))
                {
                    grp_num[1].value[4]  = 75;
                    grp_num[1].value[20] = 75;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(8))
                {
                    grp_num[1].value[4]  = 150;
                    grp_num[1].value[20] = 150;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(11))
                {
                    grp_num[1].value[4]  = 220;
                    grp_num[1].value[20] = 220;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(17.5))
                {
                    grp_num[1].value[4]  = 400;
                    grp_num[1].value[20] = 400;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(26))
                {
                    grp_num[1].value[4]  = 550;
                    grp_num[1].value[20] = 550;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(34))
                {
                    grp_num[1].value[4]  = 750;
                    grp_num[1].value[20] = 750;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(49))
                {
                    grp_num[1].value[4]  = 1100;
                    grp_num[1].value[20] = 1100;
                }
            }
            else if (grp_num[0].value[4] == 1)//------------------------------400V
            {
                if (MainIQ_Variable.I_Rate == _IQ17(1.5))
                {
                    grp_num[1].value[4]  = 40;
                    grp_num[1].value[20] = 40;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(3.5))
                {
                    grp_num[1].value[4]  = 75;
                    grp_num[1].value[20] = 75;
                }
                else if (MainIQ_Variable.I_Rate >= _IQ17(4.8)
                        && MainIQ_Variable.I_Rate < _IQ17(5) )
                {
                    grp_num[1].value[4]  = 150;
                    grp_num[1].value[20] = 150;
                }
                else if (MainIQ_Variable.I_Rate >= _IQ17(6.2)
                        && MainIQ_Variable.I_Rate < _IQ17(7))
                {
                    grp_num[1].value[4]  = 220;
                    grp_num[1].value[20] = 220;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(11))
                {
                    grp_num[1].value[4]  = 400;
                    grp_num[1].value[20] = 400;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(15))
                {
                    grp_num[1].value[4]  = 550;
                    grp_num[1].value[20] = 550;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(18))
                {
                    grp_num[1].value[4]  = 750;
                    grp_num[1].value[20] = 750;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(27))
                {
                    grp_num[1].value[4]  = 1100;
                    grp_num[1].value[20] = 1100;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(34))
                {
                    grp_num[1].value[4]  = 1500;
                    grp_num[1].value[20] = 1500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(44))
                {
                    grp_num[1].value[4]  = 1850;
                    grp_num[1].value[20] = 1850;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(50))
                {
                    grp_num[1].value[4]  = 2200;
                    grp_num[1].value[20] = 2200;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(65))
                {
                    grp_num[1].value[4]  = 3000;
                    grp_num[1].value[20] = 3000;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(80))
                {
                    grp_num[1].value[4]  = 3700;
                    grp_num[1].value[20] = 3700;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(96))
                {
                    grp_num[1].value[4]  = 4500;
                    grp_num[1].value[20] = 4500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(128))
                {
                    grp_num[1].value[4]  = 5500;
                    grp_num[1].value[20] = 5500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(165))
                {
                    grp_num[1].value[4]  = 7500;
                    grp_num[1].value[20] = 7500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(186))
                {
                    grp_num[1].value[4]  = 9000;
                    grp_num[1].value[20] = 9000;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(224))
                {
                    grp_num[1].value[4]  = 1100;
                    grp_num[1].factor[4] = 0x0151;
                    grp_num[1].value[20] = 1100;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(253))
                {
                    grp_num[1].value[4]   = 1320;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 1320;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(304))
                {
                    grp_num[1].value[4]   = 1600;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 1600;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(340))
                {
                    grp_num[1].value[4]   = 1850;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 1850;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(377))
                {
                    grp_num[1].value[4]   = 2000;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 2000;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(430))
                {
                    grp_num[1].value[4]   = 2200;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 2200;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(510))
                {
                    grp_num[1].value[4]   = 2500;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 2500;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(590))
                {
                    grp_num[1].value[4]   = 3150;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 3150;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(675))
                {
                    grp_num[1].value[4]   = 3550;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 3550;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(740))
                {
                    grp_num[1].value[4]   = 4000;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 4000;
                    grp_num[1].factor[20] = 0x0151;
                }
            }
    }
    else//------------Large mainboard
    {
        //-----------------Rated power F105,F121------------------------------------------------
            if (grp_num[0].value[4] == 0)//-----------------------------------200V
            {
                if (MainIQ_Variable.I_Rate == _IQ17(3))
                {
                    grp_num[1].value[4]  = 40;
                    grp_num[1].value[20] = 40;
                }
                else if (MainIQ_Variable.I_Rate >= _IQ17(5.3)
                        && MainIQ_Variable.I_Rate < _IQ17(6))
                {
                    grp_num[1].value[4]  = 75;
                    grp_num[1].value[20] = 75;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(8))
                {
                    grp_num[1].value[4]  = 150;
                    grp_num[1].value[20] = 150;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(11))
                {
                    grp_num[1].value[4]  = 220;
                    grp_num[1].value[20] = 220;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(17.5))
                {
                    grp_num[1].value[4]  = 400;
                    grp_num[1].value[20] = 400;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(26))
                {
                    grp_num[1].value[4]  = 550;
                    grp_num[1].value[20] = 550;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(34))
                {
                    grp_num[1].value[4]  = 750;
                    grp_num[1].value[20] = 750;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(49))
                {
                    grp_num[1].value[4]  = 1100;
                    grp_num[1].value[20] = 1100;
                }
            }
            else if (grp_num[0].value[4] == 1)//------------------------------400V
            {
                if (MainIQ_Variable.I_Rate == _IQ17(1.5))
                {
                    grp_num[1].value[4]  = 40;
                    grp_num[1].value[20] = 40;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(3.5))
                {
                    grp_num[1].value[4]  = 75;
                    grp_num[1].value[20] = 75;
                }
                else if (MainIQ_Variable.I_Rate >= _IQ17(4.8)
                        && MainIQ_Variable.I_Rate < _IQ17(5))
                {
                    grp_num[1].value[4]  = 150;
                    grp_num[1].value[20] = 150;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(6.2)
                        && MainIQ_Variable.I_Rate < _IQ17(7))
                {
                    grp_num[1].value[4]  = 220;
                    grp_num[1].value[20] = 220;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(11))
                {
                    grp_num[1].value[4]  = 400;
                    grp_num[1].value[20] = 400;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(15))
                {
                    grp_num[1].value[4]  = 550;
                    grp_num[1].value[20] = 550;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(18))
                {
                    grp_num[1].value[4]  = 750;
                    grp_num[1].value[20] = 750;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(27))
                {
                    grp_num[1].value[4]  = 1100;
                    grp_num[1].value[20] = 1100;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(34))
                {
                    grp_num[1].value[4]  = 1500;
                    grp_num[1].value[20] = 1500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(44))
                {
                    grp_num[1].value[4]  = 1850;
                    grp_num[1].value[20] = 1850;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(50))
                {
                    grp_num[1].value[4]  = 2200;
                    grp_num[1].value[20] = 2200;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(65))
                {
                    grp_num[1].value[4]  = 3000;
                    grp_num[1].value[20] = 3000;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(80))
                {
                    grp_num[1].value[4]  = 3700;
                    grp_num[1].value[20] = 3700;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(96))
                {
                    grp_num[1].value[4]  = 4500;
                    grp_num[1].value[20] = 4500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(128))
                {
                    grp_num[1].value[4]  = 5500;
                    grp_num[1].value[20] = 5500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(165))
                {
                    grp_num[1].value[4]  = 7500;
                    grp_num[1].value[20] = 7500;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(186))
                {
                    grp_num[1].value[4]  = 9000;
                    grp_num[1].value[20] = 9000;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(224))
                {
                    grp_num[1].value[4]  = 1100;
                    grp_num[1].factor[4] = 0x0151;
                    grp_num[1].value[20] = 1100;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(253))
                {
                    grp_num[1].value[4]   = 1320;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 1320;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(304))
                {
                    grp_num[1].value[4]   = 1600;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 1600;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(340))
                {
                    grp_num[1].value[4]   = 1850;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 1850;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(377))
                {
                    grp_num[1].value[4]   = 2000;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 2000;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(430))
                {
                    grp_num[1].value[4]   = 2200;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 2200;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(510))
                {
                    grp_num[1].value[4]   = 2500;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 2500;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(590))
                {
                    grp_num[1].value[4]   = 3150;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 3150;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(675))
                {
                    grp_num[1].value[4]   = 3550;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 3550;
                    grp_num[1].factor[20] = 0x0151;
                }
                else if (MainIQ_Variable.I_Rate == _IQ17(740))
                {
                    grp_num[1].value[4]   = 4000;
                    grp_num[1].factor[4]  = 0x0151;
                    grp_num[1].value[20]  = 4000;
                    grp_num[1].factor[20] = 0x0151;
                }
            }
    }

//-----------------Rated power F105,F121------------------------------------------------
}

///////////////////////////////////////////////////////////////////////
void interrupt_timer(void)  //1ms
{
    static uint16_t cnt = 0;
    if (++cnt >= 1000)   // 10 kHz / 1000 = 10 Hz
    {
        cnt = 0;
        lcd_refresh_req = true;  // ����� main � refresh LCD
        if(GpioDataRegs.GPBDAT.bit.GPIO34)
        GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
        else
        GpioDataRegs.GPBSET.bit.GPIO34 = 1;

    }

    //uint16_t Buff_Prtsel;
    TImeOutHotKey++;
//----------------------------------------------------------------
//  Delay keypad waiting for hardreset
    if(KeyPad_reg.GENERAL_FLAG.bit.ConnectKeyPad==1)
    {
        KeyPad_reg.DelayKeypad++;
    }
//----------------------------------------------------------------
    if(KeyPad_reg.TRIGGER_BIT.bit.TimeOutE2p_Trg==1)//check time e2prom
    {
        if(TimeOutE2p>timeout_e2p)
        {
            TimeOutE2p = 0;
            KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error = 1;
        }else
        {
            TimeOutE2p++;
        }
        KeyPad_reg.TRIGGER_BIT.bit.TimeOutE2p_Trg = 0;
    }
//----------------------------------------------------------------
//      counter for enabling speed up cntr for up/down key
    if(Tmr_key_next_step > 0)
    {
        Tmr_key_next_step--;
    }
    else
    {
        KeyPad_reg.TRIGGER_BIT.bit.Speed_UpDwn_Trg = 0;
    }
//----------------------------------------------------------------
    KeyPad_reg.TimerBlinkLED++;
    if(KeyPad_reg.TimerBlinkLED >= TimeLEDBlnk)//timer for blinking LED on keypad
    {
        KeyPad_reg.TimerBlinkLED = 0;
        KeyPad_reg.DISP_FLAG.bit.GeneralBlk = ~KeyPad_reg.DISP_FLAG.bit.GeneralBlk;
    }
//----------------------------------------------------------------
    if(KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg==1)
    {
        if(Tmr_Norm_Set !=0)
        {
            Tmr_Norm_Set--;
        }
        else
        {
            KeyPad_reg.TRIGGER_BIT.bit.Norm_Set_Trg = 0;
        }
    }
    //-------------------Gus----------------------
    if(KeyPad_reg.TRIGGER_BIT.bit.Data_Set_Trg==1)
    {
        if(Tmr_Data_Set !=0)
        {
            Tmr_Data_Set--;
        }
        else
        {
            KeyPad_reg.TRIGGER_BIT.bit.Data_Set_Trg = 0;
        }
    }
    //-------------------Gus----------------------

    //-------------------Gus----------------------
    if(KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg==1)
    {
        if(Tmr_Mode_Set !=0)
        {
            Tmr_Mode_Set--;
        }
        else
        {
            KeyPad_reg.TRIGGER_BIT.bit.Mode_Set_Trg = 0;
        }
    }
    //-------------------Gus----------------------


//----------------------------------------------------------------
    if(Tmr_CNTR_UpDown1 > 0)
    {
        Tmr_CNTR_UpDown1--;
    }
    else
    {
        KeyPad_reg.TRIGGER_BIT.bit.RunCntr_Trg = 0;
    }
//----------------------------------------------------------------
    if(KeyPad_reg.TIMER0.bit.Time_blink>0)
    {
        KeyPad_reg.TIMER0.bit.Time_blink--;
        if(KeyPad_reg.TIMER0.bit.Time_blink==0)
        {
            if(KeyPad_reg.DISP_TAB.bit.Disp_Table!=0)
            {
                KeyPad_reg.DISP_TAB.bit.Disp_Table  = 0;
                KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg = 0;
            }
            else
            {
                KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg = ~KeyPad_reg.TRIGGER_BIT.bit.Blnk_Trg;
            }
        }
    }
    else
    {
        if(KeyPad_reg.MODE_FLAG.bit.setting_mode==0 && KeyPad_reg.MODE_FLAG.bit.norm_state==0)//normal mode and without setting
        {
            KeyPad_reg.TIMER0.bit.Time_blink = tm_blnk;
        }
        else
        {
            KeyPad_reg.TIMER0.bit.Time_blink = tm_blnk1;
        }
    }
//----------------------------------------------------------------
    if(KeyPad_reg.TRIGGER_BIT.bit.E2Prom_Trg==1)//delay restart writing 7ms
    {
        if(KeyPad_reg.TIMER0.bit.Time_WRE2Prom<delay_wrE2prom)
        {
            KeyPad_reg.TIMER0.bit.Time_WRE2Prom++;
        }
        else
        {
            KeyPad_reg.TRIGGER_BIT.bit.E2Prom_Trg = 0;
            KeyPad_reg.TIMER0.bit.Time_WRE2Prom   = 0;
            KeyPad_reg.E2PROM_FLAG1.bit.W_Status  = I2C_MSGSTAT_RESTART;
        }
    }
//----------------------------------------------------------------
}



//===========================================================================
// End of file.
//===========================================================================
