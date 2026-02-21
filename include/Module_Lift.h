//added all chanel conversion
#ifndef MODULE_LIFT_H
#define MODULE_LIFT_H

//------------------------------------------------------------------------------------------------------------
struct Flag_Lift_bit{  					// bits   description 
   	Uint16	StopLift:1;						//=1 use decel time of lift	
} ;

union Flag_Lift {
	Uint16	all;
	struct Flag_Lift_bit	bit;
} ;  
//------------------------------------------------------------------------------------------------------------
struct LIFT_REG {
	_iq							InitLiftDec;			//F023 initial time lift decel
	_iq							LiftDec;				//F024 middle time lift decel
	_iq							FinalLiftDec;			//F025 final time lift decel
	union Flag_Lift				FLAG_LIFT;
}; 
extern struct LIFT_REG Lift_Reg; 
#endif  

//===========================================================================
// End of file.
//===========================================================================


















