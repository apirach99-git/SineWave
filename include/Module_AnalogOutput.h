
#ifndef MODULE_ANALOGOUTPUT_H
#define MODULE_ANALOGOUTPUT_H

//------------------------------------------------------------------------------------------------------------ 
//this iq variable F244-F254 can not change the order
struct AOUT_REG {
	_iq							Buff_FMA;		//buffer data for FMA
	_iq							Buff_FMALPF;	//backup for last data LPF of FMA
	_iq							FMA_filter;		//F243 filter time of FMA
	_iq							FMA_X1;			//F244 X1 of FMA
	_iq							FMA_Y1;			//F245 Y1 of FMA
	_iq							FMA_X2;			//F246 X2 of FMA
	_iq							FMA_Y2;			//F247 Y1 of FMA
	_iq							FMA_DB;			//F248 deadband of FMA
	_iq							AOUT2_filter;	//F249 filter time of AOUT2
	_iq							AOUT2_X1;		//F250 X1 of AOUT2
	_iq							AOUT2_Y1;		//F251 Y1 of AOUT2
	_iq							AOUT2_X2;		//F252 X2 of AOUT2
	_iq							AOUT2_Y2;		//F253 Y1 of AOUT2
	_iq							AOUT2_DB;		//F254 deadband of AOUT2 
	_iq							Buff_AOUT2;		//buffer data for AOUT2
	_iq							Buff_AOUT2LPF;	//backup for last data LPF of AOUT2
}; 
extern struct AOUT_REG AOUT_Reg; 

#endif  

//===========================================================================
// End of file.
//===========================================================================


















