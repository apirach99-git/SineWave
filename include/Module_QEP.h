//added all chanel conversion
#ifndef MODULE_QEP_H
#define MODULE_QEP_H

#define t_sampqep	550000		//Tsampling < (60/RPMmax)
#define max_cntrqep	4096		//max cntr = 4*pulse of encoder per rev.
#define line_rev	1024
#define CapConst 0.0052416			

#define toa_qep		1.5
#define TsQEP		0.0055

#define cntr_dirt 10				//1<=cntr_dirt<=10

#define fsamp _IQ17(181.82)		//1/(t_sampqep*10ns)
#define GapRPM _IQ17(2) 
//------------------------------------------------------------------------------------------------------------
struct General_QEPFlag_bit{    // bits   description 
   Uint16 data_qdf:1;		   //last direction
   Uint16 buffer_qdf:1;		   //buffer direction
   Uint16 DirStable:1;		   //Direction is already stable
   Uint16 FrstChkDirt:1;		//First check direction after reset
   Uint16 UTOActive:1;			//Speed by UTO Section
} ;

union General_QEPFlag {
	Uint16	all;
	struct General_QEPFlag_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct QEP_REG {
	union General_QEPFlag		GENERAL_QEPFLAG;      
	Uint32                     	New_Cnt;   //new contr from latch register
	Uint32                     	Old_Cnt;   //new contr from latch register
	Uint32						DeltaCnt;
	_iq							RPM_UTO;	//absolute RPM UTO
	_iq							RPM_CAP;	//absolute RPM CAP
	_iq							TimeScaleUTO;	//Timescale for Unit timeout Calcution
	float32						TimeScaleCap;	//Timescale for Capture Calcution
	_iq							PU_RPM;			//per unit RPM
	_iq							ABSRPM;		//Absolute RPM after switch CAP/UTO
}; 
extern struct QEP_REG QEP_Reg;//QEP_Reg={0,0,0,0,0,_IQ17(0),_IQ17(0)}

#endif  

//===========================================================================
// End of file.
//===========================================================================























