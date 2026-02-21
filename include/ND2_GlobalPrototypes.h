
#ifndef ND2_GLOBALPROTOTYPES_H
#define ND2_GLOBALPROTOTYPES_H


#ifdef __cplusplus
extern "C" {
#endif
//Function boot strap
extern void ClrBtStrp(void);		//Clear data of bootstrap when stop  state
extern void Function_BtStrp(void);	//Operation of bootstrap function  

//Keypad
extern Uint16 ReadE2prom(Uint16 AdrsH, Uint16 AdrsL);
extern void Chk_TimeOutE2P(void);
extern void WriteE2prom(Uint16 AdrsH, Uint16 AdrsL, Uint16 Data);
extern void Chk_Timeout_Read(Uint16 AdrsH, Uint16 AdrsL);
extern void Run_AdrsGRP(void);
extern void WriteUserE2prom(Uint16 Grp, Uint16 Sub, Uint16 ValE2, Uint16 FactE2);//write new data setting with group/subfunction to e2prom
extern void LoadDefaultE2(Uint16 StartGrp);
extern void Correct_Data(void);
extern void DispTab(Uint16 datatab);
extern void CheckKeyIn(void);
extern void Up_speedUpDown(void);
extern void CheckMode(void);
extern void ClearKey(void);
extern void DataDisp(void);
extern void SelectFunct(void);
extern void init_keypad(void);
extern void SettingSubFNC(void);
extern void ClrStSetting(void);//clear state of setting
extern void ClrStSetting1(void);
extern void RunCntrSetting(Uint16 max_val, Uint16 min_val, Uint16 factor_val);//max value,min value,factor
extern void RunSubGrp(Uint16 max_grp, Uint16 min_grp);
extern Uint16 run_updwncntr(Uint16 max_valueA, Uint16 min_valueA, Uint16 actuator, Uint16 step_count);
extern void LoadDefault(Uint16 StartGrp1);
extern void UpDigit(void);
extern void RunPassword(void);
extern void Operation_GrpPrd_Srvc(void);
extern void Update_Range(Uint16 range_value);
extern void Check_Range(void);
extern void Normal_mode(void);//run normal mode i/p=key up/down
extern void Run_SpectGrpFreq(void);//manage run keypad in group3
extern void Run_SpectGrpSrvc(void);
extern void Arrng_DataDisp(void);
extern void initE2Prom(void);
extern void interrupt_timer(void);
extern void ChkDEC(Uint16 x1,Uint16 y1, Uint16 DECDir);
extern void Min_Value(Uint16 min_val1, Uint16 factor_val1, Uint16 load_min1);
extern void Max_Value(Uint16 max_val1, Uint16 factor_val1,Uint16 load_max1);
extern void Update_StInvrt();
extern Uint16 HideSubFnct(Uint16 SubGrp);//hide some sub-function in small model
extern void Record_fault();
extern void ConvIQ(_iq data, Uint16 maxdec,Uint16 mindec);
extern void ConvAllKeyPad2IQ(Uint16 grp, Uint16 sub_grp);//converse all setting that have DEC 													 //to IQ, argument for pointing grp&su grp
extern void DisplayNormal(void);	//calculate data display, LPF for F013 in normal mode
extern void DspDataFnt(void); 

//ADC
extern void ADC_Setup(void);
extern void interrupt_timerADC(void);
extern void interrupt_a2d(void);
extern void FunctLPF(void);
extern _iq LnrScl(_iq input,_iq x1,_iq x2,_iq y1,_iq y2,_iq db);
extern void FunctLnrScl(void);
extern _iq ConvStg(Uint16 value1, Uint16 factor1);
extern _iq ChkRngF(_iq input, _iq pufmax, _iq pufmin);//check limit of %signal:PUinput,pumax,pumin
extern _iq LowPassFilter(_iq input, _iq inputn_1, _iq toa, _iq ts);
extern _iq IQmpyMin(_iq A,_iq B);
extern void FunctFSet(void);
extern void ChkVRKey(void);	//check and update data VR of keypad
extern void ClrAvrgA2D(void); 

//AOUT
extern void AnalogOutSetup(void);	//Setup GPIO and config. PWM4A,PWM5A for analog output
extern void Update_AOUT(void);		//update data to pwm module
extern void DtoA(void);      //update data to pwm module
extern _iq SelectAOUT(Uint16 selector);//selector for aout source
extern _iq AOUT_Scaling(_iq input,_iq x1,_iq y1,_iq x2,_iq y2);//linear scaling 
extern _iq AOUT_DB(_iq input1,_iq deadband);//if OP is <= DB, then OP = 0 and %DB = 100%,DB=10V 

//Function service backup
extern void Uploading(void);			//Door to rountine read a2d
extern void Downloading(void);			//Door to rountine read a2d 

//Module check I,V
extern void ReadIVRMS(void);		//loop1k
extern void ReadIV(void);			//loop5K
extern void ScaleA2DToIPU(void);	//scale data A2D(0-4095) to I PU(-2-2)
extern void CalIrms(void);			//calculate I rms : Forward clarke->IPeak->IrmsPU
extern void CalVrms(void);			//calculate Vrms output for Vs_alpha,Vs_beta
extern _iq IPhase(Uint16 datain,int16 CenterI);//calculate I phase
extern _iq IPhasePok(Uint16 datain,int16 CenterI);//calculate I phase
extern void CalBuffIp(void);		//calculate buff Ip in loop 5k 

//Module check VDC
extern void ReadVDC(void);	//Door to rountine read a2d VDC bus
extern void ScaleA2DToVDC(void);	//scale A2D A3 to VDC reference by F005 

//Module check fault
extern void Chk_Fault(void);
extern void Chk_FaultE2P(void);//cheack fault cannt read/write
extern void Chk_FaultSC(void);//check fault SC flag from trip zone interrupt
extern void Chk_FaultOH(void);//check over heat
extern void Chk_FaultHE(void);//check external fault
extern void Clr_FaultDIN(void);//clearing fault by DIN
extern void Clr_Fault(void);//common(clear by keypad/DIN) clear fault process
extern void Chk_FaultOC(void);//check over current
extern void Chk_Warning(void);//check warning OC/OL
extern void Chk_FaultOL(void);//check overload (I^2)*t
extern void Cal_Cutfault(void);//calculate cut-point VDC,I check fault I(t)/Imotor
extern void Chk_FaultCAN(void);//check fault can when upload/dnload(no response from backup)
extern void Chk_FaultOU(void);//over voltage
extern void Chk_FaultLU(void);//lower voltage
extern void LUMode0(void);		//check fault LU mode0
extern void LUMode1(void);		//check fault LU mode1
extern void LUMode2(void);		//check fault LU mode2
extern void LUMode3(void);		//check fault LU mode3
extern void LUMode4(void);		//check fault LU mode4
extern void SetFaultLU(void);	//set fault lu
extern void AutoRstFlt(void);	//auto reset fault
extern void ChkTmrLU(void);		//check timer lu for F418=2,3,4
extern void Chk_DecOL(void);	//check condition dec OL data(TWOC,AccuI)
extern Uint16 ChkClrOH(Uint16 CircuitTemp,_iq Temp); //check clear fault OH per channel

extern void Chk_FaultTimer(void); //Timer
extern void Clear_FaultTimer(void); //Reset all timer

//Module check temperature
extern void ReadTemperature(void);
extern _iq ConverseTemp(_iq inputA2D);//converse A2D to temp.C 

//Module DAC
extern void DAC_Setup(void);
extern void Show_DAC(void);
extern void Send_DAC(void);  

//Module DOUT
extern void interrupt_timerDigitalPrt(void);	//Door to rountine digital I/O port
extern void UpdateFinalDIN(void);				//update final data of each port
extern void DigitalPrt_Setup(void);				//Setup GPIO as Digital I/O
extern void Check_DebounceDIN(void);			//check the status and debounce 
extern Uint16 UpdateFinalDOUT(Uint16 selector_DOUT,Uint16 DOUT_state);	//check and datat out of digital output with normal/inverse
extern void UpDwnDIN(void);						//function up/down by DIN 
extern void UpDownOprDIN(Uint16 MaxSet,Uint16 MinSet,Uint16 Factor);		//operation up/down by DIN

extern void ChkFAR_FDT(Uint16 selector);		//function check FAR and FDT
extern void SelectMotor(void);					//Selection of motor1/2 by DIN 

//Module flying start
extern void FlyingStart_Function(_iq target_output1);
extern void DecFFly(_iq reff); 

//Module source of freq.
extern void Freq_function(void);	//Door to rountine rugular Freq. Group3
extern float32 FJogging(void);		//control function jogging
extern _iq FPattern(void);		//control function pattern 

//Module PID
extern void interrupt_timerPID(void);	//Door to rountine PID
extern void FbCond(void);				//feedback signal conditioning rountine
extern void SpCond(void);				//Setpoint signal conditioning rountine
extern void CalPID(void);				//calculation of PID controller
extern _iq LimitPID(void);				//PID Output management : upper/lower limit, limit time
extern _iq Add_IQ17(_iq A,_iq B);//Adding with saturation IQ17
extern float32 Mul_IQ17_IQ8(float32 A17, float32 B15);//multiplication with 2IQs and limited with IQ17
extern float32 Accel(float32 CurrentF,float32 Target,float32 Tacc,float32 Reff,float32 Tsmp);//sub-routine accel
extern float32 Decel(float32 CurrentF,float32 Target,float32 Tdec,float32 Reff,float32 Tsmp);//sub-routine decel
extern float32 Accel_Decel(float32 InputAccDec,float32 MainTarget,float32 Tdecel,float32 Taccel);//accel/decel for without S-Curve
extern float32 SelectorFSet(Uint16 Grp,Uint16 Selector,Uint16 InvDir);
       //selctor of Fset output is PU SP : group need to implement, Selector is value of(F301/F604), and dirtion(1=-1)
 
//Module QEP
extern void QEP_Setup(void);
extern void ReadSpeed(void);
extern void ClrCptr(void);
extern void ClrCntr(void);
extern void ChkDir(void);
extern void LPFRPM(void); 
extern void LdCAPSpeed(void);
extern void LdUTOSpeed(void);

//Module SC fault
extern void SCFault_Setup(void);

//Module status control
extern void Check_Status(void);
extern void Chk_Command(void);//check run/stop command from keypad
extern void FCommMngr(void);//F command manager : Skip/limit/Acc/dec
extern void TCommMngr(void);//Torqe command manager : Acc/dec
extern float32 SkipF(float32 FtargetSkip1);//skip function F411-F414 output is Fcommand before acc/dec
//extern void ChkShtdwnOP(void);	//check shutdown output from fault,coast stop
extern void MchnStatus(void);	//control and check status machine stop,stopping,run,start
extern void CommandStatus(void);    //control and check status of command(torqe,Freq) steady,ready,accel,decel
extern void ClrDataStopping(void); 
extern _iq ACDCSCrv(_iq target,_iq mainacc,_iq maindec,Uint16 slctrinitfinal,_iq current,_iq base);//accel/decel s-curve
extern void ClrDataStop (void); //clear data in stopping/stop
extern void CurrentStall(void);	//stall I F401-F403
extern _iq PI_StallCurrent(_iq spstalli);//PI controller
extern void ClrDataStallI (void);	//clear data PI of current stall
extern void ClrDataStallV(void);
extern void ChckSyst(void);			//check system is ready to run?
extern void InitDataDrv(void);		//initial data for driving
// Add 08/11/65 TSU
extern void PI_Antiwindup(void);
extern void Funtion_FAN_and_RchargeC(void);
extern void DynamicBreak(void);
extern void DCBreak(void);
extern void ePWM_Setup(void);
extern void  eQEP_Init(void);
extern void Motor_Drive(void);
extern void interrupt_ecan0(void);
extern void V_PER_F(void);
extern void Vecter_Control_FOC(void);
extern void VoltageStall(void);
extern void SendoutCAN(void);
//GUS
extern __interrupt void timer0_isr(void);
extern void interrupt_I2C(void);
extern void interrupt_ecan1(void);
extern void CANA_SetUp(void);
extern void ClearMBox (void);
extern void I2C_Init(void);
extern void Setup_GPIO(void);
extern void eCANController(void);
extern void ResetModuleCAN(void);
extern void sendCANMessage(uint32_t objID, uint32_t msgDATAL, uint32_t msgDATAH);
extern void getCANMessage(uint32_t objID, uint32_t msgDATAL, uint32_t magDATAH);
extern __interrupt void epwm1_tz_isr(void);
extern __interrupt void epwm2_tz_isr(void);
extern __interrupt void epwm3_tz_isr(void);
extern __interrupt void epwm4_isr(void);
extern __interrupt void epwm6_isr(void);
extern __interrupt void eqep1_isr(void);
extern void Run_ePWM(void);
extern void ConfigPWM2GPIO(void);

extern void DebounceKeyIN(void);
//Power measurement function
extern void Average_power_CAL(void);

extern void CalPRControl(void);
extern void SetupPRControl(void);

extern void RMS_SystemInit(void);

#ifdef __cplusplus
}
#endif 
#endif   // - end of DSP280X_GLOBALPROTOTYPES_H

//===========================================================================
// End of file.
//===========================================================================
