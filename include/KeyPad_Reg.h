//for KeyPad4.c
//for KeyPad27.c
//for KeyPad33.c-->correct factor F002,F703=0x0801
//for KeyPad37.c
//revised factor F230 and F232
/*
	note : Factor definition      
    15 14 13 12 / 11 10 9 8 / 7 6 5 4 / 3 2 1 0                
	-  -  -  -  / -  -  - - / - - - - / - - - -

	bit13  : 1=-,0=+
	bit12  : 1; there are +,- value
	bit11  : 1=setting by digit,0=normal setting
	bit10  : 1=no setting when run,0=no effect running	
	bit9-8 : current position of DECIB
	bit7-6 : min position of DECIB
	bit5-4 : max position of DECIB
	bit3-2 : how many digit display; 
			 00->4digit
			 01->3digit
			 10->2digit
			 11->1digit
	bit1-0 : counting/step
*/
#ifndef KEYPAD_REG_H
#define KEYPAD_REG_H


#ifdef __cplusplus
extern "C" {
#endif
				
#define GrpNumber		9//num of group  

#define TimeLEDBlnk		500//time period blinking LED
#define TimeOuteCAN		200
//------------------------------------------------------------------------------------------------------------
struct e2prom_flag_bit{   					// bits   description 
   Uint16 WriteNewData:1;//flag for checking need to load defualt value or not
   Uint16 EEP_error:1;//set=1 when initial e2prom out-of range, clear after user save data
   Uint16 EEP_r_w_error:1;//read/write e2p error
   Uint16 EEP_correct:1;
   Uint16 Chck_Rng:1;
   Uint16 Cnt_Err:4;
   Uint16 Time_OutE2:1;
   Uint16 Loading_def:1;//loading default
   Uint16 Writting:1;	//=1e2prom is writing
} ;

union e2prom_flag {
	Uint16	all;
	struct e2prom_flag_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct e2prom_flag1_bit{   					// bits   description 
   Uint16 W_Status:4;						//status I2c when writing
	Uint16 R_Status:4;						//status I2c when reading
} ;

union e2prom_flag1 {
	Uint16	all;
	struct e2prom_flag1_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct buff_cdrr_bit{   					// bits   description 
    Uint16 Buff_CDRR_L:8;//flag for checking need to load defualt value or not
	Uint16 Buff_CDRR_H:8;//flag for checking need to load defualt value or not
} ;

union buff_cdrr {
	Uint16	all;
	struct buff_cdrr_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct e2prom_adrs_bit{   					// bits   description    
   Uint16 Adrs_L:8;
   Uint16 Adrs_H:8;//buffer for high of adrs e2prom
} ;

union e2prom_adrs {
	Uint16	all;
	struct e2prom_adrs_bit	bit;
} ;
//------------------------------------------------------------------------------------------------------------
struct generalflag_bit{   					// bits   description
   Uint16 MinMainMode:1;//0 
   Uint16 StateDig:3; 	//3-1
   Uint16 LockSetting:1;//4	   //
   Uint16 new_fault:1;	//new fault come into the system   
   Uint16 End_InitKeyPAd:1;
   Uint16 FKey_active:1;//bit=1, when F604/F304 = 0,5-8,use in normal mode
   Uint16 ConnectKeyPad:1;	//set after connected with keypad
   Uint16 BuffBRP:3;	//0 : F009=1->32us
						//1 : F009=2->16us
						//2 : F009=3->8us
						//3 : F009=4->4us
   Uint16 Analog_Src:1;																	
   Uint16 ScanNode:1;	//check which node of CAN will be sent the data
} ;

union General_flag {
	Uint16	all;
	struct generalflag_bit	bit;
}  ;
//------------------------------------------------------------------------------------------------------------
struct Trigger_bit_bit{   					// bits   description
   Uint16 MBX1Tx_Trg:1;    	
   Uint16 Speed_UpDwn_Trg:1;   	//trigger for up speed
   Uint16 RunCntr_Trg:1;		//bit time for counter setting 
   Uint16 open_loop_cntr:1; //disable counter loop stop@min/max         
   Uint16 Next_digt:1;
   Uint16 Blnk_Trg:1;
   Uint16 E2Prom_Trg:1;//for delay e2prom
   Uint16 Norm_Set_Trg:1;//for delay normal mode in setting
   Uint16 TimeOutE2p_Trg:1;//start timer for time out e2prom
   //-----------Gus------------
   Uint16 Data_Set_Trg:1;
   Uint16 Mode_Set_Trg:1;
   Uint16 Button_Set_Trg:1;
   //-----------Gus------------
} ;

union Trigger_bit {
	Uint16	all;
	struct Trigger_bit_bit	bit;
}  ;
//------------------------------------------------------------------
struct Key_flag_bit {   					// bits   description
    Uint16 state_key1:1;			//bit0
	Uint16 key_prog:1;				//bit1 
	Uint16 key_data:1;				//bit2 
	Uint16 key_up:1;				//bit3 
	Uint16 key_dwn:1;				//bit4 
	Uint16 state_speed_updwn:3;		//bit7-5
	Uint16 key_run:1;				//bit8 
	Uint16 key_stop:1;				//bit9
	Uint16 key_shift:1;				//bit10
	Uint16 first_runstop:1;		    //bit11
	Uint16 enable_runstop:1;		//bit12 
} ;

union Key_flag {
	Uint16	all;
	struct	Key_flag_bit bit;
} ;
//------------------------------------------------------------------
struct Pntr_mode_bit {
	Uint16	PntrQck_mode:12;//pointer to point quick mode
	Uint16	PntrGrp:4;//pointer to point quick mode
	Uint16	PntrSubFunc:8;//pointer for sub function
} ;

union Pntr_mode {
	Uint32	all;
	struct	Pntr_mode_bit	bit;
} ;
//------------------------------------------------------------------
struct Timer0_bit {
	Uint16	Time_blink:12;//pointer to point quink mode
	Uint16  Time_WRE2Prom:4;//timer delay for writing e2prom
} ;

union Timer0 {
	Uint32	all;
	struct	Timer0_bit	bit;
} ;
//------------------------------------------------------------------
struct Disp_flag_bit {
	Uint16	DispPreBuf:1;		//pointer to point quink mode
	Uint16  DispPSW:1;			//display password
	Uint16	NoBlink:1;			//if this bit=1 display will not blink
	Uint16  Fwd_Rev:1;			//direction of motor
	Uint16  GeneralBlk:1;		//on-off LED keypad
	Uint16  SignNormDisp:1;		//if the displayed data is minus, this bit will set
} ;

union Disp_flag {
	Uint16	all;
	struct	Disp_flag_bit	bit;
} ;
//------------------------------------------------------------------
struct Disp_flag1_bit {
	Uint16  Bckup_DispSel:4;	//backup display data selection
	Uint16  BufDispNorm:4;		//pointer display in normal mode
	Uint16  BckBufDispNorm:4;	//backup pointer display in normal mode
} ;

union Disp_flag1 {
	Uint16	all;
	struct	Disp_flag1_bit	bit;
} ; 
//------------------------------------------------------------------
struct Disp_Tab_flag_bit {
	Uint16	Disp_Table:6;//display hi at max setting
	Uint16	Disp_fault:1;//display fault set in setting mode when see the fault memory
} ;

union Disp_Tab_flag {
	Uint16	all;
	struct	Disp_Tab_flag_bit	bit;
} ;
//------------------------------------------------------------------
struct Mode_flag_bit {  
	Uint16 setting_mode:1;//bit0   					
   	Uint16 mode_state:2;//bit1-0  
	Uint16	mode_st3_2:1;//first state bit after setting(setting)
    Uint16	PrePntr:5;//prepointer0-29 to map with PntrQck_mode    
	Uint16 norm_state:1;//normal mode flag
//  Uint16	PreDECIB:2;		
} ;

union Mode_flag {
	Uint16	all;
	struct	Mode_flag_bit bit;
} ;
//------------------------------------------------------------------
struct BufferDataL_bit { 
	Uint16 fmode:2;		//bit0-1
	Uint16 decib:2;		//bit2-3
	Uint16 resodisp:3;	//bit4-6
	Uint16 sign:1;		//bit7
	Uint16 unit:3;		//bit8-10
	Uint16 nop:1;		//bit11
	Uint16 run:1;		//bit12
	Uint16 vrcom:1;		//bit13
	Uint16 fault:1;		//bit14
	Uint16 shpass:1;	//bit15
	Uint16 data1:16;	//bit16-31
} ;

union BufferDataL {
	Uint32	all;
	struct	BufferDataL_bit	bit;
} ;
//------------------------------------------------------------------
struct BufferDataH_bit { 
	Uint16 data2:16;		//bit0-15 ->parameter of sub function
	Uint16 Data_Mode:1;		//bit16	  ->data function,normal mode
	Uint16 Unit_SubFN:4;	//bit17-20->unit of sub function
	Uint16 reserve:1;		//bit21	  ->no used
	Uint16 Run_Comm:1;		//bit22	  ->run command remote/local
	Uint16 Dir_M:1;			//bit23	  ->motor direction	REV,FWD
	Uint16 Status_InvA:4;   //bit27-24->status of inverter
							//DC,FLY,FER,ACC,DEC,STD,READY,NOT-R
	Uint16 Status_InvB:3;	//bit30-28->statusA of inverter
							//W-OC,W-OL
	Uint16 Status_Run:1;	//bit31   ->Run or Stop
} ;

union BufferDataH {
	Uint32	all;
	struct	BufferDataH_bit	bit;
} ;

struct BufferButton_bit {
    uint8_t BuffSW_PROG     :1;
    uint8_t BuffSW_UP       :1;
    uint8_t BuffSW_DWN      :1;
    uint8_t BuffSW_DATA     :1;
    uint8_t BuffSW_RUN      :1;
    uint8_t BuffSW_SHIFT    :1;
    uint8_t BuffSW_STOP     :1;
};

union BufferButton {
    uint8_t all;
    struct BufferButton_bit bit;
};
//------------------------------------------------------------------
struct KEYPAD_REG{
	union Trigger_bit		TRIGGER_BIT;      
	union Key_flag     		KEY_FLAG;      
	union Mode_flag     	MODE_FLAG;      
	union BufferDataL		BUFFERDATAL;  
	union BufferDataH		BUFFERDATAH; 
	union Pntr_mode			PNTR_MODE;     
	union Disp_flag			DISP_FLAG;	
	union Disp_Tab_flag 	DISP_TAB; 
	union Timer0			TIMER0;
	union General_flag		GENERAL_FLAG;
	union e2prom_flag		E2PROM_FLAG;
	union e2prom_flag1		E2PROM_FLAG1;	//initial 0x0040
	union e2prom_adrs		E2PROM_ADRS;
	union buff_cdrr			E2PROM_BUFF_CDRR;
	Uint16 					TimerBlinkLED;
	_iq 					BuffIQNorm;		//buffer IQ in normal mode
	_iq						LineSpdCoef;	//F522 line speed co.
	Uint16					TimeoutKeyPad;	//time-out ecan
	_iq 					BckDispNorm;	//backup data to display in normal mode
	_iq						ToaDispNorm;	//F013 filter display
	_iq						Sp_Norm;		//Setpoint from up/down normal mode
	union Disp_flag1		DISP_FLAG1;
	Uint16					DelayKeypad;	//Waiting for check hard reset
	union BufferButton      BUFFERBUTTON;
};
extern struct KEYPAD_REG KeyPad_reg;

#ifdef __cplusplus
}
#endif

#endif  // 

//===========================================================================
// End of file.
//===========================================================================


