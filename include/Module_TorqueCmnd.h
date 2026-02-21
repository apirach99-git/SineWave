
#ifndef MODULE_TORQUECMND_H
#define MODULE_TORQUECMND_H

//------------------------------------------------------------------------------------------------------------
struct FlagTrqCmnd_bit{   					// bits   description 
	Uint16 Test:1;
} ;

union FlagTrqCmnd {
	Uint16	all;
	struct FlagTrqCmnd_bit	bit;
} ;

//------------------------------------------------------------------------------------------------------------ 
//this iq variable F535-F537 can not change the order
struct TRQCMND_REG {
	union FlagTrqCmnd				FLAG_TRQCMND;
	_iq								TrqCommand;
	_iq								TrqdAcc;		//F535 Accel time torque command
	_iq								TrqDec;			//F536 Decel time torque command
	_iq								TrqKey;			//F537 Torque key command
	_iq								TrqSp;			//torque setpoint
}; 
extern struct TRQCMND_REG TrqCmnd_Reg; 

#endif  

//===========================================================================
// End of file.
//===========================================================================


















