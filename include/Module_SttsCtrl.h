
#ifndef MODULE_STTSCTRL_H
#define MODULE_STTSCTRL_H

#define state_stop			1
#define state_start 		2
#define state_startAT		3//state start autotune
#define state_run			4
#define state_stopping		8
#define state_stoppingAT	9//state stopping autotune

#define SystemAccel		0
#define SystemDecel		1
#define SystemSteady	2
#define SystemReady     3

#define GapAnalog	_IQ17(0.00005)
#define Gapdigital	0

#define UpperStallV		_IQ17(1)
#define LowerStallV		_IQ17(-1)
//------------------------------------------------------------------------------------------------------------
struct State_Flag_bit{   					// bits   description 
	Uint16 Fault:1;							
	Uint16 State:4;							/*__/__/__/__
											   |  |	|	|stop = 1													
											   |  |	|start = 2	
											   |  | |startAT = 3												
											   |  |run = 4
											   |stopping = 8
											   |stoppingAT = 9

											*/
	Uint16 RunCommand:1;	//run command,set after key pressed run
	Uint16 StopCommand:1;	//stop command,set after key pressed stop when no fault occur
	Uint16 BckUpSTT:1;		//backup status ny user or terminal(1=run,0=stop)
	Uint16 InitPU:1;		//initial data PU
	Uint16 ShtdwnOP:1;		//bit shutdown output immediately from fault,coast stop
	Uint16 UpdateCmd:1;		//bit update command(F/T)
	Uint16 CmpltdRC:1;		//bit set after time count completed
	Uint16 ChkACDC:1;		//
} ;

union State_Flag {
	Uint16	all;
	struct State_Flag_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct SttsCtrlT_bit{   					// bits   description 
	Uint16 T_AccDec:6;						//Tsampling check Acc/dec/steady 50ms
	Uint16 T_Smp_AccDecTOP:5;				//Tsampling Acc/dec/steady 30ms
} ;

union SttsCtrlT {
	Uint16	all;
	struct SttsCtrlT_bit bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct Flagstall_bit{   	// bits   description 
	Uint16 StallIActive:1;	//stall current active=1
	Uint16 StallDecF:1;		//stall f
	Uint16 StallFStop:1;	//check current F when stop
	//-------------------Gus----------------------------
	Uint16 SaveData_StallI:1;
	Uint16 DisableStallI:1;
	Uint16 StallActive_bit_Set:1;
	Uint16 StallActive_bit_Clear:1;
    //-------------------Gus----------------------------
} ;

union Flagstall {
	Uint16	all;
	struct Flagstall_bit bit;
} ;  
//------------------------------------------------------------------------------------------------------------
struct Main_Flag2_bit{   					// bits   description 
	Uint16	CmdType:1;					//0=Freq cmd,1=Torque cmd
	Uint16  TestNoFault:1;				//Disable some fault for test
	Uint16  Test:1;						//General Test
	Uint16	StatusCmnd:2;				//Command(F,T)=steady(0)accel(1)decel(2)ready(3)
	Uint16	Far:1;
	Uint16	Fly_En:1;
	Uint16	Fly_Delay:1;
	Uint16	Fly_Search:1;
	Uint16	Fly_Wait:1;
} ;

union Main_Flag2 {
	Uint16	all;
	struct Main_Flag2_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct FlagSystCtrl_bit{   					// bits   description 
	Uint16	DataACDCChnged:1;				//set after parameter for cal. s-curve changed in acc/dec condition
	Uint16	DataChnged:1;					//set after parameter for cal. s-curve changed 
	Uint16 	DecZero:1;						//set when the system decF to 0 when target and currentF are difference sign
	Uint16  ACDCCmd:1;						//control acc(0) or dec(1)
	Uint16  Syst_ready:1;					//bit check all bit check state is ready(init keypad, A2DI)
	Uint16  CmpltdACDC:1;					//Set after completed ACDC
} ;

union FlagSystCtrl {
	Uint16	all;
	struct FlagSystCtrl_bit 	bit;
} ;  
//------------------------------------------------------------------------------------------------------------

//!!!!Caution!!!! Dont change the order of IQ F407-F409,F503-F509,F522-F527
struct ND2SYSTEM_REG {
	union State_Flag		STATE_FLAG;			//status main flag
	union Main_Flag2		MAIN_FLAG2;
	union SttsCtrlT			STTSCTRL_TIMER;		//status control timer flag
	union FlagSystCtrl		FLAG_SYST_CTRL;		//general flag
	union Flagstall			FLAG_STALL;			//Flag stall V,I
	_iq						TargetACDC;			//target acc/dec scurve
	_iq						MainAccACDC;		//main acc time scurve
	_iq						MainDecACDC;		//main dec time scurve
	_iq						PUBaseACDC;			//main pu maxF time scurve
	_iq						InitAccACDC;		//init acc time scurve
	_iq						InitDecACDC;		//init dec time scurve
	_iq						FinalAccACDC;		//final acc time scurve
	_iq						FinalDecACDC;		//final dec time scurve
	float32					TcntrSCrv;			//counter s-curve
	float32					F_CMDA_X_1;			//data integrate
	float32					MidTACDC;			//mid time of s-curve
	_iq						InitTACDC;			//last init time s-curve
	_iq						FinalTACDC;			//last final time s-curve
	float32					SCrvTACDC;			//total time s-curve
	float32					A_SCurve;			//A-scurve***_iq30
	_iq						DeltaF;				//for cal in s-curve
	_iq						TargetSkip;			//check change target
	_iq						OutputSkip;
	_iq						TcntrStartingT;		//Counter hold time for starting Freq.
	_iq						Vboost;				//F503
	_iq						Program_F1;			//F504
	_iq						Program_F2;			//F505
	_iq						Program_F3;			//F506
	_iq						Program_V1;			//F507
	_iq						Program_V2;			//F508
	_iq						Program_V3;			//F509 
	Uint16					CntrTimeDelayRC;	//Counter time delay r/c
	_iq						StallI_Sp;			//Setpoint of stall I
	_iq						StallI_IK_1;		//I-term(k-1) of PI stall I 
	Uint16					Carrier_Frq;		//F529
	_iq						DCBreak_Level;		//F522
	_iq						DCBreak_Time;		//F523
	_iq						DCBreak_Start_Frq;	//F524
	_iq						StartingF;			//F525 starting freq.
	_iq						StartingT;			//F526 starting time
	_iq						StopF;				//F527 stop freq.
	_iq						StallI_KP;			//Kp stall current function F424
	_iq						StallI_Ti;			//Ti stall current function F425
	_iq						Over_Volt_Stall;	//Over valtage stall level F407
	_iq						StallV_KP;			//Kp stall voltage function F408
	_iq						StallV_Ti;			//Ti stall voltage function F409
	_iq						DynamicBrkLevel;	//Dynamic brake level F411
	_iq						BckupCommand;		//Last target Accel/decel
}; 
extern struct ND2SYSTEM_REG ND2System_Reg;

struct ND2GAINREG{
	long					PWM_Gain;//F731
};

extern struct ND2GAINREG ND2Gain_Reg;  
 
#endif  

//===========================================================================
// End of file.
//===========================================================================
