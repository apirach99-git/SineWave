//completed : trig timer10ms
//read a2d
//converse to iq value
//got CoEff_Output,CoEff_Input
//LPF but not modify1
//LPF but not modify1
//revised LPF not use float32
//revised LPF move variable to local
//scaled but not completed 
//revised delta_x=0
//revised delta_x>0 and other condition
//completed : equotaion of AIN2(+-10v)
//converted : data from keypad to float32
//converted : data from keypad to iq
//scaled B1 with setting from keypad
//completed : fset chanel AIN12
//completed : calibrate A2d
//completed : tunning for A2DB1
//revised : compensate linear +0.0001
//revised : tunning AIN2
//completed : scaled and FrefA2
//completed : scaled and FrefA3
//completed : tunned AIN12
//completed : convert back of A12
//completed : all chanel of A2D converted to Freq
//debugged : protection overflow of FSet of AIN12 only
//debugged : protection overflow all AIN
//added all chanel conversion
//revised : LPFmodule on float32 variable
//added : checking finish read e2prom
//revised : grp_num from number to name
//revised : converse A2D all chanels not select by F301
//added : *compensation 1.0007629
//no compensate
//no cal.
//2012-6-21 : revised move conv. IQ into keypad.c
//2012-6-25 : revised check range
//2012-7-2 : revised remove linear result to PU directly
//2012-7-27 : revised function "ChkRngF" without IQtoF in argument
//2012-7-30 : revised function LowPassFilter with i/p,o/p iq and check minimum
//2012-7-30 : revised function LowPassFilter all 3chanels
//2012-9-3 : completed VR Keypad with LPF
//2012-9-4 : revised VR key
//2012-9-6 : added reset WD
//2012-10-2 : add compensate AIN12
//2012-10-3 : revised compensate AIN12
//2012-10-3 : add compensate LPF 2minimum res iq17
//2012-10-4 : revised VR key remove last bit from key
//2012-10-15 : add cal offset A2D I
//2012-10-22 : revised avrg read I to 4096times
//2012-11-03 : use center of I sensor replace the offset
//2012-11-07 : debugged port A2D for I sensor
//2012-11-12 : revised order of conversion A2D
//2012-12-19 : revised initial ADC with toa = 5.6ns
//2013-4-18 : removed unnecessary for scalling A2D I
//2013-6-10 : use filter time converse IQ
//2013-6-10 : Linear scale equation use IQ
//2013-6-29 : revise no add minimum IQ if input==0
//2013-7-9 : revise compensate A2
//2013-7-17 : revise devider 4095 of center I
//2022-12-3 : The ADC module has been intialized by the TI-Driver(F280049)

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"


//#include "KeyPad.h"
/*
	|--------------------------------------------|    
	|Input signal at terminal and voltage of A2D | 
	|--------------------------------------------|   
	|             |   High 3.0V   |    Low 0.0V  | 
	|-------------|---------------|--------------|
	| -AIN12(B0)  |     10V       |       0V	 |	
	| -AIN2(B1)   |     10V       |     -10V	 |
	| -AIN3(B2)   |     20mA      |       4mA	 |
	|-------------|---------------|--------------|  
	
	Direction of motor and Freq.Set
	Freq.Set < 0 --> REV
	Freq.Set > 0 --> FWD
*/
Uint16 arA2dBuff[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

struct ANALOG_REG Analog_reg={0,0,0,0,0,
							  0,0,0,0,0,
							  0,0,0,0,0,
							  0,0,0,0,0,
							  0,0,0,0,0,
							  0,0};
//---External variable import--------------------------------------
struct data_grp{
    Uint16 value[100];
    Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber];
extern uint32_t BufferMBX0L;
//-----------------------------------------------------------------
Uint16 RamTestADCIP=0;
_iq RamTestLnr1=0,RamTestLnr2=0,RamTestLnr3=0,RamTestLnr4=0;
_iq RamTestCmp1=0,RamTestCmp2=0;
/////////////////////////////////////////////////////////////////////////////////
void interrupt_timerADC(void)						
{
	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{	
		Analog_reg.Timer_ReadADC++;	
		if(Analog_reg.Timer_ReadADC >= TsCntr1)			
		{
			Analog_reg.Timer_ReadADC = 0;	
					
			//FunctLPF();   	//low pass filter
			//FunctLnrScl();	//routine linear scale
			ChkVRKey();		//Check VR from Keypad
		}
	}		
}
/////////////////////////////////////////////////////////////////////////////////
void ChkVRKey(void)
{
Uint32 buff=0;
_iq	buffdata=0;
	
	buff = BufferMBX0L & 0xFFFF0000;
	buff = buff>>16;
	buff = (buff==1)?0:buff;
	buffdata = _IQ17div(_IQ17(buff),_IQ17(1023));	
	if(buffdata!=0)	
	{
		buffdata = LowPassFilter(buffdata,Analog_reg.VRKeypad,toaVRKey,IQ17_1);	
	}		
	Analog_reg.VRKeypad = (buffdata>=IQ17_1)?IQ17_1:buffdata;
} 
/////////////////////////////////////////////////////////////////////////////////
_iq ChkRngF(_iq input,_iq pufmax,_iq pufmin)						
{
/*
	Operation
	input=>PU of input need to checked

	check PU input with max/min-->check direction-->return as PU
*/
_iq bufffset=0,dir_m=1;

	if(input<0)
	{
		dir_m = -1;
	}
	
	if(_IQ17abs(input)>=pufmax)//limit max&min
	{
		input=_IQ17mpy(pufmax,_IQ17(dir_m));
	}else if(_IQ17abs(input)<=pufmin)
	{
		input=_IQ17mpy(pufmin,_IQ17(dir_m));
	}
	bufffset = input;

	return bufffset;
}  
/////////////////////////////////////////////////////////////////////////////////
void FunctLnrScl(void)						
{
_iq iqbuffip=0;	
	
_iq	cmpst=0;
_iq buffinput=0;
_iq IQ4095=_IQ17(4095);
//A2D AIN12 chanel

		iqbuffip  = _IQ17div(RangeAIN12,IQ4095);
		buffinput = Analog_reg.A2dB0-CmpstAIN12Lower;
		buffinput = (buffinput<0)?0:Analog_reg.A2dB0;
		cmpst     = (buffinput!=0)?CmpstAIN12Upper:0;		//compemsate A2D that data>0
		iqbuffip  = _IQ17mpy(iqbuffip,buffinput)+cmpst;

		Analog_reg.PU_B0 = LnrScl(iqbuffip,Analog_reg.X1_AIN12,Analog_reg.X2_AIN12,
									Analog_reg.Y1_AIN12,Analog_reg.Y2_AIN12,Analog_reg.DB_AIN12);	

//A2D A2 chanel
		Analog_reg.GENERAL_ADCFLAG.bit.AIN2_Chnl = 1;

		iqbuffip  = _IQ17div(RangeAIN2,IQ4095);
		buffinput = Analog_reg.A2dB1-CmpstAIN2Lower;
		buffinput = (buffinput<0)?0:Analog_reg.A2dB1;
		buffinput = _IQ17mpy(iqbuffip,buffinput);
		cmpst     = (buffinput!=0)?CmpstAIN2Upper:0;
		buffinput = buffinput+OffBackB1+cmpst;	
		Analog_reg.PU_B1 = LnrScl(buffinput,Analog_reg.X1_A2,Analog_reg.X2_A2,
							Analog_reg.Y1_A2,Analog_reg.Y2_A2,Analog_reg.DB_A2);	
		Analog_reg.GENERAL_ADCFLAG.bit.AIN2_Chnl = 0;														

//A2D A3 chanel

		iqbuffip = _IQ17div(RangeAIN3,IQ4095);
		iqbuffip = _IQ17mpy(iqbuffip,Analog_reg.A2dB2)+compA3;			
		Analog_reg.PU_B2 = LnrScl(iqbuffip,Analog_reg.X1_A3,Analog_reg.X2_A3,
									Analog_reg.Y1_A3,Analog_reg.Y2_A3,Analog_reg.DB_A3);
}
/////////////////////////////////////////////////////////////////////////////////
_iq LnrScl(_iq input,_iq x1,_iq x2,_iq y1a,_iq y2a,_iq db)
{
/*
	|----------------------------------------|    
	|equation : 						 	 |    
	|		y-y1 = m(x-x1) 					 |
	|		note:     						 |     
	| 		        y2-y1					 |
	|	       m =  -----					 |
	|            	x2-x1					 |
	|----------------------------------------|
*/
_iq y=0,m=0,db_crtr=0;
_iq delta_x=0,delta_y=0,cross_x=0;
_iq y1=_IQ17div(y1a,IQ17_100);
_iq y2=_IQ17div(y2a,IQ17_100);

	delta_x = x2-x1;
	delta_y = y2-y1;
	if(delta_x>0)
	{	
		cross_x = _IQ17mpy(delta_x,y1);
		cross_x = x1-_IQ17div(cross_x,delta_y);

		m = _IQ17div(delta_y,delta_x);
		y = _IQ17mpy(m,(input-x1));
		y = y+y1;//there are sign for direction of rotation		

		//the below algorithm will update y in special criteria(deadband)
		if(Analog_reg.GENERAL_ADCFLAG.bit.AIN2_Chnl==0)//deadband for AIN12,AIN3
		{						
			db_crtr = _IQ17abs(_IQ17abs(cross_x)-input);//deadband criteria y=0 for sign of y2/y1 are diff
			//Gus--------------------------------------------
			        Analog_reg.BuffInputA12 = db_crtr;
			//Gus--------------------------------------------

			if(y2==0||y1==0)	
			{
				if(input<db)
				{
					y = y1;
				}		
			}else if((y2<0 && y1<0)||(y2>0 && y1>0))//check same sign or not
			{
				if(input<db)
				{
					y = y1;
				}
			}else if(db_crtr<db)
			{
				y=0;
			}
		}else//deadband for AIN2
		{
			Analog_reg.GENERAL_ADCFLAG.bit.AIN2_Chnl = 0;
			db_crtr = _IQ17abs(cross_x-input);//deadband criteria y=0 for sign of y2/y1 are diff

			if((cross_x <=_IQ17(-9.99999))||(cross_x >= _IQ17(9.99999)))//check x-cross = -10 or 10
			{
				if(input < (_IQ17(-10)+db))
				{
					y = y1;
				}				
			}else if(db_crtr<db)
			{
				y=0;
			}								
		}
	}else
	{
		y = 0;
	}
	y = _IQ17mpy(y,_IQ17(comp_scl));
	return y;				
}
///////////////////////////////////////////////////////////////////////////////// 
_iq ConvStg(Uint16 value1, Uint16 factor1)
{
	float32 buffdivider=0;
	_iq buffdata=0;
	ServiceDog();
	buffdivider = pow(10, ((factor1 & 0x0300)>>8));

  	buffdata = _IQ17(value1);
	buffdata = _IQ17div(buffdata, _IQ17(buffdivider));
	if((factor1 & 0x2000)!=0)
	{
		buffdata = _IQ17mpy(buffdata, MinusIQ);
	} 
	return buffdata;
} 
/////////////////////////////////////////////////////////////////////////////////
void FunctLPF(void)
{
_iq buffnewdata=0,buffts=0;
	
	buffts = IQ17_1;
	//Low pass filter for AIN-12	
	buffnewdata      = _IQ17(arA2dBuff[A2dB0Buff]);
	Analog_reg.A2dB0 = LowPassFilter(buffnewdata,Analog_reg.A2dB0,Analog_reg.FilterAIN12,buffts);

	//Low pass filter for AIN-A2
	buffnewdata      = _IQ17(arA2dBuff[A2dB1Buff]);
	Analog_reg.A2dB1 = LowPassFilter(buffnewdata,Analog_reg.A2dB1,Analog_reg.FilterAINA2,buffts);

	//Low pass filter for AIN-A3
	buffnewdata      = _IQ17(arA2dBuff[A2dB2Buff]);
	Analog_reg.A2dB2 = LowPassFilter(buffnewdata,Analog_reg.A2dB2,Analog_reg.FilterAINA3,buffts);
}
/////////////////////////////////////////////////////////////////////////////////
_iq LowPassFilter(_iq input, _iq inputn_1, _iq toa, _iq ts)
{
/*
	|----------------------------------------|    
	|LPF equation : 						 |    note:
	|										 |    0<=Vi,Vo<=4095 
	|          Ts            Toa			 |
	|Vo(n) = ------xVi(n) + ------xVo(n-1)	 |
	|        Toa+Ts         Toa+Ts			 |
	|----------------------------------------|
							
				    Ts     
	CoEff_Input = ------   ->max=0.5
	              Toa+Ts   ->min0.0001

    			    Toa     
	CoEff_Output = ------  ->max=0.9999
	               Toa+Ts  ->min0.5
*/


    // 1) Q17 -> float
    float input_f    = (float)input    / IQ17_SCALE_F;  // Vi(n)
    float inputn1_f  = (float)inputn_1 / IQ17_SCALE_F;  // Vo(n-1)

    // toa, ts เก็บเป็น Q17 ของค่าคงที่เวลา → แปลงเป็น float
    float toa_f = (float)toa / IQ17_SCALE_F;
    float ts_f  = (float)ts  / IQ17_SCALE_F;

    // 2) หา denominator และ coefficient ในโดเมน float
    float denom_f        = toa_f + ts_f;       // Toa + Ts
    float coeffinput_f   = ts_f  / denom_f;    // Ts/(Toa+Ts)
    float coeffoutput_f  = toa_f / denom_f;    // Toa/(Toa+Ts)

    // 3) คูณด้วย float
    float buff_f  = coeffinput_f  * input_f;    // ส่วนของ Vi(n)
    float buff1_f = coeffoutput_f * inputn1_f;  // ส่วนของ Vo(n-1)

    float output_f = buff_f + buff1_f;          // Vo(n) (float)

    // 4) แปลงกลับเป็น Q17
    _iq output = (_iq)(output_f * IQ17_SCALE_F);

    // 5) เลียนแบบโค้ดเดิม:
    //    ถ้า buff != 0 → output += MinResIQ17
    //    (ใช้ buff_f แทน buff แบบ Q17)
    if (buff_f != 0.0f)
    {
        output += MinResIQ17;
    }

/*
    _iq buff=0,buff1=0,output=0;
    _iq coeffinput=0;
    _iq coeffoutput=0;

	buff = toa+ts;	
	coeffinput  = _IQ17div(ts,buff);
	coeffoutput = _IQ17div(toa,buff);
	buff   = IQmpyMin(coeffinput,input);
	buff1  = _IQ17mpy(coeffoutput,inputn_1);
	output = buff+buff1;
	if(buff!=0)
	{
		output = output+MinResIQ17;
	}
*/
	return output;	
}
////////////////////////////////////////////////////////////////
_iq IQmpyMin(_iq A,_iq B)
{
_iq output=0;

	output = _IQ17mpy(A,B);
	if(output==0)
	{
		if(A!=0 && B!=0)
		{
			output = MinResIQ17;
		}
	}
	return output;
} 
/////////////////////////////////////////////////////////////////////////////////

/*
/////////////////////////////////////////////////////////////////////////////////
void ADC_Setup(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

    EALLOW;
    SysCtrlRegs.HISPCP.all = 0x4;  //SYSCLKOUT/8=100M/8=12.5MHz at MAX for ADC
    EDIS;

    AdcRegs.ADCTRL3.bit.ADCBGRFDN = 3;  //Bandgap&Ref. power up
    AdcRegs.ADCTRL3.bit.ADCPWDN = 1;    //circuit power up
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0;   //ADC Clock=HSPCLK/1=12.5MHz-->80ns
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;  //Simultaneous mode
    DELAY_US(ADC_usDELAY);

    AdcRegs.ADCTRL1.bit.SUSMOD = 0;     //Emulation is ignored
    AdcRegs.ADCTRL1.bit.ACQ_PS = 5;     //Time duration (5+1)*80ns-->480ns
    AdcRegs.ADCTRL1.bit.CPS = 0;        //HSPCLK/1-->80ns
    AdcRegs.ADCTRL1.bit.CONT_RUN = 1;
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 1;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;

    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1

    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0x7;       //haw many time to converse=n+1
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x7; //1st sequence
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x6;
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x5;
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x4;
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x3; //2nd sequence
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x2;
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x1;
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x0;
} 
/////////////////////////////////////////////////////////////////////////////////
 */
//ADC setting for the round-robbin priority
void ADC_Setup(void)
{
    //Interrupt_register(INT_ADCA1, &ADCA1_ISR);
    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 4;
    //
    // asysctl initialization
    //
    // Disables the temperature sensor output to the ADC.
    //
    ASysCtl_disableTemperatureSensor();
    //
    // Set the analog voltage reference selection to internal.
    //
    ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
    //
    // Set the internal analog voltage reference selection to 1.65V.
    //
    ASysCtl_setAnalogReference1P65( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );

    ADC_setOffsetTrimAll(ADC_REFERENCE_INTERNAL,ADC_REFERENCE_3_3V);

    //-----------------------------------------------------ADCA---------------------------------------------------------------------------------
    //
    // Configures the analog-to-digital converter module prescaler.
    //
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0);
    //
    // Sets the timing of the end-of-conversion pulse
    //
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
    //
    // Powers up the analog-to-digital converter core.
    //
    ADC_enableConverter(ADCA_BASE);
    //
    // Delay for 1ms to allow ADC time to power up
    //
    DELAY_US(ADC_usDELAY);

    //
    // SOC Configuration: Setup ADC EPWM channel and trigger settings
    //
    // Disables SOC burst mode.
    //
    ADC_disableBurstMode(ADCA_BASE);
    //
    // Sets the priority mode of the SOCs.
    //
    ADC_setSOCPriority(ADCA_BASE, ADC_PRI_ALL_ROUND_ROBIN);
    //
    // Start of Conversion 0 Configuration
    //
    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 6 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    //
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN2, 47U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN3, 47U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN4, 47U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN5, 47U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN6, 47U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER4, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN8, 47U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER5, ADC_INT_SOC_TRIGGER_NONE);

    //
    // ADC Interrupt 1 Configuration
    //      SOC/EOC number  : 5
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    //
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER4);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
    //------------------------------------------------------------------------------------------------------------------------------------------

    //-----------------------------------------------------ADCB---------------------------------------------------------------------------------
    //
    // Configures the analog-to-digital converter module prescaler.
    //
    ADC_setPrescaler(ADCB_BASE, ADC_CLK_DIV_2_0);
    //
    // Sets the timing of the end-of-conversion pulse
    //
    ADC_setInterruptPulseMode(ADCB_BASE, ADC_PULSE_END_OF_CONV);
    //
    // Powers up the analog-to-digital converter core.
    //
    ADC_enableConverter(ADCB_BASE);
    //
    // Delay for 1ms to allow ADC time to power up
    //
    DELAY_US(ADC_usDELAY);

    //
    // SOC Configuration: Setup ADC EPWM channel and trigger settings
    //
    // Disables SOC burst mode.
    //
    ADC_disableBurstMode(ADCB_BASE);
    //
    // Sets the priority mode of the SOCs.
    //
    ADC_setSOCPriority(ADCB_BASE, ADC_PRI_ALL_ROUND_ROBIN);
    //
    // Start of Conversion 0 Configuration
    //
    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCB
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 6 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    //
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0, 47U);
    ADC_setInterruptSOCTrigger(ADCB_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN1, 47U);
    ADC_setInterruptSOCTrigger(ADCB_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN2, 47U);
    ADC_setInterruptSOCTrigger(ADCB_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN3, 47U);
    ADC_setInterruptSOCTrigger(ADCB_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN4, 47U);
    ADC_setInterruptSOCTrigger(ADCB_BASE, ADC_SOC_NUMBER4, ADC_INT_SOC_TRIGGER_NONE);
    //-----------------------------------------------------------------------------------------------------------------------------------------------

    //---------------------------------------------------------------ADCC----------------------------------------------------------------------
    //
    // Configures the analog-to-digital converter module prescaler.
    //
    ADC_setPrescaler(ADCC_BASE, ADC_CLK_DIV_2_0);
    //
    // Sets the timing of the end-of-conversion pulse
    //
    ADC_setInterruptPulseMode(ADCC_BASE, ADC_PULSE_END_OF_CONV);
    //
    // Powers up the analog-to-digital converter core.
    //
    ADC_enableConverter(ADCC_BASE);
    //
    // Delay for 1ms to allow ADC time to power up
    //
    DELAY_US(ADC_usDELAY);

    //
    // SOC Configuration: Setup ADC EPWM channel and trigger settings
    //
    // Disables SOC burst mode.
    //
    ADC_disableBurstMode(ADCC_BASE);
    //
    // Sets the priority mode of the SOCs.
    //
    ADC_setSOCPriority(ADCC_BASE, ADC_PRI_ALL_ROUND_ROBIN);
    //
    // Start of Conversion 0 Configuration
    //
    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCC
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 6 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    //
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0, 47U);
    ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN1, 47U);
    ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN2, 5U);
    ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN4, 47U);
    ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);

    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN14, 47U);
    ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER4, ADC_INT_SOC_TRIGGER_NONE);

    //-------------------------------------------------------------------------------------------
}
void ClrAvrgA2D(void)
{
	IV_Read_reg.CntrAvrgA2D = 0;
	IV_Read_reg.BuffA2DA0   = 0;
	IV_Read_reg.BuffA2DA1   = 0;
	IV_Read_reg.BuffA2DA2   = 0;
	IV_Read_reg.BuffA2DA7   = 0;
}
/////////////////////////////////////////////////////////////////////////////////

void interrupt_a2d(void)
{

//--------------------------------------ADC for 280049--------------------------------------------------------

    arA2dBuff[A2dA0Buff] = AdcaResultRegs.ADCRESULT0; //IU
    arA2dBuff[A2dA1Buff] = AdcbResultRegs.ADCRESULT0; //IV
    arA2dBuff[A2dA2Buff] = AdccResultRegs.ADCRESULT0; //IW
    arA2dBuff[A2dA3Buff] = AdcaResultRegs.ADCRESULT1; //VDC_bus
    arA2dBuff[A2dA4Buff] = AdcbResultRegs.ADCRESULT1; //Vrect
    arA2dBuff[A2dA5Buff] = AdccResultRegs.ADCRESULT1; //Temperature F030
    arA2dBuff[A2dA6Buff] = AdcaResultRegs.ADCRESULT3; //Temperature F031
    arA2dBuff[A2dA7Buff] = AdcbResultRegs.ADCRESULT3;   //Vout

    arA2dBuff[A2dB0Buff] = AdcaResultRegs.ADCRESULT2; //Ain_1
    arA2dBuff[A2dB1Buff] = AdcbResultRegs.ADCRESULT2; //Ain_2
    arA2dBuff[A2dB2Buff] = AdccResultRegs.ADCRESULT2; //Ain_3
    arA2dBuff[A2dB3Buff] = AdccResultRegs.ADCRESULT3;
    arA2dBuff[A2dB4Buff] = AdcaResultRegs.ADCRESULT4;
    arA2dBuff[A2dB5Buff] = AdcbResultRegs.ADCRESULT4; //Temperature F032
    arA2dBuff[A2dB6Buff] = AdccResultRegs.ADCRESULT4; //Temperature F033
    arA2dBuff[A2dB7Buff] = AdcaResultRegs.ADCRESULT5;
//--------------------------------------ADC for 280049--------------------------------------------------------


//============= Cal. Offset I ===============
    if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd == 1)
    {
        if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
        {
            if(IV_Read_reg.CntrAvrgA2D >= 4095)
            {
                //IV_Read_reg.CenterIu = IV_Read_reg.BuffA2DA0>>12;
                if(grp_num[0].value[11] == 0)
                {
                    IV_Read_reg.CenterIu = IV_Read_reg.BuffA2DA0>>12;
                    IV_Read_reg.CenterIw = IV_Read_reg.BuffA2DA1>>12;
                    IV_Read_reg.CenterIv = IV_Read_reg.BuffA2DA2>>12;

                    IV_Read_reg.CenterVo = IV_Read_reg.BuffA2DA7>>12;
                //    IV_Read_reg.CenterVi = IV_Read_reg.BuffA2DA7>>12;
                    IV_Read_reg.IV_FLAG.bit.OffsetA2DI = 1;//complete after mcu reset
                    ClrAvrgA2D();
                }
                else
                {
                    IV_Read_reg.CenterIu = IV_Read_reg.BuffA2DA0>>12;
                    IV_Read_reg.CenterIv = IV_Read_reg.BuffA2DA1>>12;
                    IV_Read_reg.CenterIw = IV_Read_reg.BuffA2DA2>>12;

                    IV_Read_reg.CenterVo = IV_Read_reg.BuffA2DA7>>12;
                  //  IV_Read_reg.CenterVi = IV_Read_reg.BuffA2DA7>>12;
                    IV_Read_reg.IV_FLAG.bit.OffsetA2DI = 1;//complete after mcu reset
                    ClrAvrgA2D();
                }

                //IV_Read_reg.IV_FLAG.bit.OffsetA2DI=1;//complete after mcu reset
                //ClrAvrgA2D();
            }
            else
            {
                IV_Read_reg.CntrAvrgA2D++;
                IV_Read_reg.BuffA2DA0 = IV_Read_reg.BuffA2DA0+arA2dBuff[A2dA0Buff];
                IV_Read_reg.BuffA2DA1 = IV_Read_reg.BuffA2DA1+arA2dBuff[A2dA1Buff];
                IV_Read_reg.BuffA2DA2 = IV_Read_reg.BuffA2DA2+arA2dBuff[A2dA2Buff];

                IV_Read_reg.BuffA2DA7 = IV_Read_reg.BuffA2DA7+arA2dBuff[A2dA7Buff];

            }
        }
        else
        {
            ClrAvrgA2D();
        }
    }
//  AdcaRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1 to CONV00
//  AdcaRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
    //ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    //Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}
