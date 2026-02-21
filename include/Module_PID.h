//added all chanel conversion
#ifndef MODULE_PID_H
#define MODULE_PID_H

#define ts_accdecT		_IQ17(0.03)		//global accel/decel time
//#define TsPID			1 				//t sampling PID max=10		
//#define TsStep			30				//t sampling limit time and Accel/Decel for IQ17 resolution
#define TsPIDMain		_IQ17(0.001)
//------------------------------------------------------------------------------------------------------------
struct FlagPID_bit{   					// bits   description 
   Uint16 PID_En:1;						//PID enable
   Uint16 Td_TiIQ15:1;					//if the result of Td/Ti is IQ15, this bit will be set
   Uint16 Td_TsIQ15:1;					//if the result of Td/Ts is IQ15, this bit will be set   	
   Uint16 Dec_sign:1;					//under dec. when SP change sign, this bit will be set
   Uint16 Chk_AccDec:1;					//if acc/dec is performed this bit will be set and clear in systctrl module
} ;

union FlagPID {
	Uint16	all;
	struct FlagPID_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct TimerPID_bit{   					// bits   description 
   Uint16 T_Smp:4;						//timer for sampling of PID
   Uint16 T_Smp_LimT:5;					//buffer timer for count limit time sampling	
   Uint16 T_Smp_AccDecTSP:5;			//buffer timer for count Accel/Decel Setpoint time sampling	
} ;

union TimerPID {
	Uint16	all;
	struct TimerPID_bit	bit;
} ;  
//------------------------------------------------------------------------------------------------------------
//this iq variable F605-F611,F613-F620 can not change the order
struct PID_REG {
	union FlagPID				FLAG_PID;	
	union TimerPID				TIMER_PID;	
	_iq							PID_TAccelSp;		//F605 PID accel time Setpoint
	_iq							PID_TDecelSp;		//F606 PID decel time Setpoint
	_iq							Sp_gain;			//F607 Setpoint gain 
	_iq							Fb_max;				//F609 Percent feedback max limit 
	_iq							Fb_min;				//F610 Percent feedback min limit 
	_iq							Fb_gain;			//F611 Percent feedback gain  
	_iq							Kp;					//F613 Kp gain 
	_iq							Ti;					//F614 Integral time 
	_iq							Td;					//F615 Derivative time 
	_iq							PID_max;			//F617 upper limit PID output
	_iq							PID_min;			//F618 lower limit PID output
	_iq							PID_Lim_T;			//F619 Limit time
	_iq							PID_Freq;			//F620 Percent PID Freq. 	
	_iq							PU_SetpointK;		//per unit of setpoint(k) signal	
	_iq							PU_FeedbackK;		//per unit of feedbac(k) signal	
	_iq							PU_FeedbackK_1;	 	//per unit of feedbac k-1 signal 
	_iq							PU_ErrorK;			//per unit of error(k) PID 
	_iq							PU_ErrorK_1;		//per unit of error(k-1) PID  	
	_iq							I_termK_1;			//I_term(k-1)
	_iq 						PU_OutputK;			//per unit of output(k) PID before multiply by Fbase	
	_iq 						PU_SatErK;			//per unit of saturate error			
	_iq							PU_OutputPreBuff;	//per unit of outputPID before check limit(saturation) and acc/dec
	_iq							PID_Dyna_Max;		//PID dynamic range max follow limit time
	_iq							PID_Dyna_Min;		//PID dynamic range min follow limit time		
	_iq							PID_SpTemp;			//PID setpoint before accel/decel
	_iq							P_TermK;			//Last P(k)-term
	_iq							I_TermK;			//Last I(k)-term
	_iq							D_TermK;			//Last D(k)-term
}; 
extern struct PID_REG PID_Reg; 

extern _iq arrPID_MulF[15];
#endif  

//===========================================================================
// End of file.
//===========================================================================


















