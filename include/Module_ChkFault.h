//added all chanel conversion
#ifndef MODULE_ChkFault_H
#define MODULE_ChkFault_H

//fault data 0:Ready,1:Acc,2:Dec,3:Steady
#define	oc0		3  
#define	oc1		4  
#define	oc2		5  
#define	oc3		6  
#define	sc0		7  
#define	sc1		8  
#define	sc2		9  
#define	sc3		10 
#define	ou0		11 
#define	ou1		12 
#define	ou2		13 
#define	ou3		14 
#define	oh0		15 
#define	oh1		16 
#define	oh2		17 
#define	oh3		18 
#define	lu		19
#define	ol0		20 
#define	ol1		21 
#define	ol2		22 
#define	ol3		23 
#define	he0		24 
#define	he1		25 
#define	he2		26 
#define	he3		27 
#define	oh4		28 
#define    eep1	29
#define    eep2	30

#define DelayFirstLU	2	//delay first start checking LU for avoid LPF effect sec. unit
#define FaultSlowLoop	10 	//Loop slow for fault	
#define timeFaultWoc	40 	//timer fault OL after W-OC
#define step_dec		5	//step decreasing of accu.I OL

#define CutInPUOU		_IQ17(0.8)//pu for cut in ou
#define CutInLU400		_IQ17(380)//cut-in LU VDC bus for 400VAC

#define CutInLU200		_IQ17(190)	//cut-in LU VDC bus for 200VAC 
#define VDecLU			_IQ17(40)	//VDC start decel of LU+40VDC(F420)
#define VOutDecLU		_IQ17(80)	//VDC accel. back to normal state LU+80VDC

#define TempCutOutOH	_IQ17(50)	//Temp. clear fault OH

extern const Uint16 TableFault[];
extern const Uint16 *PntrFault;

//------------------------------------------------------------------------------------------------------------
struct FlagFault_bit{   					// bits   description 
   Uint16 UnClr_Fault:1;   
   Uint16 Fisrt_SC:1;	
   Uint16 W_OC:1;							//bit waring OC
   Uint16 W_OL:1;							//bit waring OL

   Uint16 ClrSuccess:1;						//clear fault success = 1
   Uint16 NotClrFault:1;					//unclearable fault(by key/DIN) set when SC,LU(F418=2,3,4)
   Uint16 ShtdwnOPFault:1;					//bit shutdown output after fault
   Uint16 BckupDINReset:1;					//backup status of DIN reset fault
   Uint16 LULockState:1;					//1:when set fault LU mode2-4
   											// :mode2;VDC<levelLU+Vdec
   											// :mode1;VDC<level LU
											//0:clear fault LU
   Uint16 IncTWOC:1;						//1=increase timer WOC,0=the timer need decrease
   Uint16 IncACCUI:1;						//1=increase accumulate I,0=the accumulate I need decrease
   Uint16 DelayChkLU:1;						//Delay checking LU after reset CPU
} ;

union FlagFault {
	Uint16	all;
	struct FlagFault_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct FlagFault1_bit{   					//bits   description
   Uint16 CntrRstFlt:3;						//bit cntr faults for auto reset
   Uint16 ResetAuto:1;						//for checking command	
   Uint16 Timer10mS:4;						//base timer 10ms   
   Uint16 Timer1s:7;						//base timer 1sec
} ;

union FlagFault1 {
	Uint16	all;
	struct FlagFault1_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct DataFault_bit{   					// bits   description 
   Uint16 Fault_EEP2:1;
   Uint16 Fault_SC:1;	
   Uint16 Fault_OH:1;	
   Uint16 Fault_HE:1;		
   Uint16 Fault_OC:1;		
   Uint16 Fault_OL:1;
   Uint16 Fault_OU:1;
   Uint16 Fault_LU:1;
   Uint16 Fault_CAN:1;
} ;

union DataFault {
	Uint16	all;
	struct DataFault_bit	bit;
} ;
struct FlagStall40Sec_bit
{
    Uint16 StallTimer10ms:4;
    Uint16 StallTimer1s:7;
    Uint16 Flag10ms_set:1;
    Uint16 Flag1s_set:1;
    Uint16 Timer_trig_Increase:1;
};
union FlagStall40Sec
{
    Uint16  all;
    struct FlagStall40Sec_bit   bit;
};
//------------------------------------------------------------------------------------------------------------ 
//Do not change line of F030-F033
struct CHKFAULT_REG {
	union FlagFault				FLAG_FAULT;
	union FlagFault1 			FLAG_FAULT1;
	union DataFault				DATA_FAULT;

	union FlagStall40Sec        FLAGSTALL40SEC;

	Uint16                     	Fault_Buffer;   
	Uint16						TimerLU;		//timer LU for active type(F418=2,3,4)
	Uint16						TimerOL_WOC;	//timer cut-off fault OL for W-OC
	Uint16						TimerChkLU;		//timer delay LU after reset
	Uint16 						Timer_ResetFlt;	//timer for auto reset fault

	float32						Accu_I;			//accumulation of i^6

	_iq							I_Fault;		//Ipu with Imotor
	_iq							Level_LU;		//cut-point of LU
	_iq 						DecLU;			//Decel LU
	_iq							TempCutInOH1;	//F030 Temp fault OH channel1
	_iq							TempCutInOH2;	//F031 Temp fault OH channel2
	_iq							TempCutInOH3;	//F032 Temp fault OH channel3
	_iq							TempCutInOH4;	//F033 Temp fault OH channel4
}; 
extern struct CHKFAULT_REG ChkFault_Reg; 


#endif  

//===========================================================================
// End of file.
//===========================================================================
