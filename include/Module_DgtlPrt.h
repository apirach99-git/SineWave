
#ifndef MODULE_DGTLPRT_H
#define MODULE_DGTLPRT_H

#define TsUpdateDataDIN			10 				//t sampling update final data of port	
#define TsUpdatePrtDIN			1				//t sampling check status port

#define Active					1
#define Inactive				0

#define DelayDINFstCnt			100				//delay*10ms fisrt count of Up/Down DIN(must be<127)
#define DelayDINCnt				25				//delay*10ms fisrt count of Up/Down DIN(must be<127)
//------------------------------------------------------------------------------------------------------------
struct FlagDgtlPrt_bit{   					// bits   description 
   Uint16 Buff_DOUT30:1;					//buffer data output terminal30
   Uint16 Buff_DOUT31:1;					//buffer data output terminal31
   Uint16 Buff_DOUT32:1;					//buffer data output terminal32  	
   Uint16 EnbDINDrvEnb:1;					//bit use DIN drive enable(DIN=0)
   Uint16 EnbDINACDC:1;						//bit use AC/DC by DIN(DIN=16,17)
   Uint16 UpDwnDIN:1;						//bit identify the system is up/down by DIN
   Uint16 SttUpDwnDIN:1;					//status 0 is delay 1s before count,status1 delay100ms
   Uint16 StateFAR:1;						//status FAR
   Uint16 StateFDT:1;						//status FDT
   Uint16 StDOUTFault:1;					//Hold state DOUT Fault
   Uint16 FirstCheck:1;                      //*** Tsu Add Mon.4.12.60
   Uint16 EnaFlying:1;                      //*** Tsu Add Mon.4.12.60
} ;

union FlagDgtlPrt {
	Uint16	all;
	struct FlagDgtlPrt_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct TimerDgtlPrt_bit{   					// bits   description 
   Uint16 T_SmpData:4;						//timer for sampling of update final data port   	
   Uint16 T_SmpPort:4;						//timer for sampling of checking port
   Uint16 T_CntUpDwn:7;						//timer for running data up/down DIN
} ;

union TimerDgtlPrt {
	Uint16	all;
	struct TimerDgtlPrt_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------ 
struct DataPortDIN_bit{   					// bits   description 
   Uint16 DataGPIO_DIN:7;					//final data GPIO DIN 
   Uint16 BuffGPIO_DIN:7;					//buffer from port before debounce
} ;

union DataPortDIN {
	Uint16	all;
	struct DataPortDIN_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------  
struct DataL_DIN_bit{   					// bits   description 
   Uint16 Drv_En:1;						//0 = Drive enable
   Uint16 Fwd_Com:1;					//1 = Forward command
   Uint16 Rev_Com:1;					//2 = Reverse command
   Uint16 Coast_stop:1;					//3 = Coast to stop command
   Uint16 Jog_Rev:1;					//4 = Jog reverse command
   Uint16 Jog_Fwd:1;					//5 = Jog forward command
   Uint16 DC_Brk:1;						//6 = DC braking command
   Uint16 Rev_Fwd:1;					//7 = Rev/Fwd command
   Uint16 Up:1;							//8 = Up command
   Uint16 Down:1;						//9 = Down command
   Uint16 Mul_FrqB0:1;					//10 = Multi Freq bit0
   Uint16 Mul_FrqB1:1;					//11 = Multi Freq bit1
   Uint16 Mul_FrqB2:1;					//12 = Multi Freq bit2
   Uint16 Mul_FrqB3:1;					//13 = Multi Freq bit3
   Uint16 Ext_fault:1;					//14 = External fault 
   Uint16 Alrm_reset:1;					//15 = Alarm reset
};

union DataL_DIN {
	Uint16	all;
	struct DataL_DIN_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------ 
struct DataH_DIN_bit{ 
   Uint16 AccDecTB0:1;					//16 = Accel/Deccel time select bit0
   Uint16 AccDecTB1:1;					//17 = Accel/Deccel time select bit1
   Uint16 Curr_IP:1;					//18 = Current-input signal select
   Uint16 Wire_stp:1;					//19 = 3-wire stop command
   Uint16 SelM2M1:1;					//20 = Select M2=active/M1=inactive
};

union DataH_DIN {
	Uint16	all;
	struct DataH_DIN_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------ 
struct DGTLPRT_REG {
	union FlagDgtlPrt				FLAG_DGTLPRT;	//data of each port
	union TimerDgtlPrt				TMR_DGTLPRT;	//timer
	union DataL_DIN					DataLDIN;		//final data low byte follow setting F202-F208
	union DataH_DIN					DataHDIN;		//final data high byte follow setting F202-F208
	union DataPortDIN				DataPrtDIN;		//information from port	
}; 
extern struct DGTLPRT_REG DgtlPrt_Reg; //Digital port register

struct KEYPADData{
	Uint16							BuffVal;		//buffer value for setting up/down by DIN
	Uint16 							BuffFact;		//buffer factor for setting up/down by DIN
	Uint16							BuffDEC;		//buffer DEC for setting up/down by DIN
	Uint16							BuffSign;		//buffer sign for setting up/down by DIN
	_iq                             BuffSp_Norm;
    Uint16                          BuffValMax;        //buffer value for setting up/down by DIN
    Uint16                          BuffFactMax;       //buffer factor for setting up/down by DIN
    Uint16                          BuffDEC_Max;        //buffer DEC for setting up/down by DIN
    Uint16                          BuffSign_Max;       //buffer sign for setting up/down by DIN
}; 

extern struct KEYPADData BuffDgtPr;	
extern struct KEYPADData *CurrentCntr;	

#endif  

//===========================================================================
// End of file.
//===========================================================================
