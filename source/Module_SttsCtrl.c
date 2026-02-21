//completed : PID status with start/stop/stopping/run
//revised : acc/dec when stopping
//completed : display status on PID
//revised : display acc/dec/stdy
//debugged : PUSP = 0
//debugged : PUOP = PUSP when first start to remove the step increase
//completed : display SP when stop mode
//debugged : PID_Reg.PU_SetpointK = PID_Reg.PU_OutputK while stopping
//revised : stopping with loop acc/dec outsite to avoid IQ converse effect
//revised : run/stop command bit from keypad
//revised : change status from start to run state
//16/5/2012 : revised : groupping stopping operation
//21/5/2012 : revised : no absolute when determind differrence SPsource and SPacc/dec
//2012-6-11 : -move PID section into PID this module only check and control status
//			  -clearing bit run/stop command after process check (&0x001F)
//2012-6-12 : revised checking acc/dec/stdy with timer50ms
//2012-6-15 : added initial ND2System_Reg
//2012-6-15 : completed : function select F.
//2012-6-16 : comlpeted : acc/dec output
//2012-6-16 : added check range outout
//2012-6-18 : completed determind Fskip max/med/min
//2012-6-18 : revised skip local variable as structure
//2012-6-18 : completed skip decel
//2012-6-18 : completed skip accel/steady
//2012-6-18 : completed skip function
//2012-6-18 : debugged skip function if no enter the process
//2012-6-19 : debugged skip function if hysteresis = 0 but not finish acc/dec s-curve
//2012-6-19 : got acc with total time
//2012-6-20 : not work acc s-curve 
//2012-6-21 : added output shutdown by bit fault
//2012-6-23 : revised seperate sub-rountine
//2012-6-23 : got ramp for acc/dec
//2012-6-25 : revised Freq_Reg.TcntrSCrv += _IQ17(0.000961);
//2012-6-25 : improved s-curve by /1000 at last data
//2012-6-26 : improved s-curve by /1000 at A cal. and IQ system are IQ30/IQ17
//2012-6-26 : revised accel s-curve
//2012-6-28 : just got acc-dec with real function
//2012-6-28 : completed s-curve at start&stop
//2012-6-29 : move cal.deltaF in s-curve
//2012-6-29 : completed s-curve
//2012-6-29 : revised reverse
//2012-6-29 : revised reverse with no return
//2012-6-29 : completed start/stop +,- Fcommand
//2012-6-29 : just return to 0 after change direction but not complete in the acc again
//2012-6-29 : just come change direction
//2012-6-29 : revise on and off operation with s-curve
//2012-7-2 : remove check acc/dec remain 1time
//2012-7-2 : revised deltaF variable to global variable
//2012-7-7 : added function DIN=0,1,2
//2012-7-7 : revised FCommMngr before Chk_Command because it must be update data before ACDC
//2012-9-7 : debugged load Fcommand=Ftarget if time ACDC=0
//2012-9-7 : completed command from terminal FWD/REV
//2012-9-7 : completed Jogging with command run and stop
//2012-9-7 : completed shutdown output from fault
//2012-9-7 : completed coast to stop function
//2012-9-7 : completed select ACDC by DIN
//2012-9-7 : added margin to avoid the fluctuation of A2D when ACDC
//2012-9-7 : revised bit check update for grp3 before load sp of ACDC
//2012-7-11 : debugged skip function
//2012-7-11 : revised diffrent Sp=0.001 and clear Cntr Scrv when reach the SP
//2012-7-11 : revised order state in FCommMngr
//2012-7-11 : added bit check update FSP"ND2System_Reg.STATE_FLAG.bit.UpdateCmd=0;"
//			  in keypad run command
//2012-7-12 : added FSET PID start/run as OutputPID stop/stopping as SptempPID
//2012-7-13 : stop operation with F516/F517
//2012-7-13 : completed start F514/F515
//2012-7-14 : remove checking 0 max,med,min in skip function
//2012-7-14 : debugged change starting F and holdtime after skip function
//2012-7-17 : revised Time counter = 0.095ms and add 0.000008 when counter =0
//2012-7-17 : revised Time counter ACDC as float32
//2012-7-18 : use compensate mid time "#define CompMidT"
//2012-7-18 : revised compensate s-curve at final time(5% of final time)
//2012-7-18 : completed compensate S-curve at % of final time
//2012-7-18 : debugged S-curve must has minimum ram _IQ17(0.000008)
//2012-7-23 : debugged runing dubble-times by clearing add function ClrDataStopping
//2012-7-24 : add compensate @ ramp midtime
//2012-7-24 : add two conditions for cal ramp in midtime
//2012-7-27 : no conversion IQtoF in argument of function in ACDC
//2012-7-27 : debugged increase resolution in determind data change or not from 0.001 to 0.0001
//			  in ACDC, skip
//2012-8-7 : intial ND2System_Reg
//2012-8-7 : completed display run command before ND2System_Reg.STATE_FLAG.bit.CmpltdRC 
//			 set to "1"
//2012-8-9 : add initial ND2System_Reg
//2012-8-10 : revised startingF with check condition Fcaommand>Fstart
//2012-8-16 : debugged starting function(no decel stop problem)
//2012-8-9 : add initial ND2System_Reg
//2012-8-18 : delay stage stop when DC breaking
//2012-8-30 : revised check command:bit BckUpSTT and operation of restarting the system after fault
//			  ,jog,coast
//2012-8-30 : revised s-curve with selectable with/without init/final
//2012-8-30 : revised clearing data stopping/stop by moving into CheckStateMachine
//2012-8-30 : completed reverse locking fnct(F423)
//2012-8-31 : revised compensate midtime s-curve
//2012-8-31 : revised compensate midtime s-curve
//2012-8-31 : revised compensate with decrease counter
//2012-8-31 : added check ND2System_Reg.TcntrSCrv with minus? when decrease
//2012-9-1 : revised clearing bit enablerunstop/run/stop
//2012-9-1 : debugged clear data stop when DC brake
//2012-9-5 : move clearing data stopping/stop to this module
//2012-9-10 : debugged no force BckUPSTT=0 in case LU restart
//2012-9-11 : added compensate S-curve both of ACC/DEC
//2012-10-1 : revised key run/stop active on single press
//2012-10-3 : revised time compensate counter s-curve0.09
//2012-10-3 : revised ramp in s-curve in the case of ramp=0
//2012-10-3 : add time compensate in final s-curve
//2012-10-3 : revise compensate final time by #define
//2012-10-13 : add clear variable Vref fly
//2012-10-24 : change control run command with ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready
//2012-10-24 : revised function check system
//2012-11-2 : revised check Fset->after update Fset the system will check the status immediately no wait delay 50sec again
//2012-11-7 : add function initial PU
//2012-11-7 : completed initial PU
//2012-11-19 : add set 5VOP after system was complted 
//2012-11-20 : move enabling 5V i/o to Fan&RCharge.C
//2012-11-21 : change gap targetACDC and FCommand
//2012-11-22 : add clear data stallI,V when shutdown op
//2012-11-27 : revised no reset bit "ShtdwnOPFault" untill clear fault
//2012-12-1 : stop running s-curve when stallI,V
//2012-12-1 : no stall when Flying,shutdwn op
//2012-12-28 : revised s-curve with float32 and without compensate
//2013-1-10 : different gap of Fsource
//2013-1-21 : debugged starting freq. use <= for checking counter
//2013-2-20 : debugged no gap for digital and target=0 when check new target s-curve
//2013-3-7 : reset bit FLAG_SYST_CTRL[Syst_ready] when ShtdwnOP = 1
//2013-4-1 : change bit "Syst_ready" to check command run/stop
//2013-4-2 : revise Tdec!=0 and Fcommand!=0,the system will acc normally(fly is no effect)
			 //,shutdwn op when stop with no finish fly
//2013-4-8 : debug re-load init-final s-curve when change the setpoint
//2013-4-8 : revise no checking dataaccdecchange/datachange before re-load init-final s-curve
//2013-4-23 : add bit check dirtion speed and delay stop status
//2013-4-24 : use bit shtdwnop to check system ready
//2013-5-2 : revise use "stopping condition" for checking lift stop
//2013-5-6 : use F727=1(Lift Auto Enable) checking the stop lift s-curve
//2013-5-6 : add condition re-cal s-curve with target >=1
//2013-5-15 : add checking bootstrap before normal run
//2013-5-21 : revise clear data stop when FCom=0
//2013-5-21 : remove clear data sto when DIN DC=1 in "MachineStateCtrl"
//2013-6-11 : add init F_base
//2013-6-13 : add stop command for stopping jog
//2013-6-18 : define MainIQ_Variable.Omega_base here
//2013-6-24 : add state start/stop autotune
//2013-7-2 : revise Fset status
//2013-7-2 : revise status machine
//2013-7-2 : revise status Fset
//2013-7-5 : remove dc break when check command
//2013-7-11 : add bit check auto stop of monocycle pattern,clear data pattern
//2013-7-19 : revise backup status of LU
//2013-8-7 : Add initial "Freq_rev"
//2013-9-2 : revise pustopF
//2013-9-2 : revise ACDC with output argument
//2013-9-3 : revise ACDC 
//2013-9-3 : revise status command/machine
//2013-9-3 : completed check machine state with torque cmmnd
//2013-9-4 : use bit "CmdType" check mode
//2013-11-11 : revise algorithm check status command
//2013-11-12 : revise algorithm check stop state first


#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct ND2GAINREG ND2Gain_Reg = {0}; 


struct ND2SYSTEM_REG ND2System_Reg = {0x0002,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0,0,0,
                                      0,0,0};
//---External variable import--------------------------------------
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];  
extern int Flag_DCbreak;
extern int First_Test_ND2;
extern char Flag_stall_V_Active;
extern long I_fullload;
extern long I_fullload;
extern _iq17 Frate;
extern _iq17 Vrate;
extern long I_noload;
extern int flag_PWMEnable;
extern long Freq_rev;
extern long Slip_rate;

Uint16 RamTestdelaystop=0;
/////////////////////////////////////////////////////////////////////////////////
void Check_Status(void)
{
    ChckSyst();     //Check system is ready or not?
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
    {
        Chk_Command();      //check RunCommand/StopCommand which one is actived?
        MchnStatus();       //state control : stop,stopping,start,run
        FCommMngr();        //Select FSET(PID/Normal),ACCT,DECT->Skip->Limit->Acc/Dec
        TCommMngr();        //Torqe command manager
        CommandStatus();    //Check command status(Freq,Torqe) : ready,accel,decel,steady
        InitDataDrv();      //initial data for driving
    }
}
/////////////////////////////////////////////////////////////////////////////////
void InitDataDrv(void)
{
_iq buff1=0;

    if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)//OFF PWM setting
    {
        if(grp_num[5].value[0] == 4 && grp_num[5].value[33]==1)
        {
            ND2System_Reg.MAIN_FLAG2.bit.CmdType=1;//Torque command     //1
        }else
        {
            ND2System_Reg.MAIN_FLAG2.bit.CmdType=0;//Freq. command
        }
        MainIQ_Variable.Omega_base = _IQ17(314.1592654);//Omega=2*Pi*F;F=50Hz
        //--I-------------------------------------------------------------------------
        MainIQ_Variable.IPU_Motor = _IQ17div(CurrentMotor->I_Motor,MainIQ_Variable.I_Rate);
        I_fullload = MainIQ_Variable.IPU_Motor;//_IQ17div(CurrentMotor->I_Motor,_IQ17(20.00))   ;//  1PU = 20A
        MainIQ_Variable.I_base = MainIQ_Variable.I_Rate;//Rate current inverter
        MainIQ_Variable.IPU_NoLoad = _IQ17div(CurrentMotor->I_NL_Motor,MainIQ_Variable.I_Rate);
        I_noload   = MainIQ_Variable.IPU_NoLoad;//_IQ17div(CurrentMotor->I_NL_Motor,_IQ17(20.00));//  1PU = 20A

        //--F & Speed-------------------------------------------------------------------------
        MainIQ_Variable.FPU_Motor = _IQ17div(CurrentMotor->F_Motor,MainIQ_Variable.FRef);
        Frate = _IQ17div(CurrentMotor->F_Motor,MainIQ_Variable.FRef);
        MainIQ_Variable.F_base = MainIQ_Variable.FRef;
        MainIQ_Variable.FSlip = _IQ17div(_IQ17(2),MainIQ_Variable.FRef);
        MainIQ_Variable.FreqCut_PU = _IQ17div(MainIQ_Variable.FreqCut,MainIQ_Variable.F_base);
        Freq_rev = _IQ17div(_IQ17(50),MainIQ_Variable.F_base);
        /*
                             Pole*RPM
            Slip rate = 1 - (--------)
                             120*Freq
        */
        buff1 = _IQ17mpy(CurrentMotor->Poles_Motor,CurrentMotor->Speed_Motor);
        buff1 = _IQ17div(buff1,CurrentMotor->F_Motor);
        buff1 = _IQ17div(buff1,_IQ17(120));
        buff1 = IQ17_1-buff1;
        Slip_rate = ((long)328 * buff1)>>17;
        //--V-------------------------------------------------------------------------
        if(grp_num[0].value[4]==1)
        {//for 400V model
            MainIQ_Variable.VPU_Motor =  _IQ17div(CurrentMotor->V_Motor,VRate400);
            MainIQ_Variable.V_base = VDCmax1;
        }else
        {//for 200V model
            MainIQ_Variable.VPU_Motor =  _IQ17div(CurrentMotor->V_Motor,VRate200);
            MainIQ_Variable.V_base = VDCmax0;
        }
        Vrate = _IQ17div(CurrentMotor->V_Motor,MainIQ_Variable.V_base);

        SetupPRControl();

        MainIQ_Variable.I_base_PER_Vbase = _IQ17div(MainIQ_Variable.I_Rate,MainIQ_Variable.V_base);







        //--Complete-------------------------------------------------------------------------
        ND2System_Reg.STATE_FLAG.bit.InitPU = 1;    //bit check for complete initial
    }
}
/////////////////////////////////////////////////////////////////////////////////
void ChckSyst(void)
{
Uint16 Chkbit=0;

        Chkbit = Chkbit|ND2System_Reg.STATE_FLAG.bit.CmpltdRC;
        Chkbit = Chkbit<<1;
        Chkbit = Chkbit|IV_Read_reg.IV_FLAG.bit.OffsetA2DI;
        Chkbit = Chkbit<<1;
        Chkbit = Chkbit|KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd;
        Chkbit = Chkbit<<1;
        Chkbit = Chkbit|ND2System_Reg.STATE_FLAG.bit.InitPU;
        Chkbit = Chkbit<<1;
        if(Bckup_reg.BCKUP_FLAG.bit.UpldSt==0 && Bckup_reg.BCKUP_FLAG.bit.DnldSt==0)
        {
            Chkbit = Chkbit| 0x01;
        }
        Chkbit = Chkbit<<1;
        if(ChkFault_Reg.FLAG_FAULT.bit.ShtdwnOPFault==0)
        {
            Chkbit = Chkbit|0x01;
        }
        Chkbit = Chkbit<<1;
      //  Chkbit = Chkbit|QEP_Reg.GENERAL_QEPFLAG.bit.FrstChkDirt;
        Chkbit = Chkbit|1;
        if(Chkbit==0x007F)
        {
            ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready=1;
        }else
        {
            ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready=0;
        }
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_Command(void)
{
/*
#define state_stop          1
#define state_start         2
#define state_startAT       3//state start autotune
#define state_run           4
#define state_stopping      8
#define state_stoppingAT    9//state stopping autotune
*/
    if(ChkFault_Reg.FLAG_FAULT.bit.ShtdwnOPFault == 1)
    {
        //case of shutdown op from fault
        ND2System_Reg.STATE_FLAG.bit.BckUpSTT = (ChkFault_Reg.DATA_FAULT.bit.Fault_LU==1
                                                    && grp_num[4].value[21]>=2)?ND2System_Reg.STATE_FLAG.bit.BckUpSTT:0;
        ND2System_Reg.STATE_FLAG.bit.ShtdwnOP = 1;  //bit shutdown output
        if(ND2System_Reg.STATE_FLAG.bit.State == state_start ||
           ND2System_Reg.STATE_FLAG.bit.State == state_run)
        {
            ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
        }
    }else
    {
        if(Flag_DCbreak==1)
        {
            ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
        }else if(DgtlPrt_Reg.DataLDIN.bit.Coast_stop==1)
        {
            ND2System_Reg.STATE_FLAG.bit.ShtdwnOP = 1;
            ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
        }else if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault ==0 && ChkFault_Reg.FLAG_FAULT.bit.LULockState ==0)
        {
            ND2System_Reg.STATE_FLAG.bit.ShtdwnOP = 0;
            if((DgtlPrt_Reg.FLAG_DGTLPRT.bit.EnbDINDrvEnb==1)&&
               (DgtlPrt_Reg.DataLDIN.bit.Drv_En==Inactive))
            {//case of disable driving
                ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
            }else if(Freq_Reg.FLAG_FREQ.bit.Jogging==2)
            {//case of run jogging
                if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready == 1)
                {
                    ND2System_Reg.STATE_FLAG.bit.RunCommand = 1;
                }
            }else if(Freq_Reg.FLAG_FREQ.bit.Jogging==1)
            {
                ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
            }else
            {//case of normal operation
                if(grp_num[2].value[0]==0)
                {//case of run/stop from keypad
                    if(KeyPad_reg.KEY_FLAG.bit.key_run==1)
                    {
                        if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready == 1)
                        {
                            ND2System_Reg.STATE_FLAG.bit.RunCommand = 1;
                            ND2System_Reg.STATE_FLAG.bit.UpdateCmd = 0;

                            if(grp_num[5].value[31] == 0)
                                ND2System_Reg.STATE_FLAG.bit.BckUpSTT = 1;
                        }
                    }else if(KeyPad_reg.KEY_FLAG.bit.key_stop==1||
                             Freq_Reg.FLAG_FREQ.bit.AutoStopPttn==1)
                    {
                        ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
                        ND2System_Reg.STATE_FLAG.bit.BckUpSTT = 0;
                    }else
                    {   //case of restart from stop free run
                        if(ND2System_Reg.STATE_FLAG.bit.BckUpSTT==1)
                        {
                            if(ND2System_Reg.STATE_FLAG.bit.State == state_stop ||
                               ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
                            {
                                ND2System_Reg.STATE_FLAG.bit.RunCommand = 1;
                                ND2System_Reg.STATE_FLAG.bit.UpdateCmd = 0;
                            }
                        }else
                        {
                            if(ND2System_Reg.STATE_FLAG.bit.State == state_start ||
                               ND2System_Reg.STATE_FLAG.bit.State == state_run)
                            {
                                ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
                            }
                        }
                    }
                }else
                {//case use terminal command
                    if(DgtlPrt_Reg.DataLDIN.bit.Fwd_Com==DgtlPrt_Reg.DataLDIN.bit.Rev_Com)
                    {
                        if(DgtlPrt_Reg.DataHDIN.bit.Wire_stp==Inactive)
                        {
                            ND2System_Reg.STATE_FLAG.bit.StopCommand = 1;
                            ND2System_Reg.STATE_FLAG.bit.BckUpSTT = 0;  //clear backup command from keypad
                        }
                    }else
                    {
                        if(ND2System_Reg.STATE_FLAG.bit.State == state_stop ||
                            ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
                        {
                            if(ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready == 1)
                            {
                                ND2System_Reg.STATE_FLAG.bit.RunCommand = 1;
                                ND2System_Reg.STATE_FLAG.bit.BckUpSTT = 1;  //clear backup command from keypad
                                ND2System_Reg.STATE_FLAG.bit.UpdateCmd = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    KeyPad_reg.KEY_FLAG.bit.key_run = 0;
    KeyPad_reg.KEY_FLAG.bit.key_stop = 0;
}
///////////////////////////////////////////////////////////////////////////////// 
void FCommMngr(void)
{
_iq target_output=0;
_iq buffmainacc=0;
_iq buffmaindec=0;
_iq *pntracc = &Freq_Reg.AccT1;
_iq *pntrdec = &Freq_Reg.DecT1;
Uint16 ACDCComm=0;
_iq buff=0;
_iq pustopF = 0;//per unit stop freq.
_iq pustartF = _IQ17div(ND2System_Reg.StartingF,MainIQ_Variable.FRef);//per unit start freq.

struct {
    Uint16 DisableACDC:1;       //bit indicate no need ACDC
    Uint16 SelectInitFinal:1;   //selector init,final S-curve
    Uint16 DisableStall:1;      //disable stallI,V
    Uint16 declift:1;           //buffer use decel lift
    Uint16 run_normal:1;        //run with normal
    Uint16 disableFCommMngr:1;  //disable this module
}flagFCommMngr={0};

    if(ND2System_Reg.STATE_FLAG.bit.UpdateCmd==0 ||
        ND2System_Reg.MAIN_FLAG2.bit.CmdType==1)
    {
        flagFCommMngr.disableFCommMngr=1;
    }
//------------------------------------------------------------------------------------
    if(flagFCommMngr.disableFCommMngr==0)
    {
        if(grp_num[7].value[0]==0)  //No PID function
        {
        //---Load data in regular Fsource(Fset,Fout,AccT,DecT)
            MainIQ_Variable.FSet = Freq_Reg.Freq_SpPU;//no computing in process so in=out
            target_output = Freq_Reg.Freq_SpPU;
            if(Freq_Reg.FLAG_FREQ.bit.Jogging==1 || DgtlPrt_Reg.FLAG_DGTLPRT.bit.EnbDINACDC==0)
            {
                buffmainacc = Freq_Reg.MainAcc;
                if(grp_num[0].value[21]==1 && (ND2System_Reg.STATE_FLAG.bit.State == state_stopping))//if(grp_num[Vctr_mode].value[0]!=0 && (DgtlPrt_Reg.DataLDIN.all & 0x3C00)==0)
                {//case of lift function and stopping
                    buffmaindec = Lift_Reg.LiftDec;
                    flagFCommMngr.declift = 1;
                }else
                {
                    buffmaindec = Freq_Reg.MainDec;
                }
            }else
            {
                ACDCComm = DgtlPrt_Reg.DataHDIN.bit.AccDecTB0;
                ACDCComm = ACDCComm+(DgtlPrt_Reg.DataHDIN.bit.AccDecTB1*2);
                buffmainacc = *(pntracc+ACDCComm*2);
                buffmaindec = *(pntrdec+ACDCComm*2);
            }
        }else
        {
        //---PID function
            if(ND2System_Reg.STATE_FLAG.bit.State == state_run ||
                ND2System_Reg.STATE_FLAG.bit.State == state_start)
            {
                MainIQ_Variable.FSet = PID_Reg.PU_OutputK;
            }else
            {
                MainIQ_Variable.FSet = PID_Reg.PID_SpTemp;
            }

            target_output = PID_Reg.PU_OutputK;
            buffmainacc = Freq_Reg.AccT1;
            buffmaindec = Freq_Reg.DecT1;
        }
        Lift_Reg.FLAG_LIFT.bit.StopLift = flagFCommMngr.declift;
        //----------------------------------------------
        //REV Locking
        if(grp_num[4].value[26]==1)
        {
            target_output = (target_output<0)?0:target_output;
        }
        //----------------------------------------------
        if(ND2System_Reg.STATE_FLAG.bit.ShtdwnOP==1)
        {//Shutdown section--------------------------
            goto ShtdwnFComm;
        }else if(Flag_DCbreak==1)
        {
            MainIQ_Variable.FCommandA = 0;
            goto DisableA1;
        }else if(ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
        {//Stopping section--------------------------
            if(grp_num[5].value[27]==1)//stopping mode is free run
            {
                goto ShtdwnFComm;
            }else
            {
                //Check stop freq. from DC brake/stop freq.
                pustopF = ND2System_Reg.StopF;
                if(grp_num[5].value[22]!=0)
                {
                    if(ND2System_Reg.DCBreak_Start_Frq>=pustopF)
                    {
                        pustopF = ND2System_Reg.DCBreak_Start_Frq;
                    }
                }
                //----------------------------------------------------
                pustopF = _IQ17div(pustopF,MainIQ_Variable.FRef);
                if(ND2System_Reg.TcntrStartingT<ND2System_Reg.StartingT || FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive==1)
                {
                    goto ShtdwnFComm;
                }else if(_IQ17abs(MainIQ_Variable.FCommandA)>pustopF)
                {
                    buff = pustopF;     //PU stop freq.
                    target_output = (MainIQ_Variable.FCommandA<0)?_IQ17mpy(buff,MinusIQ):buff;
                }else //case of decel FCommand reach the stop freq.
                {
ShtdwnFComm:        ND2System_Reg.STATE_FLAG.bit.ShtdwnOP=1;
                    if(flag_PWMEnable == 2)//check off PWM(=2)
                    {
                        MainIQ_Variable.FCommandA = 0;  //off PWM before F=0
                    }
                    goto DisableA1;
                }
            }
        }else if(ND2System_Reg.STATE_FLAG.bit.State==state_start||
                 ND2System_Reg.STATE_FLAG.bit.State==state_run)
        {
            if(BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp==1)
            {   //Flying-Start function
                FlyingStart_Function(target_output);
                if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive == 0)
                {
                    //Check LU
                    if(ChkFault_Reg.FLAG_FAULT.bit.LULockState == 1)
                    {
                        flagFCommMngr.SelectInitFinal = 1;
                        buffmaindec = ChkFault_Reg.DecLU;
                        target_output = 0;
                    }else
                    {
                        //Skip function
                        if(((target_output & 0x80000000)==(MainIQ_Variable.FCommandA & 0x80000000))
                            || (MainIQ_Variable.FCommandA==0))//only sign same could be processed
                        {
                            if(MainIQ_Variable.HystrsJumpF!=0)
                            {
                                buff = _IQ17abs(target_output-ND2System_Reg.TargetSkip);//for avoid fluctuation of A2D
                                if(buff>=_IQ17(0.0001))
                                {
                                    ND2System_Reg.TargetSkip = target_output;
                                    ND2System_Reg.OutputSkip = _IQ17(SkipF(_IQ17toF(target_output)));
                                }
                                target_output = ND2System_Reg.OutputSkip;
                            }else
                            {
                                ND2System_Reg.TargetSkip = 0;
                            }
                        }
                        //Starting Freq. and holdtime function
                        if(ND2System_Reg.TcntrStartingT<=ND2System_Reg.StartingT)
                        {
                            if(_IQ17abs(MainIQ_Variable.FCommandA) <= pustartF)
                            {
                                ND2System_Reg.TcntrStartingT+=_IQ17(0.001);
                                if(_IQ17abs(target_output)>=pustartF)
                                {
                                    MainIQ_Variable.FCommandA = (target_output<0)?_IQ17mpy(pustartF,MinusIQ):pustartF;//load starting F to Fcaommand immediately
                                }

                                flagFCommMngr.DisableACDC  = 1;
                            }
                            else
                            {
                                ND2System_Reg.TcntrStartingT=ND2System_Reg.StartingT;
                            }
                        }
                        //Check range of Output
                        target_output = ChkRngF(target_output,MainIQ_Variable.PU_FMax,MainIQ_Variable.PU_FMin);//check range of sp
                    }
                }
            }
        }
        else if(ND2System_Reg.STATE_FLAG.bit.State==state_stop)
        {
            DgtlPrt_Reg.FLAG_DGTLPRT.bit.FirstCheck = 0;//Checked bit of Flying start in the first time is cleared.
        }
        if(BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp==0
                ||ND2System_Reg.STATE_FLAG.bit.State==state_stop
                ||FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive == 1
                ||AutoTuneCtrlRegs.FLAGALLPARA.bit.EnableAT==1)
        {
DisableA1:
            flagFCommMngr.DisableACDC  = 1;
            flagFCommMngr.DisableStall = 1;
        }
//-----------------------------------------------------------------
//Check stall I,stall V no process ACDCSCrv
        if((Flag_stall_V_Active!=0)||(ND2System_Reg.FLAG_STALL.bit.StallIActive!=0))
        {
            flagFCommMngr.DisableACDC = 1;
        }
//-----------------------------------------------------------------
//Accel/Decel output
        if(flagFCommMngr.DisableACDC == 0)
        {
            MainIQ_Variable.FCommandA = ACDCSCrv(target_output,buffmainacc,buffmaindec,flagFCommMngr.SelectInitFinal
                                                 ,MainIQ_Variable.FCommandA,MainIQ_Variable.PU_FMax);//accel/decel s-curve currentF,target
        }
//Over-Current Stall
        if(flagFCommMngr.DisableStall==0)
        {
            CurrentStall(); //MainIQ_Variable.FCommandA->MainIQ_Variable.FCommandB
            VoltageStall(); //MainIQ_Variable.FCommandB->MainIQ_Variable.FCommand
        }else
        {
            MainIQ_Variable.FCommand  = MainIQ_Variable.FCommandA;
            MainIQ_Variable.FCommandB = MainIQ_Variable.FCommandA;
            ClrDataStallI();
            ClrDataStallV();
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
void ClrDataStop(void)
{
//S-Curve
    ND2System_Reg.TargetACDC = 0;
    ND2System_Reg.TcntrSCrv = 0;
//StartingTime
    ND2System_Reg.TcntrStartingT = 0;
//PID
    PID_Reg.PID_Dyna_Max = 0;//clear dynamic limit after complete stop
    PID_Reg.PID_Dyna_Min = 0;
//Flying Start Data
    FlyStrt_Reg.Flag_FlyStrt.bit.LockFly = FlyStrt_Reg.Flag_FlyStrt.bit.LockFly & 0xC;
    FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive = 0; //no vout step up
//Jogging
    Freq_Reg.FLAG_FREQ.bit.Jogging = 0;
} 
/////////////////////////////////////////////////////////////////////////////////
void ClrDataStopping(void)
{
    RamTestdelaystop = 0;
//Skip Data
    ND2System_Reg.TargetSkip = 0;
    ND2System_Reg.OutputSkip = 0;
//ACDC Data
    ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero = 0;
//PID
    PID_Reg.FLAG_PID.all = 0;
    PID_Reg.PU_FeedbackK_1 = 0;
    PID_Reg.PU_ErrorK = 0;
    PID_Reg.PU_ErrorK_1 = 0;
    PID_Reg.I_termK_1 = 0;
    PID_Reg.PU_SatErK = 0;
    PID_Reg.PU_OutputPreBuff = 0;
    PID_Reg.PU_OutputK = 0;
//Flying Start Data
    FlyStrt_Reg.TDelayVout = 0;
    FlyStrt_Reg.Flag_FlyStrt.bit.FlyStage = 0;
    FlyStrt_Reg.Flag_FlyStrt.bit.StepDwnFout = 0;
    FlyStrt_Reg.Flag_FlyStrt.bit.IDecrease = 0;
    FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_AccVop = TsCntr30;
    FlyStrt_Reg.FlyStrt_Timer.bit.T_Smp_DecFop = 0;
    FlyStrt_Reg.VOutputSearch = 0;
    FlyStrt_Reg.FFly_K_1 = 0;
    FlyStrt_Reg.IFly_K_1 = 0;
    FlyStrt_Reg.VReffSrch = 0;
//State flag
    ND2System_Reg.STATE_FLAG.bit.ChkACDC = 0;
//Pattern
   Freq_Reg.TIMER_FREQ.bit.T_Smpstg = 0;
   Freq_Reg.TCtrlPttn = 0;
   Freq_Reg.FLAG_FREQ.bit.AutoStopPttn=0;
   if(grp_num[3].value[53]==0 ||
      Freq_Reg.FLAG_FREQ.bit.FinishMonoCyc == 1)
   {
         Freq_Reg.FLAG_FREQ.bit.CtrlPttnStg = 0;
         Freq_Reg.FLAG_FREQ.bit.FinishMonoCyc = 0;
   }
}
/////////////////////////////////////////////////////////////////////////////////
_iq ACDCSCrv(_iq target,_iq mainacc,_iq maindec,Uint16 slctrinitfinal,_iq current,_iq base)
{
_iq PreMidT=0;
_iq buff=0;
Uint16 buffdir=0;  
float32 buffflt=0,buffinitt=0,bufffinalt=0,ramp=0,crrntflt=_IQ17toF(_IQ17abs(current));
_iq GapTarget=0;
 
    //check parameters for calculation, if there were changed will be re-calculated
    if(target!=ND2System_Reg.TargetACDC)
    {
        buff = _IQ17abs(target-ND2System_Reg.TargetACDC);//for avoid fluctuation of A2D
        GapTarget = (KeyPad_reg.GENERAL_FLAG.bit.Analog_Src == 1)?GapAnalog:Gapdigital;
        if(buff>=GapTarget||target==0||target>=IQ17_1)//check gap
        {
            ND2System_Reg.FLAG_SYST_CTRL.bit.DataChnged=1;
            ND2System_Reg.TargetACDC = target;
        }
    }
    if(base!=ND2System_Reg.PUBaseACDC)
    {
        ND2System_Reg.FLAG_SYST_CTRL.bit.DataChnged=1;
        ND2System_Reg.PUBaseACDC = base;
    }
    //--------------------------------------------
    if(ND2System_Reg.FLAG_SYST_CTRL.bit.DataChnged==0)
    {
        if((ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero == 1) && (current==0))
        {
            ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero = 0;
            ND2System_Reg.FLAG_SYST_CTRL.bit.DataChnged=1;
            goto accfrom0;
        }else
        {
            goto chckaccdec;
        }
    }else
    {
accfrom0:

        if((ND2System_Reg.TargetACDC & 0x80000000)!=(current & 0x80000000))
        {   //case different sign
            if(current==0)
            {
                ND2System_Reg.DeltaF = _IQ17abs(ND2System_Reg.TargetACDC);
                ND2System_Reg.FLAG_SYST_CTRL.bit.ACDCCmd = 0;
            }else
            {
                ND2System_Reg.DeltaF = _IQ17abs(current);
                ND2System_Reg.FLAG_SYST_CTRL.bit.ACDCCmd = 1;
                if(ND2System_Reg.STATE_FLAG.bit.State != state_stopping)
                {
                    ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero = 1;
                }
            }
        }else
        {   //case same sign
            if(_IQ17abs(ND2System_Reg.TargetACDC)<=_IQ17abs(current))
            {
                ND2System_Reg.DeltaF = _IQ17abs(current)-_IQ17abs(ND2System_Reg.TargetACDC);
                ND2System_Reg.FLAG_SYST_CTRL.bit.ACDCCmd = 1;
            }else
            {
                ND2System_Reg.DeltaF = _IQ17abs(ND2System_Reg.TargetACDC)-_IQ17abs(current);
                ND2System_Reg.FLAG_SYST_CTRL.bit.ACDCCmd = 0;
            }
        }
chckaccdec:
        if(ND2System_Reg.FLAG_SYST_CTRL.bit.ACDCCmd == 1)
        {//system decel
            buffdir = (current<0)?1:0;//check direction currentF if-,buffdir=1
            //---check data changed or not?
            if(maindec!=ND2System_Reg.MainDecACDC)
            {
                ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged=1;
                ND2System_Reg.MainDecACDC = maindec;
            }
            if(Lift_Reg.FLAG_LIFT.bit.StopLift==0)
            {
                if(Freq_Reg.InitDec!=ND2System_Reg.InitDecACDC)
                {
                    ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged=1;
                    ND2System_Reg.InitDecACDC = Freq_Reg.InitDec;
                }
                if(Freq_Reg.FinalDec!=ND2System_Reg.FinalDecACDC)
                {
                    ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged=1;
                    ND2System_Reg.FinalDecACDC = Freq_Reg.FinalDec;
                }
            }else
            {//case stop lift
                if(Lift_Reg.InitLiftDec!=ND2System_Reg.InitDecACDC)
                {
                    ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged=1;
                    ND2System_Reg.InitDecACDC = Lift_Reg.InitLiftDec;
                }
                if(Lift_Reg.FinalLiftDec!=ND2System_Reg.FinalDecACDC)
                {
                    ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged=1;
                    ND2System_Reg.FinalDecACDC = Lift_Reg.FinalLiftDec;
                }
            }
            //---re-cal.
            PreMidT = ND2System_Reg.MainDecACDC;
            ND2System_Reg.InitTACDC = ND2System_Reg.InitDecACDC;
            ND2System_Reg.FinalTACDC = ND2System_Reg.FinalDecACDC;
        }else
        {//system accel
            buffdir = (ND2System_Reg.TargetACDC<0)?1:0;//check direction target if-,buffdir=1
            //check data changed or not?
            if(mainacc!=ND2System_Reg.MainAccACDC)
            {
                ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged = 1;
                ND2System_Reg.MainAccACDC = mainacc;
            }
            if(Freq_Reg.InitAcc!=ND2System_Reg.InitAccACDC)
            {
                ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged = 1;
                ND2System_Reg.InitAccACDC = Freq_Reg.InitAcc;
            }
            if(Freq_Reg.FinalAcc!=ND2System_Reg.FinalAccACDC)
            {
                ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged = 1;
                ND2System_Reg.FinalAccACDC = Freq_Reg.FinalAcc;
            }
           //---re-cal.
           PreMidT = ND2System_Reg.MainAccACDC;
           ND2System_Reg.InitTACDC  = ND2System_Reg.InitAccACDC;
           ND2System_Reg.FinalTACDC = ND2System_Reg.FinalAccACDC;
        }
    }
//----------------------------------------------------------
//Cal. Section MidTime,T-Scurve,A-Scurve 
    if(slctrinitfinal==0)//case of slctrinitfinal = 1 as LU(2),torque decel without s-curve
    {
        buffinitt = _IQ17toF(ND2System_Reg.InitTACDC);
        bufffinalt = _IQ17toF(ND2System_Reg.FinalTACDC);
    }
    if(ND2System_Reg.FLAG_SYST_CTRL.bit.DataChnged==1 ||
       ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged==1)
    {//All of case of data of all data changed
        ND2System_Reg.FLAG_SYST_CTRL.bit.DataChnged     = 0;
        ND2System_Reg.FLAG_SYST_CTRL.bit.DataACDCChnged = 0;

        ND2System_Reg.TcntrSCrv  = 0;//clear counter s-curve
        ND2System_Reg.F_CMDA_X_1 = crrntflt;

        ND2System_Reg.FLAG_SYST_CTRL.bit.CmpltdACDC = 0;

        //MidTime = (MainTime*deltaF)/PU FMAX
        buff = _IQ17mpy(PreMidT,ND2System_Reg.DeltaF);
        buff = _IQ17div(buff,ND2System_Reg.PUBaseACDC);
        ND2System_Reg.MidTACDC = _IQ17toF(buff);

        //Time S-curve = [(InitTime+Final)/2]+MidTime
        buff = ND2System_Reg.InitTACDC+ND2System_Reg.FinalTACDC;
        buff = buff>>1;
        ND2System_Reg.SCrvTACDC = _IQ17toF(buff)+ND2System_Reg.MidTACDC;

        //A-Scurve = deltaF/Time S-curve
        ND2System_Reg.A_SCurve = 0;
        if(ND2System_Reg.SCrvTACDC!=0)
        {
            buffflt = _IQ17toF(ND2System_Reg.DeltaF);
            buffflt = buffflt/ND2System_Reg.SCrvTACDC;
            ND2System_Reg.A_SCurve = buffflt/1000;
        }
    }
//----------------------------------------------------------
//Ramp Control
    if(ND2System_Reg.A_SCurve!=0)
    {
        if(ND2System_Reg.FLAG_SYST_CTRL.bit.CmpltdACDC == 0)
        {
            ND2System_Reg.TcntrSCrv+=0.001;
            if(ND2System_Reg.TcntrSCrv<buffinitt)
            {//ramp = (A*Cntr)/Tinit
                ramp = ND2System_Reg.A_SCurve*ND2System_Reg.TcntrSCrv;
                ramp = ramp/buffinitt;
            }else if(ND2System_Reg.TcntrSCrv<buffinitt+ND2System_Reg.MidTACDC)
            {//ramp = A
                ramp = ND2System_Reg.A_SCurve;
            }else
            {//ramp = A*(Tinit+MidT+FinalT-Cntr)/FinalT
                if(bufffinalt!=0)
                {
                    ramp = buffinitt+ND2System_Reg.MidTACDC+bufffinalt-ND2System_Reg.TcntrSCrv;
                    ramp = (ramp*ND2System_Reg.A_SCurve)/bufffinalt;
                    ramp = (ramp<=0)?0.000001:ramp;
                }else
                {
                    ramp = ND2System_Reg.A_SCurve;
                }
            }
//add ramp
            if(ND2System_Reg.FLAG_SYST_CTRL.bit.ACDCCmd == 1)
            {
                ramp = ramp*(-1);
                ND2System_Reg.F_CMDA_X_1 = ND2System_Reg.F_CMDA_X_1+ramp;
                buff = _IQ17(ND2System_Reg.F_CMDA_X_1);
                if(ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero==1)
                {
                    if(buff<=0)//DEC reach to 0Hz?
                    {
                        ND2System_Reg.F_CMDA_X_1 = 0;
                    }
                }else if(buff<=_IQ17abs(ND2System_Reg.TargetACDC))
                {
                    ND2System_Reg.F_CMDA_X_1 = _IQ17toF(_IQ17abs(ND2System_Reg.TargetACDC));
                    ND2System_Reg.FLAG_SYST_CTRL.bit.CmpltdACDC = 1;
                }
            }else
            {
                ND2System_Reg.F_CMDA_X_1 = ND2System_Reg.F_CMDA_X_1+ramp;
                buff = _IQ17(ND2System_Reg.F_CMDA_X_1);
                if(buff>=_IQ17abs(ND2System_Reg.TargetACDC))
                {
                    ND2System_Reg.F_CMDA_X_1 = _IQ17toF(_IQ17abs(ND2System_Reg.TargetACDC));
                    ND2System_Reg.FLAG_SYST_CTRL.bit.CmpltdACDC = 1;
                }
            }
            current = _IQ17(ND2System_Reg.F_CMDA_X_1);
            current = (buffdir==1)?_IQ17mpy(current,MinusIQ):current;
        }
    }else
    {
        current = ND2System_Reg.TargetACDC;
        ND2System_Reg.TcntrSCrv = 0;
    }
    return current;
}  
/////////////////////////////////////////////////////////////////////////////////
void CommandStatus(void)
{
_iq buff_data =0;
_iq OpCmnd=0;
//_iq SpCmnd=0;
/*-->STATE_FLAG[State]<--
#define state_stop          1
#define state_start         2
#define state_startAT       3//state start autotune
#define state_run           4
#define state_stopping      8
#define state_stoppingAT    9//state stopping autotune

-->MAIN_FLAG2[StatusCmnd]<--
#define SystemReady     3
#define SystemAccel     0
#define SystemDecel     1
#define SystemSteady    2
*/
    if(ND2System_Reg.MAIN_FLAG2.bit.CmdType==0)
    {
        OpCmnd = MainIQ_Variable.FCommand;
    }else
    {
        OpCmnd = TrqCmnd_Reg.TrqCommand;
    }
    if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
    {
        ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd = SystemReady;
    }else if(ND2System_Reg.STATE_FLAG.bit.UpdateCmd==1)
    {
        if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive == 1||
           Flag_stall_V_Active!=0||
           ND2System_Reg.FLAG_STALL.bit.StallIActive==1)
        {
            buff_data = _IQ17abs(OpCmnd) - ND2System_Reg.BckupCommand;
            goto ChkGap;
        }else if(ND2System_Reg.FLAG_SYST_CTRL.bit.DecZero==1||
                 ND2System_Reg.STATE_FLAG.bit.State == state_stopping)
        {
            ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd = SystemDecel;
        }else
        {
            buff_data = _IQ17abs(ND2System_Reg.TargetACDC)-_IQ17abs(OpCmnd);
        ChkGap:
            if(buff_data>0)
            {
                ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd = SystemAccel;
            }else if(buff_data<0)
            {
                ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd = SystemDecel;
            }else
            {
                ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd = SystemSteady;
            }
        }
    }
    ND2System_Reg.BckupCommand = _IQ17abs(OpCmnd);
}
/////////////////////////////////////////////////////////////////////////////////
void MchnStatus(void)
{
_iq buff_data =0;
_iq OpCmnd=0,SpCmnd=0;
/*-->STATE_FLAG[State]<--
#define state_stop          1
#define state_start         2
#define state_startAT       3//state start autotune
#define state_run           4
#define state_stopping      8
#define state_stoppingAT    9//state stopping autotune
*/


    if(ND2System_Reg.MAIN_FLAG2.bit.CmdType==0)     //freq command
    {
        OpCmnd = MainIQ_Variable.FCommand;
        SpCmnd = MainIQ_Variable.FSet;
    }else
    {
        OpCmnd = TrqCmnd_Reg.TrqCommand;
        SpCmnd = TrqCmnd_Reg.TrqSp;
    }

    switch(ND2System_Reg.STATE_FLAG.bit.State)
        {
            case state_stop://system is in stop state and wait to start/autotune
                if(ND2System_Reg.STATE_FLAG.bit.RunCommand == 1)
                {
                    if(AutoTuneCtrlRegs.FLAGALLPARA.bit.EnableAT ==1)
                    {
                        ND2System_Reg.STATE_FLAG.bit.State = state_startAT;
                    }else
                    {
                        ND2System_Reg.STATE_FLAG.bit.State = state_start;
                    }
                }
                break;

            case state_start://system is in start state and wait to stop/run
                if(ND2System_Reg.STATE_FLAG.bit.StopCommand == 1)
                {
                    ND2System_Reg.STATE_FLAG.bit.State = state_stopping;
                    ClrDataStopping();
                }else if(ND2System_Reg.STATE_FLAG.bit.ChkACDC==1)//run/start continue
                {
                    buff_data = OpCmnd-SpCmnd;//determind how far between SP(acc/dec) and SP(source)
                    buff_data = _IQ17abs(buff_data);
                    if(buff_data<=_IQ17(0.001))//check steady state by checking diff. of o/p
                    {
                        ND2System_Reg.STATE_FLAG.bit.State = state_run;
                    }
                }else
                {
                    ND2System_Reg.STATE_FLAG.bit.ChkACDC = 1;
                }
                break;
            case state_startAT://system is in start autotune function
                if(ND2System_Reg.STATE_FLAG.bit.StopCommand == 1||
                   AutoTuneCtrlRegs.FLAGALLPARA.bit.ActiveAT==0)//waiting for stop autotune/finish operation
                {
                    ND2System_Reg.STATE_FLAG.bit.State = state_stoppingAT;
                }
                break;

            case state_run://system is in start state and wait to stop or run
                if(ND2System_Reg.STATE_FLAG.bit.StopCommand == 1)
                {
                    ND2System_Reg.STATE_FLAG.bit.State = state_stopping;
                    ClrDataStopping();
                }
                break;

            case state_stopping:
                if(ND2System_Reg.STATE_FLAG.bit.RunCommand == 1)
                {
                    ND2System_Reg.STATE_FLAG.bit.State = state_start;
                }else//stopping continue
                {
                    if(OpCmnd == 0)
                    {
                        ClrDataStop();
                        if(Flag_DCbreak==0)
                        {
                            if(RamTestdelaystop == grp_num[0].value[25])
                            {
                                ND2System_Reg.STATE_FLAG.bit.State = state_stop;
                            }else
                            {
                                RamTestdelaystop++;
                            }
                        }
                    }
                }
                break;
            case state_stoppingAT://state stopping autoune
                if(grp_num[5].value[31] == 0)
                {
                    ND2System_Reg.STATE_FLAG.bit.State = state_stop;
                }
                break;
        }
        ND2System_Reg.STATE_FLAG.all = ND2System_Reg.STATE_FLAG.all & 0xFF9F;//clearing bit run/stop command
}
/////////////////////////////////////////////////////////////////////////////////
float32 SkipF(float32 FtargetSkip1)
{
_iq FtargetSkip= _IQ17(FtargetSkip1);       //target F in skip function
_iq absFtargetSkip = _IQ17abs(FtargetSkip);
struct {
    _iq FSkipMax;
    _iq FSkipMed;
    _iq FSkipMin;
}FSkipBuff = {0,0,0};
_iq *pntrskip = &FSkipBuff.FSkipMax;
struct {                    // bits   description
   Uint16 direction:1;  
   Uint16 index:2; 
   Uint16 rechkind:2;//recheck index for rechecking intersection band
   Uint16 decel:1;//decel=1
} bufferskip={0}; 

    if(_IQ17abs(MainIQ_Variable.FCommandA)>absFtargetSkip)
    {
        bufferskip.decel=1;
    }
    absFtargetSkip = _IQ17mpy(absFtargetSkip,MainIQ_Variable.FRef);
    if(MainIQ_Variable.JumpF1>=MainIQ_Variable.JumpF2)
    {
        if(MainIQ_Variable.JumpF2>=MainIQ_Variable.JumpF3)
        {
            FSkipBuff.FSkipMax = MainIQ_Variable.JumpF1;
            FSkipBuff.FSkipMed = MainIQ_Variable.JumpF2;
            FSkipBuff.FSkipMin = MainIQ_Variable.JumpF3;
        }else if(MainIQ_Variable.JumpF1>=MainIQ_Variable.JumpF3)
        {
            FSkipBuff.FSkipMax = MainIQ_Variable.JumpF1;
            FSkipBuff.FSkipMed = MainIQ_Variable.JumpF3;
            FSkipBuff.FSkipMin = MainIQ_Variable.JumpF2;
        }else
        {
            FSkipBuff.FSkipMax = MainIQ_Variable.JumpF3;
            FSkipBuff.FSkipMed = MainIQ_Variable.JumpF1;
            FSkipBuff.FSkipMin = MainIQ_Variable.JumpF2;
        }
    }else
    {
        if(MainIQ_Variable.JumpF1>=MainIQ_Variable.JumpF3)
        {
            FSkipBuff.FSkipMax = MainIQ_Variable.JumpF2;
            FSkipBuff.FSkipMed = MainIQ_Variable.JumpF1;
            FSkipBuff.FSkipMin = MainIQ_Variable.JumpF3;
        }else if(MainIQ_Variable.JumpF2>=MainIQ_Variable.JumpF3)
        {
            FSkipBuff.FSkipMax = MainIQ_Variable.JumpF2;
            FSkipBuff.FSkipMed = MainIQ_Variable.JumpF3;
            FSkipBuff.FSkipMin = MainIQ_Variable.JumpF1;
        }else
        {
            FSkipBuff.FSkipMax = MainIQ_Variable.JumpF3;
            FSkipBuff.FSkipMed = MainIQ_Variable.JumpF2;
            FSkipBuff.FSkipMin = MainIQ_Variable.JumpF1;
        }
    }
    bufferskip.direction = (FtargetSkip<0)?1:0;//direction = 1 is reverse
    if(bufferskip.decel==1)
    {
        pntrskip = &FSkipBuff.FSkipMax;
        for(bufferskip.rechkind=0;bufferskip.rechkind<3;bufferskip.rechkind++)
        {
            for(bufferskip.index=0;bufferskip.index<3;bufferskip.index++)
            {
                if(_IQ17abs(*(pntrskip+bufferskip.index)-absFtargetSkip)-MainIQ_Variable.HystrsJumpF<0)
                {
                    absFtargetSkip = *(pntrskip+bufferskip.index)+MainIQ_Variable.HystrsJumpF;
                }
            }
        }
    }else
    {
        pntrskip = &FSkipBuff.FSkipMin;
        for(bufferskip.rechkind=0;bufferskip.rechkind<3;bufferskip.rechkind++)
        {
            for(bufferskip.index=0;bufferskip.index<3;bufferskip.index++)
            {
                if(_IQ17abs(*(pntrskip-bufferskip.index)-absFtargetSkip)-MainIQ_Variable.HystrsJumpF<0)
                {
                    if(*(pntrskip-bufferskip.index)-MainIQ_Variable.HystrsJumpF>=0)
                    {
                        absFtargetSkip = *(pntrskip-bufferskip.index)-MainIQ_Variable.HystrsJumpF;
                    }else
                    {
                        absFtargetSkip = 0;
                    }
                }
            }
        }
    }
    absFtargetSkip = (bufferskip.direction==1)?_IQ17mpy(absFtargetSkip,MinusIQ):absFtargetSkip;//to original direction
    FtargetSkip = _IQ17div(absFtargetSkip,MainIQ_Variable.FRef);//converse to PU
    return _IQ17toF(FtargetSkip);
}
/////////////////////////////////////////////////////////////////////////////////

