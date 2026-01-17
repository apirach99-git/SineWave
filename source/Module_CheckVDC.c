//25/4/2012 completed : Tsampling read VDC  
//26/4/2012 completed : convert A2D A3 to VDC          
//26/4/2012 completed : multiply by VDC Gain(F020)
//26/4/2012 completed : LPF VDC Bus
//2012-7-30 : revised LPF with i/p,o/p as iq and check minimum
//2012-10-16 : revised LPF to A2D data

#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

struct VDC_READ_REG VDC_Read_reg = {0,0,0,0,0,
									0,0,0,0};

//---External variable import--------------------------------------
extern Uint16 arA2dBuff[];
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
//----------------------------------------------------------------- 
////////////////////////////////////////////////////////////////
void ReadVDC(void)						
{
	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{	
		VDC_Read_reg.Timer_ReadVDC++;	
		if(VDC_Read_reg.Timer_ReadVDC >= 1)			
		{
			VDC_Read_reg.Timer_ReadVDC = 0;
			ScaleA2DToVDC();
		}
	}	
} 
////////////////////////////////////////////////////////////////
void ScaleA2DToVDC(void)						
{
	/*
	//!!!Process:Scale A2D with 4095->multiply factor->LPF->check model->scaled
	buffV = _IQ17div(_IQ17(arA2dBuff[A2dA3Buff]),_IQ17(4095));
	VDC_Read_reg.BuffPUVDC = _IQ17mpy(buffV,VDC_Read_reg.VDCGain);//VDC=VDC*gain
	*/

	// 1) ADC → float PU
	float adc_f = (float)arA2dBuff[A2dA3Buff];   // 0..4095
	float buffV_f = adc_f / 4095.0f;             // per-unit 0..1
	// 2) Q17 Gain → float
	float gain_f = (float)VDC_Read_reg.VDCGain / IQ17_SCALE_F;
	// 3) Multiply
	float PUVDC_f = buffV_f * gain_f;
	// 4) float → Q17 output
	VDC_Read_reg.BuffPUVDC = (_iq)(PUVDC_f * IQ17_SCALE_F);




	VDC_Read_reg.PUVDC = LowPassFilter(VDC_Read_reg.BuffPUVDC,VDC_Read_reg.PUVDC,ConsTVDC,IQ17_1);

	if(grp_num[0].value[4]==1)//check model 1:400VAC,0:240VAC
	{
		VDC_Read_reg.VdcBus = _IQ17mpy(VDC_Read_reg.PUVDC,VDCmax1);
		VDC_Read_reg.VdcDisplay = VDC_Read_reg.VdcBus;
	}else
	{
		//VDC_Read_reg.VdcBus = _IQ17mpy(VDC_Read_reg.PUVDC,VDCmax0);

		if(VDC_Read_reg.VDCGain <= _IQ17(1))
		{
		    VDC_Read_reg.VdcBus = _IQ17mpy(VDC_Read_reg.PUVDC,VDCmax0);
		    VDC_Read_reg.VdcDisplay = _IQ17mpy(VDC_Read_reg.VdcBus,(_IQ17(1)+(_IQ17(1)-VDC_Read_reg.VDCGain)));
		}
		else if(VDC_Read_reg.VDCGain > _IQ17(1))
		{
		    VDC_Read_reg.VdcBus = _IQ17mpy(VDC_Read_reg.PUVDC,VDCmax0);

		    VDC_Read_reg.VdcDisplay = VDC_Read_reg.VdcBus;
		}
	}
} 
//////////////////////////////////////////////////////////////// 
//	VDC_Read_reg.PUVDC = Analog_reg.PU_B0; //***For test purpose
//ND2System_Reg.MAIN_FLAG2.bit.Test = 1;
