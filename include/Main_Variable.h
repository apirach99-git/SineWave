#ifndef MAIN_VARIABLE_H
#define MAIN_VARIABLE_H

#define TsCntr1			1 				//t sampling counter 1	
#define IQ17_1			_IQ17(1.00)
#define Coe_I_Warning   _IQ17(15)
#define IQ17_100		_IQ17(100)
#define TsCntr30		30				//t sampling counter 30
#define MinusIQ			_IQ17(-1)		//multiplier minus *-1
#define VRate200		_IQ17(200)		//for capacity200V
#define VRate400		_IQ17(400)		//for capacity400V
#define SQRT2			_IQ17(1.414213562)
 
//Software version year2013
//#define SWV				520			//2013-05-20
//Software version year2023
//#define SWV             217         //2023-02-17
//------------------------------------------------------------------------------------------------------------ 
//!!!!Caution!!!! Dont change the order of IQ 	F255-F260,F003-F016
struct MAIN_VARIABLE {
	_iq			Time_RelayRC;	//F003 delay time R-charge C
	_iq			I_Rate;			//F004 rate current	
	_iq			DeadTime1;		//F007 deadtime1
	_iq			DeadTime2;		//F008 deadtime2
	_iq			FreqCut;		//F016 Freq. on PWM
	_iq			FAR_Hyst;		//Value of F255 FAR hysteresis
	_iq			FDT_Level;		//Value of F256 Level of FDT
	_iq			FDT_Hyst;		//Value of F257 FDT hysteresis
	_iq			FRef;			//Value of F258(Fbase) in IQ Format
	_iq			VRef;			//Value of F259(Voltage refereence) in IQ Format
	_iq			IRef;			//Value of F260(Current refereence) in IQ Format
	_iq			FMin;			//Value of F302(Fmin) in IQ Format
	_iq			FMax;			//Value of F303(Fmax) in IQ Format
	_iq			JumpF1;			//Value of F415 Jump Freq.1
	_iq			JumpF2;			//Value of F416 Jump Freq.2
	_iq			JumpF3;			//Value of F417 Jump Freq.3
	_iq			HystrsJumpF;	//Value of F418 Hysteresis of jump Freq. 
	_iq			FSet;			//Fset of syatem
	_iq			FCommandA;		//buffer Last Fcommand from main system
	_iq			FCommandB;		//buffer Last Fcommand from stallI
	_iq			FCommand;		//buffer Last Fcommand from stallV	
	_iq			PU_FMax;		//per unit f-max
	_iq			PU_FMin;		//per unit f-min
	_iq			FreqCut_PU;		//PU of F016
	_iq			FPU_Motor;		//F PU motor(F101,F117)
	_iq			VPU_Motor;		//V PU motor(F102,F118)
	_iq			IPU_Motor;		//I PU motor(F103,F119)
	_iq			IPU_NoLoad;		//I PU noload(F108,F124)
	_iq			I_base;
	_iq			V_base;
	_iq			F_base;
	_iq			I_base_PER_Vbase;
	_iq			Omega_base;
	Uint32		DelayReset;		//loop reset cpu
	_iq			FSlip;			//
}; 
extern struct MAIN_VARIABLE MainIQ_Variable; 

struct STALL_VOLTAGE {
	_iq			PI_Output;
	_iq			PI_Er;
}; 
extern struct STALL_VOLTAGE Parameter_stall_V;   



struct DataMotor{
	_iq		F_Motor;		//F101,F117
	_iq		V_Motor;		//F102,F118
	_iq		I_Motor;		//F103,F119
	_iq		Speed_Motor;	//F104,F110
	_iq		Power_Motor;	//F105,F121
	_iq		PF_Motor;		//F106,F122
	_iq		Poles_Motor;	//F107,F123
	_iq		I_NL_Motor;		//F108,F124
	_iq		Lr;				//F109,F125
	_iq		Rr;				//F110,F126
	_iq		Ls;				//F111,F127
	_iq		Rs;				//F112,F128
	_iq		M;				//F113,F129
	_iq		sigma;			//F114,F130
	_iq		J;				//F115,F131
	_iq		PulseEncoder;	//F116,F132
};

extern struct DataMotor DataMotor1;
extern struct DataMotor DataMotor2;
extern struct DataMotor *CurrentMotor;
#endif 


//===========================================================================
// End of file.
//===========================================================================























