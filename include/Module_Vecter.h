//added all chanel conversion
#ifndef MODULE_VECTER_H
#define MODULE_VECTER_H


//------------------------------------------------------------------------------------------------------------
//!!!Do not change the order of structure F510-F520
struct VECTOR_REG {
	long						Kn_SpeedLoop;			//F510
	long						Tn_SpeedLoop;			//F511
	long						Limit_Max_SpeedLoop;	//F512
	long						Limit_Min_SpeedLoop;	//F513
	long						Kn_CurrentLoop;			//F514
	long						Tn_CurrentLoop;			//F515
	long						Limit_Max_CurrentLoop;	//F516
	long						Limit_Min_CurrentLoop;	//F517 
	long						Id_Command;				//F518
	long						Max_Freq_out;			//F519
	long						Min_Freq_out;			//F520
	long						OmageToCurrent;
	long						CurrentToVoltage;
}; 
extern struct VECTOR_REG Vecter_Reg; 


struct HPWM_REG {
	_iq						FreqEna2Arm;			//F540
	_iq						FreqDis2Arm;			//F541
	_iq						ABS_Freq;

}; 
extern struct HPWM_REG Hpwm_Reg;  


#endif  

//===========================================================================
// End of file.
//===========================================================================


















