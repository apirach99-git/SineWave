//added all chanel conversion
#ifndef MODULE_ADC_H
#define MODULE_ADC_H

#define RangeAIN12 	 _IQ17(10)//10
#define offsetA12	 56//56
#define scaleA12   0.991736
#define CmpstAIN12Upper  	 _IQ17(0.0025)//0.0025
#define CmpstAIN12Lower	_IQ17(13) //13

#define RangeAIN2 _IQ17(20)
#define offsetA2  20
#define CmpstAIN2Upper _IQ17(0.00488)
#define CmpstAIN2Lower	_IQ17(7.5)

#define RangeAIN3 _IQ17(20)
#define offsetA3  80
#define scaleA3  1.0196
#define compA3   _IQ17(0.0055)

#define ADC_usDELAY  5000L

#define OffsetA2D 35

#define toaVRKey	 _IQ17(200)//Toa of VR key in LPF
#define MinResIQ17		_IQ17(0.000008)
#define IQ17_SCALE_F   131072.0f   // 2^17
#define IQ16_SCALE_F   065535.0f   // 2^16

#define MBackB1	    0.004884004//m of convert a2dB1 back to voltage
#define OffBackB1	_IQ17(-10)//offset of convert a2dB1 back to voltage

#define AIN_12	0
#define AIN_A2	1
#define AIN_A3	2 

#define A2dA0Buff	0 		//P23:Current U
#define A2dB0Buff	1 		//P27:AIN A12
#define A2dA1Buff	2  		//P22:Current V
#define A2dB1Buff	3 		//P28:AIN A2
#define A2dA2Buff	4 		//P21:Current W
#define A2dB2Buff	5  		//P29:AIN A3
#define A2dA3Buff	6 		//P20:VDC
#define A2dB3Buff	7 		//P30
#define A2dA4Buff	8  		//P19
#define A2dB4Buff	9 		//P31
#define A2dA5Buff	10		//P18:Temperature1 
#define A2dB5Buff	11 		//P32:Temperature2 
#define A2dA6Buff	12 		//P17:Temperature3 
#define A2dB6Buff	13		//P33:Temperature4 
#define A2dA7Buff	14		//P16 
#define A2dB7Buff	15 		//P34

#define comp_scl  1.00005

//------------------------------------------------------------------------------------------------------------
struct General_ADCFlag_bit{   					// bits   description 
   Uint16 Trg_ReadADC:1;						//interval of redaing ADC
   Uint16 AIN2_Chnl:1;
} ;

union General_ADCFlag {
	Uint16	all;
	struct General_ADCFlag_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
//!!!!Caution!!!! Dont change the order of IQ F223-F240
struct ANALOG_REG{
	union General_ADCFlag		GENERAL_ADCFLAG;  
	Uint16 						Timer_ReadADC; 
	_iq							VRKeypad;	//A2D VR from keypad  
	_iq 						A2dB0;
	_iq 						A2dB1;
	_iq 						A2dB2; 
	_iq							PU_B0;//Analog input A12
	_iq							PU_B1;//Analog input A2
	_iq							PU_B2;//Analog input A3
	_iq							FilterAIN12;	//F223 filter time AIN12
	_iq							X1_AIN12;		//F224 X1 of AIN12
	_iq							Y1_AIN12;		//F225 Y1 of AIN12
	_iq							X2_AIN12;		//F226 X2 of AIN12
	_iq							Y2_AIN12;		//F227 Y2 of AIN12
	_iq							DB_AIN12;		//F228 Deadband of AIN12
	_iq							FilterAINA2;	//F229 filter time A2
	_iq							X1_A2;			//F230 X1 of A2
	_iq							Y1_A2;			//F231 Y1 of A2
	_iq							X2_A2;			//F232 X2 of A2
	_iq							Y2_A2;			//F233 Y2 of A2
	_iq							DB_A2;			//F234 Deadband of A2
	_iq							FilterAINA3;	//F235 filter time A3
	_iq							X1_A3;			//F236 X1 of A3
	_iq							Y1_A3;			//F237 Y1 of A3
	_iq							X2_A3;			//F238 X2 of A3
	_iq							Y2_A3;			//F239 Y2 of A3
	_iq							DB_A3;			//F240 Deadband of A3
	//Gus
	_iq                         BuffInputA12; //External Volume
	_iq                         DeltaInputA12;
};
extern struct ANALOG_REG Analog_reg;
#endif  // 

//===========================================================================
// End of file.
//===========================================================================
