
#ifndef MODULE_FLYSTRT
#define MODULE_FLYSTRT

#define TaccelV 		0.2	//200ms before search
#define TaccelV1		0.4//1.0 //accel V after search
#define TdelayVout		1800	//1800ms delay before Decel F
#define TdelayAccVout	2000	//2000ms delay before accel Vout
#define cutI			_IQ17(0.05)	//% check I decrease (cutI/F004)
#define cutI1			_IQ17(0.008)	//% check I increase (cutI/F004)
#define GapV			_IQ17(0.0005)//gap V srch and Vtarget
//------------------------------------------------------------------------------------------------------------
struct FlagFlyStrt_bit{   				// bits   description 
	Uint16 FlyStage:3;					//stage bit of flying start
										//->stage0:Acceling Voutput(Tacc=200ms)
										//->stage1:Holdtime = 2sec-Tacc
										//->stage2:Deceling F to 0Hz or found Fmotor
										//->stage3:Completely finish flying start
	Uint16 FlyActive:1;					//bit = 1 is under flying
	Uint16 StepDwnFout:1;				//bit check step down fout before check I
	Uint16 IDecrease:1;					//bit check Irms decrease
	Uint16 LockFly:4;					//bit disable flyingstart temporary such as 
										//LU mode4 			bit 0
										//Finish stage5 	bit 1
} ;

union FlagFlyStrt {
	Uint16	all;
	struct FlagFlyStrt_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct FlyStrtTimer_bit{   				// bits   description 
	Uint16 T_Smp_AccVop:5;				//time sampling accel volt search	
	Uint16 T_Smp_DecFop:5;				//time sampling decel freq. search	
} ;

union FlyStrtTimer {
	Uint16	all;
	struct FlyStrtTimer_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------ 
//!!!Caution do not change line IQF416-F417
struct FLYINGSTART {
	union FlagFlyStrt		Flag_FlyStrt;
	union FlyStrtTimer		FlyStrt_Timer;
	_iq	 					VOutputSearch;		//percent Voutput searching
	_iq						LevelVSearch;		//F416 %of voltage search Vmotor
	_iq						TDecelSearch;		//F417 T of searching Freq.
	Uint16 					TDelayVout;			//time delay Vout constant
	_iq						IFly_K_1;			//I k-1
	_iq						FFly_K_1;			//F K-1
	_iq						VReffSrch;			//Volt refference when accel
}; 
extern struct FLYINGSTART FlyStrt_Reg; 

#endif  

//===========================================================================
// End of file.
//===========================================================================




















