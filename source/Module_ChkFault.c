//got fault eep2 whenuser setting but not completed
//completed : check eep2(e2prom fail)
//use "ChkFault_Reg.DATA_FAULT.bit.Fault_EEP2 = 1" to dont write eep 
//debugged : change bit to "ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC" for only one time trip
//completed : SC0-SC3 follow system status
//add : variable const type
//revised : mothod of pointer fault
//debugged : after fault no key-in check
//completed : OH/debugged one time fault trigger
//completed : HE fault
//completed : clear fault by DIN
//completed : check OC fault
//completed : bit warning OL with select M1/M2
//revised : checking W-OL,W-OC
//revised : checking W-OC with F401-F403
//revised : checking W-OC/W-OL
//revised : check warning OC
//revised : check warning OL
//completed : timer 1sec
//completed : fault OL from W-OC
//revised : cal I check fault
//correct time accumulation of OL
//revised : bit stop from fault
//revised : "buff_icut" for I^6 as IQ15 for avoid overflow if it's IQ17
//26/4/2012 : completed : fault OU 
//26/4/2012 : revised : OL with checking bit fault OL before process 
//26/4/2012 : completed : fault LU
//27/4/2012 : debug : delay timer after reset in LU checking
//27/4/2012 : revised : move load display table from this rountine to display rountine
//14/5/2012 : added : operation of F418 = 0,1
//14/5/2012 : added : operation of F418 = 0,1,2,3,4
//15/5/2012 : revised : cutting point OU in "Cal_Cutfault"
//21/5/2012 : revised : remove KeyPad_reg.KEY_FLAG.bit.state_key1=0 in clearing fault rountine
//2012-6-18 : added : open/short circuit of temp.sensor is OH0
//2012-6-21 : revised : bit shutdown output, order of operation DIN clear fault
//2012-7-2 : revised auto-reset by DIN
//2012-7-4 : remove stop command when fault
//2012-7-23 : revised pointer to current motor
//2012-7-27 : use function pow(x,y) in fault(I^6)
//2012-8-7 : revised OL with Accumulation I
//2012-8-7 : revised decreasing Accumulation Ifault with flag
//2012-8-7 : completed OL 
//2012-8-7 : add delay R/C in LU
//2012-8-9 : revised decrease timer WOC
//2012-8-9 : revised check WOC-WOL
//2012-8-17 : debugged check WOC if Icut=0  no warnning
//2012-8-21 : use bit stall-I active will effect bit WOC(avoid decel condition will reset WOC)
//2012-8-21 : change bit WOC to stall-I active for running timer 40sec
//2012-8-30 : completed LU mode0
//2012-8-30 : revised clearing fault 
//2012-8-30 : completed auto reset fault
//2012-8-30 : completed LU mode1
//2012-8-30 : completed LU mode2
//2012-8-30 : completed LU mode3
//2012-8-30 : completed LU mode4
//2012-8-31 : revised voltage decel lu 
//2012-8-31 : revised exit/enter dec LU mode 460/420
//2012-9-1 : change ram name timer reset fault
//2012-9-3 : debugged clear eep1(data e2prom over range) with clearing in function clear fault
//2012-9-10 : change bit loop-check fault by using UnclrFault
//2012-9-10 : revised no fault before checking LU
//2012-9-29 : add delay check first LU
//2012-11-7 : revise formular I check fault by PU
//2012-11-28 : revised priority fault(eep2,other), one time checking,1ms scan time
//2012-11-29 : got fault 40sec of F401
//2012-11-29 : completed increase/decrease TWOC F401
//2012-11-29 : completed increase/decrease AccuI F408
//2012-11-29 : use immediate Ip,VDC before LPF for checking OC,OU
//2013-1-18 : sensor temp. open/short are oh4
//2013-2-15 : revise clearing fault and fault ee2p/upload,dnload
//2013-2-15 : add function CAN Fail when uploading
//2013-2-15 : add function CAN Fail when download
//2013-3-5 : add bit test run for disable all faults
//2013-3-8 : restart timer for LU with 1 because it used in digital output
//2013-4-1 : remove checking RchargeC before check LU
//2013-4-1 : clear fault LU when VDC>LUlevel+VOutDecLU
//2013-4-1 : use End_initKeypad instead of Syst_ready
//2013-4-18 : revise delay checking LU with time-out RC
//2013-5-6 : replace bit test run with MAIN_FLAG2[TestNoFault]
//2013-5-28 : revise variable OH,temp cut out 50C
//2013-6-11 : use fault buffer eep1,eep2
//2013-6-18 : check CAN fault after no fault
//2013-6-29 : revise cut fault OC@2.25X of inverter(there is saturation of H/W)
//2013-7-8 : revise clear timer for restarting LU2-4 in "clear fault"
//2013-7-17 : Fault OC when flying = Ichkfault
//2013-7-19 : add hysteresis for LU mode1
//2013-7-23 : Check fault 4channels
//2013-7-23 : Clearing fault 4channels
//2013-7-24 : revise status circuit temp for check fault/clear fault
//2013-8-1 : revise gap decel LU mode2
//2013-8-20 : check bit datafault before check fault
//2013-10-19 : debug check bit data fault(no fault,LU) before check fault LU

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct CHKFAULT_REG ChkFault_Reg ={0,0,0,0,0,
                                   0,0,0,0,0,
                                   0,0,0,0,0,
                                   0,0};
const Uint16 TableFault[]={oc1,oc2,oc3,oc0,//0-3rd
                           sc1,sc2,sc3,sc0,//4th-7th
                           ou1,ou2,ou3,ou0,//8th-11th
                           oh1,oh2,oh3,oh4,//12th-15th
                           lu,            //16
                           ol1,ol2,ol3,ol0,//17th-20th
                           he1,he2,he3,he0};//21th-24th
/*  0=>Stop
    1=>Acc
    2=>Dec
    3=>Steady*/
//---External variable import--------------------------------------
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];  

extern int First_Test_ND2;

_iq I_Warning = 0;
//////////////////////////////////////////////////////////////////////////// 
void Chk_Fault(void)
{
struct {
    Uint16 set10ms:1;
    Uint16 set1s:1;
    Uint16 noshtdwnop:1;
}flagChk_Fault={0}; 

    ND2System_Reg.MAIN_FLAG2.bit.TestNoFault = 0;
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
    {
        ChkFault_Reg.FLAG_FAULT1.bit.Timer10mS++;
        if(ChkFault_Reg.FLAG_FAULT1.bit.Timer10mS>=10)//Loop 10ms
        {
            ChkFault_Reg.FLAG_FAULT1.bit.Timer10mS = 0;
            flagChk_Fault.set10ms = 1;

            ChkFault_Reg.FLAG_FAULT1.bit.Timer1s++;
            if(ChkFault_Reg.FLAG_FAULT1.bit.Timer1s>=100)
            {
                ChkFault_Reg.FLAG_FAULT1.bit.Timer1s = 0;
                flagChk_Fault.set1s = 1;
            }
        }
        Clr_FaultDIN();//clear fault by DIN
        Cal_Cutfault();//cal. cut-point VDC,I
        AutoRstFlt();
        Chk_FaultE2P();
        Chk_Warning();

        //Chk_FaultTimer();

        Chk_FaultHE();
        Chk_FaultCAN();
        if(ND2System_Reg.MAIN_FLAG2.bit.TestNoFault==0)
        {
            Chk_FaultSC();
            Chk_FaultOH();
            Chk_FaultOC();
            Chk_FaultOU();
        }
        if(First_Test_ND2 == 0 && ND2System_Reg.MAIN_FLAG2.bit.TestNoFault==0)//First Test)
        {
            Chk_FaultLU();//check cut-in,cut-off fault LU
        }
        //if(flagChk_Fault.set1s==1/* && ND2System_Reg.MAIN_FLAG2.bit.TestNoFault==0*/)//Loop 1s
        if(flagChk_Fault.set1s==1)//Loop 1s
        {
            //flagChk_Fault.set1s = 0;
            Chk_FaultOL();
            Chk_DecOL();

        }
        /*if(flagChk_Fault.set1s==1)//Loop 1s
        {
            Chk_DecOL();//check condition decrease OL data(TWOC,AccuI)
        }*/

        if(ChkFault_Reg.Fault_Buffer != 0
                && ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault==0)
        {
            KeyPad_reg.GENERAL_FLAG.bit.new_fault = (ChkFault_Reg.DATA_FAULT.bit.Fault_LU==1
                    &&ND2System_Reg.STATE_FLAG.bit.State == state_stop)?0:1;//not save LU when stop

            KeyPad_reg.MODE_FLAG.bit.setting_mode   = 0;
            ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault = 1;

            if(KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==1
                    ||ChkFault_Reg.DATA_FAULT.bit.Fault_CAN==1 )
            {
                flagChk_Fault.noshtdwnop = 1;
            }
            if(flagChk_Fault.noshtdwnop==0)//only fault e2prom not stop
            {
                ChkFault_Reg.FLAG_FAULT.bit.ShtdwnOPFault = 1;
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_DecOL(void)
{
    //Decrease data TWOC
/*    if(ChkFault_Reg.FLAG_FAULT.bit.IncTWOC==1)
    {
        ChkFault_Reg.FLAG_FAULT.bit.IncTWOC = 0;
    }
    else
    {
        if(ChkFault_Reg.TimerOL_WOC > 0)
        {
            ChkFault_Reg.TimerOL_WOC--;
        }
    }
*/
    //Decrease data Accumulate I
    if(ChkFault_Reg.FLAG_FAULT.bit.IncACCUI==1)
    {
        ChkFault_Reg.FLAG_FAULT.bit.IncACCUI = 0;
    }
    else
    {
        if(ChkFault_Reg.Accu_I<=step_dec)
        {
            ChkFault_Reg.Accu_I = 0;
        }
        else
        {
            ChkFault_Reg.Accu_I = ChkFault_Reg.Accu_I-step_dec;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
void AutoRstFlt(void)
{
Uint16 buff=0;

    buff = ChkFault_Reg.DATA_FAULT.all & 0xFFFC;//SC,eep2 no auto reset
    if(buff==0)
    {
        ChkFault_Reg.Timer_ResetFlt = 0;//no fault occure
    }
    else if(ChkFault_Reg.FLAG_FAULT.bit.NotClrFault==0)
    {

        switch (grp_num[4].value[27])
        {
        case 1:
            if((ChkFault_Reg.FLAG_FAULT1.bit.CntrRstFlt<grp_num[4].value[24])&&(ChkFault_Reg.DATA_FAULT.bit.Fault_LU == 0))
            {
                if((ChkFault_Reg.Timer_ResetFlt>=(grp_num[4].value[25]*1000))) //ms unit
                {
                    Clr_Fault();
                    if(ChkFault_Reg.Fault_Buffer==0)//check clear success or not?
                    {
                        ChkFault_Reg.FLAG_FAULT1.bit.CntrRstFlt++;
                        ChkFault_Reg.Timer_ResetFlt = 0;
                    }
                }
                else
                {
                    ChkFault_Reg.Timer_ResetFlt++;
                }
            }
    //Disable counter for under voltage fault
            else if (ChkFault_Reg.DATA_FAULT.bit.Fault_LU == 1)
            {
                if((ChkFault_Reg.Timer_ResetFlt>=(grp_num[4].value[25]*1000))) //ms unit
                {
                    Clr_Fault();
                    if(ChkFault_Reg.Fault_Buffer==0)//check clear success or not?
                    {
                        ChkFault_Reg.Timer_ResetFlt = 0;
                    }
                }
                else
                {
                    ChkFault_Reg.Timer_ResetFlt++;
                }
            }
            break;
        default:
            if((ChkFault_Reg.FLAG_FAULT1.bit.CntrRstFlt<grp_num[4].value[24]))
            {
                if((ChkFault_Reg.Timer_ResetFlt>=(grp_num[4].value[25]*1000))) //ms unit
                {
                    Clr_Fault();
                    if(ChkFault_Reg.Fault_Buffer==0)//check clear success or not?
                    {
                        ChkFault_Reg.FLAG_FAULT1.bit.CntrRstFlt++;
                        ChkFault_Reg.Timer_ResetFlt = 0;
                    }
                }
                else
                {
                    ChkFault_Reg.Timer_ResetFlt++;
                }
            }

        }


        /*
        if((ChkFault_Reg.FLAG_FAULT1.bit.CntrRstFlt<grp_num[4].value[24])&&(ChkFault_Reg.DATA_FAULT.bit.Fault_LU == 0))
        {
            if((ChkFault_Reg.Timer_ResetFlt>=(grp_num[4].value[25]*1000))) //ms unit
            {
                Clr_Fault();
                if(ChkFault_Reg.Fault_Buffer==0)//check clear success or not?
                {
                    ChkFault_Reg.FLAG_FAULT1.bit.CntrRstFlt++;
                    ChkFault_Reg.Timer_ResetFlt = 0;
                }
            }
            else
            {
                ChkFault_Reg.Timer_ResetFlt++;
            }
        }
//Disable counter for under voltage fault
        else if (ChkFault_Reg.DATA_FAULT.bit.Fault_LU == 1)
        {
            if((ChkFault_Reg.Timer_ResetFlt>=(grp_num[4].value[25]*1000))) //ms unit
            {
                Clr_Fault();
                if(ChkFault_Reg.Fault_Buffer==0)//check clear success or not?
                {
                    ChkFault_Reg.Timer_ResetFlt = 0;
                }
            }
            else
            {
                ChkFault_Reg.Timer_ResetFlt++;
            }
        }
        */
    }
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultLU(void)
{
_iq17 buffdata   = 0;
_iq17 timerlu    = 0;
Uint16 bufffault = 0;

    if(ChkFault_Reg.FLAG_FAULT.bit.DelayChkLU==1)//MainIQ_Variable.Time_RelayRC
    {
        bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//protection of double fault exept CAN/eep fault
        if(bufffault==0 || bufffault==0x0080)//Enter with no fault and auto clear faultLU mode
        {
            switch(grp_num[4].value[21])
            {
                case 1:LUMode1();
                    break;
                case 2:LUMode2();
                    break;
                case 3:LUMode3();
                    break;
                case 4:LUMode4();
                    break;
                default:LUMode0();
            }
        }
    }else
    {
        timerlu  = MainIQ_Variable.Time_RelayRC+_IQ17(0.5);
        buffdata = _IQ17div(_IQ17(ChkFault_Reg.TimerChkLU),_IQ17(1000));
        if(buffdata>=timerlu)
        {
            ChkFault_Reg.FLAG_FAULT.bit.DelayChkLU = 1;     //completed delay with time RC
        }else
        {
            ChkFault_Reg.TimerChkLU++;
        }
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void LUMode4(void)
{
    if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0)
    {
        if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU)
        {
            SetFaultLU();
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 1;
            ChkFault_Reg.FLAG_FAULT.bit.NotClrFault = 1;//clear fault
        }else
        {
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 0;
        }
    }else
    {
        if(VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VOutDecLU)
        {
            ChkTmrLU();
            if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0 && ND2System_Reg.STATE_FLAG.bit.BckUpSTT == 1)
            {
                FlyStrt_Reg.Flag_FlyStrt.bit.LockFly = FlyStrt_Reg.Flag_FlyStrt.bit.LockFly | 0x1;  //disable flying start
            }
        }else
        {
            ChkFault_Reg.TimerLU = 0;
        }
    }

}   
/////////////////////////////////////////////////////////////////////////////////
void LUMode3(void)
{
    if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0)
    {
        if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU)
        {
            SetFaultLU();
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 1;
            ChkFault_Reg.FLAG_FAULT.bit.NotClrFault = 1;//clear fault
        }else
        {
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 0;
        }
    }else
    {
        if(VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VOutDecLU)
        {
            ChkTmrLU();
        }else
        {
            ChkFault_Reg.TimerLU = 0;
        }
    }
}  
/////////////////////////////////////////////////////////////////////////////////
void LUMode2(void)
{
    if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0)
    {
        if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU)
        {
            SetFaultLU();
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 1;
            ChkFault_Reg.FLAG_FAULT.bit.NotClrFault = 1;//clear fault
        }else if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU+VDecLU)
        {
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 1;
        }else if(VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VDecLU+VOutDecLU)
        {
            ChkFault_Reg.FLAG_FAULT.bit.LULockState = 0;
        }
    }else
    {
        if(VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VOutDecLU)
        {
            ChkTmrLU();
        }else
        {
            ChkFault_Reg.TimerLU = 0;
        }
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void LUMode1(void)
{
    if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0)
    {
        if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU)
        {
            ChkFault_Reg.FLAG_FAULT.bit.LULockState   = 1;
            ChkFault_Reg.FLAG_FAULT.bit.ShtdwnOPFault = 1;
        }else
        {
            if((VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VOutDecLU) &&
               (ChkFault_Reg.FLAG_FAULT.bit.LULockState == 1))
            {
                SetFaultLU();
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
void LUMode0(void)
{
    if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==0)
    {
        if(VDC_Read_reg.VdcBus<ChkFault_Reg.Level_LU)
        {
            SetFaultLU();
        }
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void ChkTmrLU(void) 
{
    ChkFault_Reg.TimerLU++;
    if(ChkFault_Reg.TimerLU>=grp_num[4].value[22]*100)
    {
        ChkFault_Reg.FLAG_FAULT.bit.LULockState = 0;
        ChkFault_Reg.FLAG_FAULT.bit.NotClrFault = 0;
        Clr_Fault();
    }
}
/////////////////////////////////////////////////////////////////////////////////
void SetFaultLU(void)
{
    ChkFault_Reg.Fault_Buffer = TableFault[16];
    ChkFault_Reg.DATA_FAULT.bit.Fault_LU = 1;
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultOU(void)
{
Uint16 bufffault = 0;

    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//FaultLU,OU,OL,OC,HE,OH,SC
    if(bufffault==0)
    {
        if(ND2System_Reg.STATE_FLAG.bit.CmpltdRC==1)
        {
            if(VDC_Read_reg.BuffPUVDC>CutInPUOU)
            {
                ChkFault_Reg.Fault_Buffer = TableFault[8+ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd];//first of sc is 4th
                ChkFault_Reg.DATA_FAULT.bit.Fault_OU = 1;//clear after clear faul
            }
        }
    }


}   
/////////////////////////////////////////////////////////////////////////////////
void Cal_Cutfault(void)
{
_iq buff_i=0;//buffer of %Irms

    /*
                          Irms*Irate
        I check fault = ---------------*SQRT(2)
                           Imotor
    */
//Gus--------------------------------------------------------------------------------------------------------------------------------------------
    if (grp_num[1].value[2]>grp_num[0].value[3])
    {
        grp_num[1].value[2] = grp_num[0].value[3];
        //buff_i = _IQ17div(IV_Read_reg.I_rms,MainIQ_Variable.IPU_Motor);//Irms/IPUmotor
        //ChkFault_Reg.I_Fault = _IQ17mpy(buff_i,SQRT2);
    }
    else if (grp_num[1].value[18]>grp_num[0].value[3])
    {
        grp_num[1].value[18]=grp_num[0].value[3];
        //buff_i = _IQ17div(IV_Read_reg.I_rms,MainIQ_Variable.IPU_Motor);//Irms/IPUmotor
        //ChkFault_Reg.I_Fault = _IQ17mpy(buff_i,SQRT2);
    }
    else if ((grp_num[1].value[2]>grp_num[0].value[3])&&(grp_num[1].value[18]>grp_num[0].value[3]))
    {
        grp_num[1].value[2]  = grp_num[0].value[3];
        grp_num[1].value[18] = grp_num[0].value[3];
        //buff_i = _IQ17div(IV_Read_reg.I_rms,MainIQ_Variable.IPU_Motor);//Irms/IPUmotor
        //ChkFault_Reg.I_Fault = _IQ17mpy(buff_i,SQRT2);
    }

//----------------------------------------------------------------------------------------------------------------------------------------------------

//Original----------------------------------------------------------------------------------------------------------------------------------
    buff_i = _IQ17div(IV_Read_reg.I_rms,MainIQ_Variable.IPU_Motor);//Irms/IPUmotor
    ChkFault_Reg.I_Fault = _IQ17mpy(buff_i,SQRT2);
//-----------------------------------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------
    //check model for cutting-level
    if(grp_num[0].value[4]==1)//check model 1:400VAC,0:240VAC
    {
        ChkFault_Reg.Level_LU = CutInLU400;
    }
    else
    {
        ChkFault_Reg.Level_LU = CutInLU200;
    }
    //-----------------------------------------
}  
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultOL(void)
{
_iq15   buff_icut=0,buff_iq15=0;//icut point Ffinal*overload time/4.4
_iq buff_cal        = 0;                 //general buffer for calculation
float32 buff_float  = 0;
Uint16 bufffault    = 0;
struct {
    Uint16 incTWOC:1;           //increasing TWOC
    Uint16 incaccI:1;           //increasing accumulating I
}flagChk_FaultOL={0};

    //-------Section OL from Stall I 40sec--------------------------------
/*    if(ChkFault_Reg.FLAG_FAULT.bit.W_OC == 1)
    {
        if(ChkFault_Reg.TimerOL_WOC>=timeFaultWoc)
        {
            goto Set_FaultOL;
        }

        else
        {
            ChkFault_Reg.TimerOL_WOC++;
            //flagChk_FaultOL.incTWOC = 1;
        }
    }
    //ChkFault_Reg.FLAG_FAULT.bit.IncTWOC = flagChk_FaultOL.incTWOC;
*/
    if(ChkFault_Reg.TimerOL_WOC>=timeFaultWoc)
    {
        goto Set_FaultOL;
    }


    //-------Section OL from accumulation I-----------------------------
    /*              Fault OL : acc_i>i_cut
                    acc_i = i(k)^6+acc_i

                                     Fs*overload time
                    i_cut =     ---------------------------
                                          4.4
    */
    if(grp_num[4].value[11]==1)//Enable thermal over load
    {
        if(ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemSteady)
        {
            buff_cal = _IQ17(grp_num[4].value[12]);
            buff_cal = _IQ17div(buff_cal,_IQ17(100.0));

            if(ChkFault_Reg.I_Fault>=buff_cal)//check case of Ipu larger than level
            {
                buff_float = _IQ17toF(ChkFault_Reg.I_Fault);
                buff_float = pow(buff_float,6);

                ChkFault_Reg.Accu_I = buff_float+ChkFault_Reg.Accu_I;

                buff_iq15 = _IQtoIQ15(_IQ17mpy(MainIQ_Variable.FRef,_IQ17abs(MainIQ_Variable.FCommand)));
                buff_icut = _IQ15(grp_num[4].value[13]);
                buff_icut = _IQ15div(buff_icut,_IQ15(4.4));//overload time/4.4
                buff_icut = _IQ15mpy(buff_icut,buff_iq15);//overload time*Ffinal/4.4
                flagChk_FaultOL.incaccI = 1;

                if(ChkFault_Reg.Accu_I>_IQ15toF(buff_icut))
                {
                    flagChk_FaultOL.incaccI = 0;

Set_FaultOL:
                    ChkFault_Reg.TimerOL_WOC = 0;

                    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//FaultLU,OU,OL,OC,HE,OH,SC

                    if(bufffault==0)
                    {
                        ChkFault_Reg.Fault_Buffer            = TableFault[17+ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd];//first of sc is 4th
                        ChkFault_Reg.DATA_FAULT.bit.Fault_OL = 1;//clear after clear fault
                    }
                }
            }
        }
    }

    ChkFault_Reg.FLAG_FAULT.bit.IncACCUI = flagChk_FaultOL.incaccI;
}    
/////////////////////////////////////////////////////////////////////////////////
void Chk_Warning(void)
{
//Uint16 bufffault    = 0;
_iq i_cutWOC  = 0;//I cut-off warning OC(operation of Over current stall)

struct {
    Uint16 buffWOC:1;
    Uint16 buffWOL:1;
}flagChk_Warning={0};


/*--ND2System_Reg.STATE_FLAG.bit.State--
#define state_stop          1
#define state_start         2
#define state_startAT       3//state start autotune
#define state_run           4
#define state_stopping      8
#define state_stoppingAT    9//state stopping autotune
---ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd--
#define SystemReady     3
#define SystemAccel     0
#define SystemDecel     1
#define SystemSteady    2
*/
    //check W-OC
    if(ND2System_Reg.STATE_FLAG.bit.State == state_run
            || ND2System_Reg.STATE_FLAG.bit.State == state_start)
    {
        if(ND2System_Reg.FLAG_STALL.bit.StallIActive==1)
        {
            flagChk_Warning.buffWOC = ND2System_Reg.FLAG_STALL.bit.StallIActive;//in the case of under stalling I

            ChkFault_Reg.FLAGSTALL40SEC.bit.Timer_trig_Increase = 1;
        }
        else if(ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd == SystemSteady)
        {
            i_cutWOC = _IQ17(grp_num[4].value[2]);
        }
        else
        {
            i_cutWOC = _IQ17(grp_num[4].value[1]);
        }
        if(i_cutWOC!=0)
        {
            ND2System_Reg.StallI_Sp = _IQ17div(i_cutWOC,_IQ17(100.0));
            //determind W-OC bit
            if(ChkFault_Reg.I_Fault>=ND2System_Reg.StallI_Sp)
            {
                flagChk_Warning.buffWOC = 1;
                //ChkFault_Reg.FLAG_FAULT.bit.W_OC = 1;
            }
            /*else if(ChkFault_Reg.I_Fault>=_IQ17(1.00)
                    && ChkFault_Reg.I_Fault<=_IQ17(1.2))
            {
                flagChk_Warning.buffWOC = 0;
                //ChkFault_Reg.FLAG_FAULT.bit.W_OC = 0;
            }
            else if (ChkFault_Reg.I_Fault<=_IQ17(1.00))
            {
                flagChk_Warning.buffWOC = 0;
                //ChkFault_Reg.FLAG_FAULT.bit.W_OC = 0;
                ChkFault_Reg.FLAGSTALL40SEC.bit.Timer_trig_Increase = 0;
            }*/

            else if((MainIQ_Variable.FCommand == MainIQ_Variable.FCommandA)
                    &&(MainIQ_Variable.FCommand == MainIQ_Variable.FCommandB))
            {
                if(ChkFault_Reg.I_Fault<=_IQ17(1.00))
                {
                    ChkFault_Reg.FLAGSTALL40SEC.bit.Timer_trig_Increase = 0;
                }
            }
            else
            {
                flagChk_Warning.buffWOC = 0;
            }
        }
        else
        {
            flagChk_Warning.buffWOC = 0;
            //ChkFault_Reg.FLAG_FAULT.bit.W_OC = 0;
        }
        //ChkFault_Reg.FLAG_FAULT.bit.W_OC = flagChk_Warning.buffWOC;
    }
    ChkFault_Reg.FLAG_FAULT.bit.W_OC = flagChk_Warning.buffWOC;
//-----------------------------------------------------------
    //check W-OL
    if(ChkFault_Reg.FLAG_FAULT.bit.W_OC == 0
            ||ND2System_Reg.FLAG_STALL.bit.StallIActive == 0)
    {
        I_Warning = LowPassFilter(ChkFault_Reg.I_Fault,I_Warning,Coe_I_Warning,IQ17_1);

        if(I_Warning>=IQ17_1)
        {
            //flagChk_Warning.buffWOL = 1;
            ChkFault_Reg.FLAG_FAULT.bit.W_OL = 1;
        }
        //else if(ChkFault_Reg.I_Fault<_IQ17(0.99))
        else if(I_Warning<_IQ17(0.98))
        {
            //flagChk_Warning.buffWOL = 0;
            ChkFault_Reg.FLAG_FAULT.bit.W_OL = 0;
        }

        //ChkFault_Reg.FLAG_FAULT.bit.W_OL = flagChk_Warning.buffWOL;
    }
    else if(ChkFault_Reg.FLAG_FAULT.bit.W_OC == 1
            ||ND2System_Reg.FLAG_STALL.bit.StallIActive == 1)
    {
        //flagChk_Warning.buffWOL = 0;
        //ChkFault_Reg.FLAG_FAULT.bit.W_OL = flagChk_Warning.buffWOL;
        ChkFault_Reg.FLAG_FAULT.bit.W_OL = 0;
    }
    //ChkFault_Reg.FLAG_FAULT.bit.W_OL = flagChk_Warning.buffWOL;
}  
/////////////////////////////////////////////////////////////////////////////////
void Clr_FaultDIN(void)
{
    if(DgtlPrt_Reg.DataLDIN.bit.Alrm_reset == Active)
    {
        //-----------------------GUS---------------------------
        //Rapidly clear wiht a terminal command
        if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault==1)//check there's fault or not?
        {
            Clr_Fault();//clear fault process
        }
        //-----------------------GUS---------------------------

        //One time clear although the terminal command is set to active
        /*if(ChkFault_Reg.FLAG_FAULT.bit.BckupDINReset==Inactive)
        {
            ChkFault_Reg.FLAG_FAULT.bit.BckupDINReset = Active;
            if(ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault==1)//check there's fault or not?
            {
                Clr_Fault();//clear fault process
            }
        }*/
    }
    else
    {
        ChkFault_Reg.FLAG_FAULT.bit.BckupDINReset = Inactive;
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void Clr_Fault(void)
{
//clearing fault

    if(ChkFault_Reg.FLAG_FAULT.bit.NotClrFault == 0)
    {
        if(ChkFault_Reg.DATA_FAULT.bit.Fault_OH==1)
        {
            if(ChkFault_Reg.Fault_Buffer==oh1)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = ChkClrOH(Temperature_reg.CircuitTemp.bit.Channel1,arTemperature[0]);
            }else if(ChkFault_Reg.Fault_Buffer==oh2)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = ChkClrOH(Temperature_reg.CircuitTemp.bit.Channel2,arTemperature[1]);
            }else if(ChkFault_Reg.Fault_Buffer==oh3)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = ChkClrOH(Temperature_reg.CircuitTemp.bit.Channel3,arTemperature[2]);
            }else if(ChkFault_Reg.Fault_Buffer==oh4)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = ChkClrOH(Temperature_reg.CircuitTemp.bit.Channel4,arTemperature[3]);
            }
        //-------------------------------------------------------
        }
        else if(ChkFault_Reg.DATA_FAULT.bit.Fault_HE==1)
        {
            if(DgtlPrt_Reg.DataLDIN.bit.Ext_fault == Inactive)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = 1;
            }
        //-------------------------------------------------------
        }
        else if(ChkFault_Reg.DATA_FAULT.bit.Fault_OC==1)
        {
            //if(IV_Read_reg.I_Peak < _IQ17(1.95))
            if(ChkFault_Reg.I_Fault < _IQ17(1.95))
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = 1;
            }
        //-------------------------------------------------------
        }
        else if(ChkFault_Reg.DATA_FAULT.bit.Fault_OL==1)
        {
            ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = 1;
        //-------------------------------------------------------
        }
        else if(ChkFault_Reg.DATA_FAULT.bit.Fault_OU==1)
        {
            if(VDC_Read_reg.PUVDC<CutInPUOU)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = 1;
            }
        //-------------------------------------------------------
        }
        else if(ChkFault_Reg.DATA_FAULT.bit.Fault_LU==1)
        {
            if(grp_num[4].value[21]>=2)
            {
                ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = 1;
            }else
            {
                if(VDC_Read_reg.VdcBus>ChkFault_Reg.Level_LU+VOutDecLU)
                {
                    ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess = 1;
                }
            }
        //-------------------------------------------------------
        }
        else
        {
            KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error = 0;
            ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess   = 1;
            Bckup_reg.BCKUP_FLAG.bit.UpldFail        = 0;
            Bckup_reg.BCKUP_FLAG.bit.DnldFail        = 0;
        }
        if(ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess == 1)
        {
            FlyStrt_Reg.Flag_FlyStrt.bit.LockFly      = FlyStrt_Reg.Flag_FlyStrt.bit.LockFly & 0xE;//clear lock fly for LU case
            ChkFault_Reg.FLAG_FAULT.bit.ClrSuccess    = 0;
            ChkFault_Reg.FLAG_FAULT.bit.LULockState   = 0;
            ChkFault_Reg.FLAG_FAULT.bit.UnClr_Fault   = 0;//clear fault
            KeyPad_reg.DISP_TAB.bit.Disp_Table        = 0;
            ChkFault_Reg.Fault_Buffer                 = 0;
            ChkFault_Reg.DATA_FAULT.all               = ChkFault_Reg.DATA_FAULT.all & 0x0003;//clear all fault exept eep2,SC
            ChkFault_Reg.FLAG_FAULT.bit.ShtdwnOPFault = 0;
            KeyPad_reg.MODE_FLAG.bit.setting_mode     = (KeyPad_reg.MODE_FLAG.bit.mode_state!=0)?1:0;
            ChkFault_Reg.TimerLU                      = 0;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
Uint16 ChkClrOH(Uint16 CircuitTemp,_iq Temp) 
{
Uint16 resultclrfault=0;
    if((CircuitTemp<=1)&&(Temp<=TempCutOutOH))
    {
        resultclrfault = 1;
    }
    return resultclrfault;
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultHE(void)
{
Uint16 bufffault=0;

    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//FaultLU,OU,OL,OC,HE,OH,SC not enter
    if(bufffault==0)
    {
        if(DgtlPrt_Reg.DataLDIN.bit.Ext_fault == Active)
        {
            ChkFault_Reg.Fault_Buffer            = TableFault[21+ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd];//first of sc is 4th
            ChkFault_Reg.DATA_FAULT.bit.Fault_HE = 1;//clear after clear fault
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultOC(void)
{
Uint16 bufffault=0;

    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//FaultLU,OU,OL,OC,HE,OH,SC
    if(bufffault==0)
    {
        if(FlyStrt_Reg.Flag_FlyStrt.bit.FlyActive==1)
        {
            if(ChkFault_Reg.I_Fault>=_IQ17(1.5))
            {
                goto FaultOC;
            }
        }
        //else if(IV_Read_reg.BuffIp >= _IQ17(2.25)) //not use 2*1.414 because hardware saturated
        else if(ChkFault_Reg.I_Fault >= _IQ17(2.00)) //not use 2*1.414 because hardware saturated
        {
FaultOC:
            ChkFault_Reg.Fault_Buffer            = TableFault[0+ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd];//first of sc is 4th
            ChkFault_Reg.DATA_FAULT.bit.Fault_OC = 1;//clear after clear fault
        }
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultOH(void)
{
struct {
    Uint16 circuit:8;   //buffer for all channels
    Uint16 i:3;
    Uint16 chkcircuit:2;//buffer per channels for checking
}flagfaultoh={0}; 
_iq *bufftempcut=&ChkFault_Reg.TempCutInOH1;

Uint16 bufffault=0;

    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//FaultLU,OU,OL,OC,HE,OH,SC
    if(bufffault==0)
    {
        flagfaultoh.circuit = Temperature_reg.CircuitTemp.all;
        for(flagfaultoh.i=0;flagfaultoh.i<4;flagfaultoh.i++)
        {
            flagfaultoh.chkcircuit = flagfaultoh.circuit & 0x0003;
            if(flagfaultoh.chkcircuit!=0)
            {
                if(arTemperature[flagfaultoh.i]>=(*bufftempcut+flagfaultoh.i)||
                    (flagfaultoh.chkcircuit!=1))
                {
                    ChkFault_Reg.Fault_Buffer            = TableFault[12+flagfaultoh.i];
                    ChkFault_Reg.DATA_FAULT.bit.Fault_OH = 1;
                    flagfaultoh.i = 4;
                }
            }
            flagfaultoh.circuit = flagfaultoh.circuit>>2;
        }
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultSC(void)
{
Uint16 bufffault=0;

    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x00FE;//FaultLU,OU,OL,OC,HE,OH,SC
    if(bufffault==0)
    {
        if(ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC==1)//check fault SC and only first time to active
        {
            ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC    = 0;
            ChkFault_Reg.Fault_Buffer               = TableFault[4+ND2System_Reg.MAIN_FLAG2.bit.StatusCmnd];//first of sc is 4th
            ChkFault_Reg.DATA_FAULT.bit.Fault_SC    = 1;//if this bit set, it alway not clear
            ChkFault_Reg.FLAG_FAULT.bit.NotClrFault = 1;//unclearable fault
        }
    }
} 
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultE2P(void)
{
    if(KeyPad_reg.E2PROM_FLAG.bit.Loading_def == 0 &&
        ChkFault_Reg.Fault_Buffer != eep2)//while loading default will not functions
    {
        if(KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error == 1)
        {
            ChkFault_Reg.Fault_Buffer              = eep2;
            ChkFault_Reg.DATA_FAULT.bit.Fault_EEP2 = 1;//if this bit set, it alway not clear
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
void Chk_FaultCAN(void)
{
Uint16 bufffault=0;

    bufffault = ChkFault_Reg.DATA_FAULT.all & 0x0100;//FaultCAN not enter
    if(bufffault==0)
    {
        if(Bckup_reg.BCKUP_FLAG.bit.UpldFail==1
                ||Bckup_reg.BCKUP_FLAG.bit.DnldFail==1)
        {
            ChkFault_Reg.Fault_Buffer             = 124;
            ChkFault_Reg.DATA_FAULT.bit.Fault_CAN = 1;//if this bit set, it alway not clear
        }
    }
}

void Chk_FaultTimer(void)
{
    ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer10ms++;
    if(ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer10ms==10)//Loop 10ms
    {
        ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer10ms = 0;
        ChkFault_Reg.FLAGSTALL40SEC.bit.Flag10ms_set   = 1;

        ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer1s++;

        if(ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer1s==100)
        {
            ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer1s = 0;
            ChkFault_Reg.FLAGSTALL40SEC.bit.Flag1s_set   = 1;

            if(ChkFault_Reg.FLAGSTALL40SEC.bit.Timer_trig_Increase == 1)
            {
                ChkFault_Reg.TimerOL_WOC++;
            }
            else if((ChkFault_Reg.FLAGSTALL40SEC.bit.Timer_trig_Increase == 0)
                    && (ChkFault_Reg.TimerOL_WOC > 0)
                    && (ND2System_Reg.FLAG_STALL.bit.StallIActive == 0))
            {
                ChkFault_Reg.TimerOL_WOC--;
            }

        }
    }

    if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
    {
        Clear_FaultTimer();
    }

}
void Clear_FaultTimer(void)
{
    ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer10ms  = 0;
    ChkFault_Reg.FLAGSTALL40SEC.bit.StallTimer1s    = 0;
    ChkFault_Reg.FLAGSTALL40SEC.bit.Flag10ms_set    = 0;
    ChkFault_Reg.FLAGSTALL40SEC.bit.Flag1s_set      = 0;

    ChkFault_Reg.FLAGSTALL40SEC.bit.Timer_trig_Increase = 0;
    ChkFault_Reg.TimerOL_WOC = 0;

}
/////////////////////////////////////////////////////////////////////////////////
