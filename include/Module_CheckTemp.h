
#ifndef MODULE_CHECK_TEMP_H
#define MODULE_CHECK_TEMP_H

#define DisableTemp		0//Disable channel temp sensor
#define	EnableNormal	1//temp sensor enable normal circuit
#define	EnableShort		2//temp sensor enable short circuit
#define	EnableOpen		3//temp sensor enable open circuit

#define MinA2DTemp  50//Min.value of A2D
#define MaxA2DTemp  4000//Max.value of A2D

//#define Cons_Temp	_IQ17(100)//time constant for temp sensor
#define Cons_Temp   _IQ17(300)//time constant for temp sensor
#define Ts_Temp		_IQ17(1000)//Tsampling
//------------------------------------------------------------------------------------------------------------
struct Timer_TempFlag_bit{   		// bits   description 
   Uint16 TimerTemp:12;				//timer 1sec for reading temperature
} ;

union Timer_TempFlag {
	Uint16	all;
	struct Timer_TempFlag_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct General_TempFlag_bit{   		// bits   description 
   Uint16 ChkCircuit:2;				//bit short/open/normal circuit temp sensor	
   Uint16 FirstTemp:1;				//bit check first time load A2D to avoid LPF rise time
} ;

union General_TempFlag {
	Uint16	all;
	struct General_TempFlag_bit	bit;
} ;  
//------------------------------------------------------------------------------------------------------------
struct CircuitTemp_bit{   		// bits   description 
	Uint16 Channel1:2;			//bit short/open/normal circuit temp sensor1	
   	Uint16 Channel2:2;			//bit short/open/normal circuit temp sensor2
	Uint16 Channel3:2;			//bit short/open/normal circuit temp sensor3
	Uint16 Channel4:2;			//bit short/open/normal circuit temp sensor4
} ;

union CircuitTempflag {
	Uint16	all;
	struct CircuitTemp_bit	bit;
} ;   
//------------------------------------------------------------------------------------------------------------
struct TEMPERATURE_REG{
	union Timer_TempFlag		TIMER_TEMPFLAG;  
	union General_TempFlag		GENERAL_TEMPFLAG;  
	union CircuitTempflag		CircuitTemp;		//Status of circuit
};
extern struct TEMPERATURE_REG Temperature_reg;
extern _iq arTemperature[];
extern _iq arA2DTemperature[];

#endif  // 

//===========================================================================
// End of file.
//===========================================================================
	





