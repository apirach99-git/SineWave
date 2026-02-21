
#ifndef MODULE_FREQ
#define MODULE_FREQ

//------------------------------------------------------------------------------------------------------------
struct FlagFreq_bit{   					// bits   description 
   Uint16 Jogging:2;					//jog is running
   Uint16 CtrlPttnStg:3;				//control pattern stage 1-7
   Uint16 FinishMonoCyc:1;				//set after F353=3 and finish state 7
   Uint16 AutoStopPttn:1;				//Auto stop in mono cycle pattern

} ;

union FlagFreq {
	Uint16	all;
	struct FlagFreq_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct TimerFreq_bit{  					// bits   description 
   Uint16 T_Smp:4;						//timer for sampling of reugular freq.
   Uint16 T_Smpstg:7;					//timer for sampling pattern stage mode 100ms
} ;

union TimerFreq {
	Uint16	all;
	struct TimerFreq_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------

//this iq variable F304-F361 can not change the order
struct Freq_REG {
	union TimerFreq				TIMER_FREQ;
	union FlagFreq				FLAG_FREQ;	
	_iq							FJogRev;		//F304 freq. jog rev
	_iq							FJogFwd;		//F305 freq. jog fwd
	_iq							AccTJog;		//F306 accel time jog
	_iq							DecTJog;		//F307 decel time jog
	_iq							F_Key;			//F308 freq. keypad
	_iq							InitAcc;		//F341 initial time accel s-curve 	
	_iq							FinalAcc;		//F342 final time accel s-curve
	_iq							InitDec;		//F343 initial time decel s-curve
	_iq							FinalDec;		//F344 final time decel s-curve 
	_iq							AccT1;			//F345 accel time1
	_iq							DecT1;			//F346 decel time1	
	_iq							AccT2;			//F347 accel time2
	_iq							DecT2;			//F348 decel time2
	_iq							AccT3;			//F349 accel time3
	_iq							DecT3;			//F350 decel time3	
	_iq							AccT4;			//F351 accel time4
	_iq							DecT4;			//F352 decel time4
	_iq							StageT1;		//F355 stage time1
	_iq							StageT2;		//F356 stage time2
	_iq							StageT3;		//F357 stage time3
	_iq							StageT4;		//F358 stage time4
	_iq							StageT5;		//F359 stage time5
	_iq							StageT6;		//F360 stage time6
	_iq							StageT7;		//F361 stage time7
	_iq							Freq_SpPU;		//per unit of FSET in grp3
	_iq							MainAcc;		//main accel of regular Freq
	_iq							MainDec;		//main dec of regular Freq
	float32						TCtrlPttn;		//timer stage control
}; 
extern struct Freq_REG Freq_Reg; 

extern _iq arrFreq_MulF[15];					//multi F,F344-F325
extern _iq arr_AccStg[7];						//array for accel stage
extern _iq arr_DecStg[7];						//array for decel stage
#endif  

//===========================================================================
// End of file.
//===========================================================================




















