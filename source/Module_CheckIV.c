//completed : sampling1ms
//completed : LPF I_U
//completed : PU Iu
//revised : scale A2D to PU
//completed : convert A2D to PU U,V,W
//completed : Ialpha
//completed : clarke conversion
//completed : Calculate Ipeak
//completed : Calculate Irms
//revised : LPF after calculate Ipeak
//completed : added type of current sensor2/3chanel by F012
//2012-7-30 : revised LPF with checking minimum
//2012-8-1 : add function read Vout
//2012-8-20 : sub offset read I 0.008
//2012-10-3 : no LPF if vout=0
//2012-10-5 : revised read I
//2012-10-5 : remove -offset read I
//2012-10-5 : change port Iw
//2012-10-8 : revised A2D to Ipu
//2012-10-8 : use offset A2D same both model
//2012-10-15 : revised auto tune offset A2D
//2012-10-15 : revised offset Ipeak
//2012-10-15 : add clearing LPF filter after stop
//2012-10-19 : revised cal.I
//2012-10-22 : revised cal.I by check <0.005 only stop
//2012-10-22 : add offset Ip
//2012-10-24 : cheange bit check ND2System_Reg.FLAG_SYST_CTRL.bit.Syst_ready to enter all functions
//2012-10-25 : remove buf calIQ in cal IV
//2012-10-29 : move cal I in loop5k
//2012-11-12 : removed offset I when stop
//2013-6-18 : move cal IPU to here
//2013-6-28 : add check bit init e2prom before cal I
//2013-7-16 : use buffer for A2D of I


//============ Aui ================
//2013-12-23 : Add Lowpass Filters (cutoff 2.5kHz) into IV_Read_reg.PU_IU,IV_Read_reg.PU_IV and IV_Read_reg.PU_IW (No Active).

#include "F28x_Project.h"
#include "include\ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct IV_READ_REG IV_Read_reg = {0,0,0,0,0,
								  0,0,0,0,0,
								  0,0,0,2048,2048,2048,
								  2048,2048,0,0,0,
								  0,0,0,0,0,
								  0,0,0,0,0,0,0};

// --- Renaming for generic use ---
// #define RMS_Update Vrms_Update  <-- Removed
// typedef VrmsCalc_t RMSCalc_t;   <-- Removed

RMSCalc_t Vrms_In;
RMSCalc_t Vrms_Out;
RMSCalc_t Irms_Out; // Added for Output Current RMS

//---External variable import--------------------------------------
extern Uint16 arA2dBuff[];
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 

extern int Vs_alpha;
extern int Vs_beta; 
extern long Volt_Vq,Volt_Vd; 

extern int cos_Theta_o;
extern float SinTheta_fp;

extern int isd_LPF ,isq_LPF ;

long isa_LPF = 0,isb_LPF = 0,isc_LPF = 0;
long buff_long = 0;
long CountFreqIn;
//Gus

float32 Pout_motor, I_DC_bus, VBaseDC, IBaseDC;
_iq Sbase;
int buffAx;
_iq buffAy;
extern float Po_total;


//----------------------------------------------------------------- 
////////////////////////////////////////////////////////////////
void ReadIVRMS(void)
{
	IV_Read_reg.IV_FLAG.bit.Timer_ReadI++;
	if(IV_Read_reg.IV_FLAG.bit.Timer_ReadI >= TsCntr1)
	{
		IV_Read_reg.IV_FLAG.bit.Timer_ReadI = 0;
		CalIrms();			//calculation Irms PU
		CalVrms();			//calculation Voutrms
	}
} 
////////////////////////////////////////////////////////////////
void CalVrms(void)
{
    float Vf_d = (float)Volt_Vd * 0.00000762939453125f; // Volt_Vd / 131072.0f
    float Vf_q = (float)Volt_Vq * 0.00000762939453125f; // Volt_Vq / 131072.0f
    float buff_cal = (Vf_d * Vf_d) + (Vf_q * Vf_q);

	IV_Read_reg.Vout_Rms = sqrtf(buff_cal);
} 
////////////////////////////////////////////////////////////////
void CalIrms(void)
{
_iq buffout = IV_Read_reg.BuffIp;

	if(ND2System_Reg.STATE_FLAG.bit.State==state_stop)
	{
		buffout = (IV_Read_reg.BuffIp>_IQ17(0.05))?IV_Read_reg.BuffIp:0;
	}
	if(buffout!=0)
	{
		IV_Read_reg.I_Peak = (float)LowPassFilter(buffout,(_iq)(IV_Read_reg.I_Peak * 131072.0f),Cons_I,IQ17_1) * 0.00000762939453125f;
	}
	else
	{
		IV_Read_reg.I_Peak = 0.0f;
	}

	//Irms****
	float I_Peak_f = IV_Read_reg.I_Peak;
	float Irms_f = I_Peak_f / 1.41421356237f; // Irms = Ip/sqrt(2)
	IV_Read_reg.I_rms = Irms_f;

}
////////////////////////////////////////////////////////////////
void ReadIV(void)
{
//_iq buff_caliq = 0;
_iq buff_long  = 0;

Uint16 buffA0 = arA2dBuff[A2dA0Buff];
Uint16 buffA1 = arA2dBuff[A2dA1Buff];
//Uint16 buffA2 = arA2dBuff[A2dA2Buff];
Uint16 buffA7 = arA2dBuff[A2dA7Buff];
Uint16 buffA4 = arA2dBuff[A2dA4Buff];

	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{
		if(IV_Read_reg.IV_FLAG.bit.OffsetA2DI==1)
		{

		    buff_long = (_iq)(IPhasePok(buffA7, IV_Read_reg.CenterVo) * 131072.0f);
            IV_Read_reg.PU_VO = LowPassFilter(buff_long, IV_Read_reg.PU_VO, 0, IQ17_1);


           buff_long = (_iq)(IPhasePok(buffA4, IV_Read_reg.CenterVo) * 131072.0f);
    //       buff_long = IPhase(buffA4, 1859);
            IV_Read_reg.PU_VI = LowPassFilter(buff_long, IV_Read_reg.PU_VI, 0, IQ17_1);

			buff_long = (_iq)(IPhasePok(buffA0, IV_Read_reg.CenterIu) * 131072.0f);
			IV_Read_reg.PU_IU = LowPassFilter(buff_long, IV_Read_reg.PU_IU, 0, IQ17_1);

			//buff_long = (_iq)(IPhasePok(buffA1, IV_Read_reg.CenterIw) * 131072.0f);
			//IV_Read_reg.PU_IV = LowPassFilter(buff_long, IV_Read_reg.PU_IV, 0, IQ17_1);
			IV_Read_reg.PU_IV = buff_long;
            if((SinTheta_fp>=0)&&(IV_Read_reg.PU_VI_last<0))
                {
                        if(IV_Read_reg.PU_VI>0)CountFreqIn = _IQ17(-0.002);
                        else CountFreqIn = _IQ17(0.002);
                }
            IV_Read_reg.PU_VI_last=SinTheta_fp;

		//buff_long = IPhasePok(buffA0,IV_Read_reg.CenterIu);
		//IV_Read_reg.PU_IU = _IQ17mpy(43691,buff_long)+_IQ17mpy(87381,IV_Read_reg.PU_IU);//cutoff 2.5kHz
			
		}
		/*=========================Clarke Transform===============================
    	!!!Process : Clarke->IPeak->Irms***
    	----------------------------------------------
    	Clarke calculation

    	|Ialpha|       |1   -1/2             -1/2   | |Iu|
    	|	   | = 2/3 |                            | |Iv|
    	|Ibeta |       |0   sqrt(3)/2     -sqrt(3)/2| |Iw|

    	IPeak calculation = sqrt(IAlpha^2+IBeta^2)
    	Irms calculation = IPeak/sqrt(2)
    	----------------------------------------------*/
    	//Alpha****
 /*   	buff_caliq = IV_Read_reg.PU_IU-
    			   _IQ17div(IV_Read_reg.PU_IV,_IQ17(2))-
    			   _IQ17div(IV_Read_reg.PU_IW,_IQ17(2));//Iu-(Iv/2)-(Iw/2)
    	IV_Read_reg.I_Alpha = _IQ17mpy(buff_caliq,_IQ17(0.6666666667));//multiplied by 2/3

    	//Beta****
    	buff_caliq         = IV_Read_reg.PU_IV-IV_Read_reg.PU_IW;//[(Iv-Iw)*sqrt(3)/2]*2/3
    	IV_Read_reg.I_Beta = _IQ17div(buff_caliq,_IQ17(1.732050808));

    	//Immediate Ipeak
    	IV_Read_reg.BuffIp = _IQ17mag(IV_Read_reg.I_Alpha,IV_Read_reg.I_Beta);//Ip = sqrt(alpha^2+beta^2)

//= ========================Park Transform===============================//rms	Q17	Ibase = F004
    	IV_Read_reg.Isd = _IQ15mpy(IV_Read_reg.I_Alpha,-cos_Theta_o) + _IQ15mpy(IV_Read_reg.I_Beta,-sin_Theta_o);
    	IV_Read_reg.Isq = _IQ15mpy(IV_Read_reg.I_Alpha,sin_Theta_o)  + _IQ15mpy(IV_Read_reg.I_Beta,-cos_Theta_o);
  */
// 		float Vin_inst = IV_Read_reg.PU_VI/524.4;
		float Vin_inst = IV_Read_reg.PU_VI*0.001906941; // Using multiplication instead of division for speed
		float Vout_inst = IV_Read_reg.PU_VO*0.001906941; // Using multiplication instead of division for speed
		float Iout_inst = IV_Read_reg.PU_IU*0.001906941*0.014; // Using multiplication instead of division for speed
    	    RMS_Update(&Vrms_In,  Vin_inst);
    	    RMS_Update(&Vrms_Out, Vout_inst);
            
            // Calculate Output Current RMS (Single Phase)
            RMS_Update(&Irms_Out,Iout_inst);
            
           // IV_Read_reg.I_rms = Irms_Out.rms;

  }
} 
////////////////////////////////////////////////////////////////

_iq IPhase(Uint16 datain,int16 CenterI)
{
	//		 A2D-Offset
	//	PU = ----------*2
	//		   Offset


int16 buff_cal=0;
_iq buffout=0;

	buff_cal = datain-CenterI;
	buff_cal = buff_cal*2;

	if(buff_cal<-4095)	buff_cal = -4095;
	else if(buff_cal>4095)buff_cal = 4095;

	buffout = _IQ17div(_IQ17(buff_cal),_IQ17(CenterI));


	return buffout;
}
////////////////////////////////////////////////////////////////
float IPhasePok(Uint16 datain,int16 CenterI)
{
    //       A2D-Offset
    //  PU = ----------*2
    //         Offset

    int16 buff_cal=0;

        float buff_f, Center_f;


        // 1) คำนวณ A2D - Offset
        buff_cal = (int16)(datain - CenterI);
        // 2) *2 ตามสมการเดิม
        buff_cal = buff_cal * 2;

        // 3) Limit เหมือนโค้ดเดิม
        if(buff_cal > 4095) buff_cal = 4095;
        else if(buff_cal < -4095)    buff_cal = -4095;

        // 4) แปลงเป็น float
        buff_f   = (float)buff_cal;   // numerator
        Center_f = (float)CenterI;    // denominator

        // 5) ทำ per-unit แบบ float
        //    PU = (buff_cal / CenterI)
        return buff_f / Center_f;
} 
////////////////////////////////////////////////////////////////
//	IV_Read_reg.BuffIp = Analog_reg.PU_B0;//For test purpose only
//	ND2System_Reg.MAIN_FLAG2.bit.Test=1;
