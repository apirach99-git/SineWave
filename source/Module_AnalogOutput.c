//completed : setup pwm4,5
//completed : select source of AOUT F241
//completed : LPF of AOUT2(F242)
//completed : linear scaling eq.
//completed : deadband
//extracted : integer part to PWM ratio
//loaded : from buff to PWM module and goout to port AOUT2
//completed : Analogout AOUT2/FMA
//2012-5-31 : moved setup GPIO into main
//2012-7-2 : use MainIQ_Variable.FCommand for all mode(PID/not PID)
//2012-7-20 : use PID_Reg.PU_FeedbackK as output if it set
//2012-7-30 : revised LPF
//2012-7-30 : revised LPF all 2chanels
//2012-8-1 : added Aout I,Vout,VDC
//2012-11-23 : revised base of vout,I
//2013-2-20 : add analog output RPM 
//2013-6-10 : use filter time by converse IQ
//2013-7-16 : use Irms for AOUT
//2013-7-18 : use Ipeak for AOUT
//2013-8-15 : chang output argument "SelectAOUT" as IQ
//2013-8-15 : chang output argument "AOUT_Scaling" as IQ
//2013-8-15 : remove interrupt PWM4
//2013-8-15 : debug load mode of PWM as load on period
//2013-9-3 : add AOUT torque command
//2023-3-2: DACA and DACB have been used to generate the signal for the FMA function

#include "F28x_Project.h"
#include "include\ND2.h"

struct AOUT_REG AOUT_Reg = {0,0,0,0,0,
							0,0,0,0,0,
							0,0,0,0,0,
							0};
//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 

extern int Vs_alpha,va_com,va;

struct  PWM_COMMAN{
   long va_com;
   long vb_com;
   long vc_com;
}; 
extern struct  PWM_COMMAN PWM_COM;   
extern int Vmax,Vmin;
extern long VoHPWM,Voa;
extern int sin_Theta_o;
extern float SinTheta_fp,CosTheta_fp;

_iq buffnewdata = 0;
_iq buff_dataout = 0;
_iq buffts = IQ17_1;
long PokTest = 0;

///////////////////////////////////////////////////////////////////////////////// 
void AnalogOutSetup(void) 
{

    //DACA, FMA
    //
    //Set DAC reference voltage gain
    //
    DAC_setGainMode(DACA_BASE, DAC_GAIN_TWO);
    //
    // Set DAC reference voltage.
    //
    DAC_setReferenceVoltage(DACA_BASE, DAC_REF_ADC_VREFHI);
    //
    // Set DAC load mode.
    //
    DAC_setLoadMode(DACA_BASE, DAC_LOAD_SYSCLK);
    //
    // Enable the DAC output
    //
    DAC_enableOutput(DACA_BASE);
    //
    // Set the DAC shadow output
    //
    DAC_setShadowValue(DACA_BASE, 0U);

    DEVICE_DELAY_US(500);

//------------------------------------------------------------------------------------------------------------------------------

    //DACB, Analog 2
    //
    //Set DAC reference voltage gain
    //
    DAC_setGainMode(DACB_BASE, DAC_GAIN_TWO); //Vref=1.65*2
    //
    // Set DAC reference voltage.
    //
    DAC_setReferenceVoltage(DACB_BASE, DAC_REF_ADC_VREFHI);
    //
    // Set DAC load mode.
    //
    DAC_setLoadMode(DACB_BASE, DAC_LOAD_SYSCLK);
    //
    // Enable the DAC output
    //
    DAC_enableOutput(DACB_BASE);
    //
    // Set the DAC shadow output
    //
    DAC_setShadowValue(DACB_BASE, 0U);
    //
    // Delay for buffered DAC to power up.
    //
    DEVICE_DELAY_US(500);

    return;

}
/////////////////////////////////////////////////////////////////////////////////
void Update_AOUT(void)
{
//_iq buff_dataout = 0;
//_iq buffnewdata = 0;
//_iq buffts = IQ17_1;

	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{
		//select source of FMA
		buffnewdata = SelectAOUT(grp_num[2].value[40]);//select source what data need to FMA		
		buffnewdata = SelectAOUT(0);
		//Low Pass Filter		
		AOUT_Reg.Buff_FMALPF = LowPassFilter(buffnewdata,AOUT_Reg.Buff_FMALPF,AOUT_Reg.FMA_filter,buffts);
		
		//Linear scaling by eq. : Y-Y1 = M*(X-X1)
		AOUT_Reg.Buff_FMA = AOUT_Scaling(buffnewdata,AOUT_Reg.FMA_X1,AOUT_Reg.FMA_Y1,AOUT_Reg.FMA_X2,AOUT_Reg.FMA_Y2);

		//only positive value is available
		AOUT_Reg.Buff_FMA = _IQ17abs(AOUT_Reg.Buff_FMA);

		//Saturate value between 0,10
		AOUT_Reg.Buff_FMA = (AOUT_Reg.Buff_FMA>=_IQ17(10))?_IQ17(10):AOUT_Reg.Buff_FMA;

		//Deadband checking => O/P = 0
		AOUT_Reg.Buff_FMA = AOUT_DB(AOUT_Reg.Buff_FMA,AOUT_Reg.FMA_DB);

        //X = (input*4095)/10
		buff_dataout = _IQ17mpy(AOUT_Reg.Buff_FMA, _IQ17(409.5));
		DAC_setShadowValue(DACA_BASE, _IQ17int(buff_dataout));

//-------------------------------------------------------------------------------------------------

		//select source of AOUT
		buffnewdata = SelectAOUT(grp_num[2].value[41]);//select source what data need to AOUT2
		buffnewdata = SelectAOUT(0);
		//Low Pass Filter		
		AOUT_Reg.Buff_AOUT2LPF = LowPassFilter(buffnewdata,AOUT_Reg.Buff_AOUT2LPF,AOUT_Reg.AOUT2_filter,buffts);
		
		//Linear scaling by eq. : Y-Y1 = M*(X-X1)
		AOUT_Reg.Buff_AOUT2 = AOUT_Scaling(buffnewdata,AOUT_Reg.AOUT2_X1,AOUT_Reg.AOUT2_Y1,AOUT_Reg.AOUT2_X2,AOUT_Reg.AOUT2_Y2);

		//only positive value is available
		AOUT_Reg.Buff_AOUT2 = _IQ17abs(AOUT_Reg.Buff_AOUT2);

		//Saturate value between 0,10
		AOUT_Reg.Buff_AOUT2 = (AOUT_Reg.Buff_AOUT2>=_IQ17(10))?_IQ17(10):AOUT_Reg.Buff_AOUT2;

		//Deadback checking => O/P = 0 	
		AOUT_Reg.Buff_AOUT2 = AOUT_DB(AOUT_Reg.Buff_AOUT2,AOUT_Reg.AOUT2_DB);

        //X = (input*4095)/10
		buff_dataout = _IQ17mpy(AOUT_Reg.Buff_AOUT2, _IQ17(409.5));
		DAC_setShadowValue(DACB_BASE, _IQ17int(buff_dataout));
	}
}
/////////////////////////////////////////////////////////////////////////////////
_iq AOUT_DB(_iq input1,_iq deadband)
{
_iq	db = 0,output_db = 0;
_iq input = 0;

	//db = %db*10=Volatge db
	db = _IQ17mpy(deadband,_IQ17(10));
	input = input1;
		
	output_db = (input<=db)?0:input;

	return output_db;
}
/////////////////////////////////////////////////////////////////////////////////
_iq AOUT_Scaling(_iq input,_iq x1,_iq y1,_iq x2,_iq y2)
{
_iq y = 0,m = 0,deltax = 0,deltay = 0;
	
/*		Main equation : Y-Y1 = M(X-X1)
					     =>Y = M(X-X1)+Y1
*/
	deltax = x2 - x1;
	deltay = y2 - y1;

	if(deltax != 0)
	{
		m = _IQ17div(deltay,deltax); //y = (y2-y1)/(x2-x1)
		y = _IQ17rsmpy(m,(input-x1));
		y = y+y1;
	}else
	{
		y = 0;
	}

	return y;
}
/////////////////////////////////////////////////////////////////////////////////
_iq SelectAOUT(Uint16 selector)
{
_iq buff_aout = 0;

	switch(selector)
	{		
		case 1://Aout => I
			buff_aout = _IQ17div(MainIQ_Variable.I_Rate,MainIQ_Variable.IRef);
			buff_aout = _IQ17mpy(buff_aout,IV_Read_reg.I_Peak);
			break;
		//-------------------------------
		case 2://Aout => V		
			buff_aout = _IQ17div(MainIQ_Variable.V_base,MainIQ_Variable.VRef);
			buff_aout = _IQ17mpy(buff_aout,IV_Read_reg.Vout_Rms);
			break;
		//-------------------------------
		case 3://Aout => VDC
			buff_aout = _IQ17div(VDC_Read_reg.VdcBus,MainIQ_Variable.VRef);
			break;
		//-------------------------------
		case 4://Aout => PID Fb
			buff_aout = PID_Reg.PU_FeedbackK;
			break;
		//-------------------------------
		case 5://Aout => RPM				
			buff_aout = QEP_Reg.PU_RPM;
			break;
		case 6://Aout => Torque				
			//buff_aout = TrqCmnd_Reg.TrqCommand;
			//buff_aout = (VoHPWM)<<2;//PWM_COM.va_com<<4;

		    buff_aout = (sin_Theta_o<<4) + _IQ17(0.5);
			break;
		default://Aout => Fout			
			//buff_aout = MainIQ_Variable.FCommand;
	//		buff_aout = (PWM_COM.va_com<<5) + _IQ16(1.0);

		//	PokTest = va;
		//	PokTest = IV_Read_reg.PU_VO;
		//	buff_aout = (PokTest) + _IQ16(1.0);
		 //   buff_aout = (va<<4) + _IQ16(1.0);
		//    buff_aout = (IV_Read_reg.PU_VO>>2) + _IQ17(0.5);
		    buff_aout = (sin_Theta_o) + _IQ17(0.5);

	}

	return _IQ17abs(buff_aout);
}
///////////////////////////////////////////////////////////////////////////////// 

void DtoA(void)         //Max =4096
{   int CH_A,CH_B;

    CH_A = (int16_t)(SinTheta_fp * 2048.0f)+2048;   // Q12
    CH_B = (int16_t)(CosTheta_fp * 2048.0f)+2048;   // Q12

    DAC_setShadowValue(DACA_BASE,CH_A);
    DAC_setShadowValue(DACB_BASE,CH_B);


}
