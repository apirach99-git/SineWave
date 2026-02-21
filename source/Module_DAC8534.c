//got d2a but there is offset 50%
//completed : d2a @100%
//2012-5-31 : moved setup GPIO into main
#include "F28x_Project.h"
#include "include\ND2.h"
//see PID outout +1.5
struct  DAC_SPI{
   long Data_high;
   long Data_low;
   long DAC_a;
   long DAC_b;
   long DAC_c;
   long DAC_d; 
   long Indep_long1;
   int  C_DAC;

}; 
struct  DAC_SPI DAC = {0,0,0,0,0,0,0,0};

float32 RamTest3=0;
Uint16 RamTesDAC=0,RamTestDAC1=5000;
extern Uint16 arA2dBuff[];
extern int F_Command;

void DAC_Setup(void)
{
//------------------------------------------------------- 		
//	Serial Peripheral Interface(SPI) Initialization     
//--------------------------------------------------------
    EALLOW;
//Clear register
	SpibRegs.SPICCR.all = 0;
	SpibRegs.SPICTL.all = 0;
	SpibRegs.SPIBRR.all = 0;

	SpibRegs.SPICCR.all = 0x004F;
	SpibRegs.SPICTL.all = 0x000E;

	SpibRegs.SPIBRR.all = 9;//Buad Rate = 9 SPI run = 10 Mbps @ clk=10MHz

	SpibRegs.SPICCR.bit.SPISWRESET 	= 1;

    EDIS;

	return;
}  

void Send_DAC(void)
{
	SpibRegs.SPITXBUF =  DAC.Data_high  ;
	while(SpibRegs.SPISTS.bit.INT_FLAG == 0);

	SpibRegs.SPITXBUF = DAC.Data_low;
	while(SpibRegs.SPISTS.bit.INT_FLAG == 0); 

	return;
}

void Show_DAC(void)
{
/*
	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{		
		RamTest3 = _IQ17toF(PID_Reg.PU_SetpointK);				
		RamTest3 = (RamTest3*65535);
		DAC.DAC_a = RamTest3;
		DAC.DAC_a = DAC.DAC_a & 0x0000FFFF;
		
		RamTest3 = arA2dBuff[A2dA1Buff];	
		RamTest3 = RamTest3/4095;
		RamTest3 = (RamTest3*65535);
		DAC.DAC_b = RamTest3;
		DAC.DAC_b = DAC.DAC_b & 0x0000FFFF;

		RamTest3 = arA2dBuff[A2dA2Buff];		
		RamTest3 = RamTest3/4095;
		RamTest3 = (RamTest3*65535);
		DAC.DAC_c = RamTest3;
		DAC.DAC_c = DAC.DAC_c & 0x0000FFFF;

		DAC.C_DAC++;
		if(DAC.C_DAC > 3)
			DAC.C_DAC =0;
			switch(DAC.C_DAC)
			{
				case 0 :	
						DAC.Indep_long1 = ((DAC.DAC_a ^ 0x0000)<<8)|0x10000000|0x00000000;
						DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
						DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
						Send_DAC();
					break;
				case 1 :
						DAC.Indep_long1 = ((DAC.DAC_b ^ 0x0000)<<8)|0x10000000|0x02000000;
						DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
						DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
						Send_DAC();
					break;
				case 2 :	
						DAC.Indep_long1 = ((DAC.DAC_c ^ 0x0000)<<8)|0x10000000|0x04000000;
						DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
						DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
						Send_DAC();
					break;
				case 3 :	
						DAC.Indep_long1 = ((DAC.DAC_d ^ 0x0000)<<8)|0x10000000|0x06000000;
						DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
						DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
						Send_DAC();
					break;
			}
	}
}*/


	if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==1)
	{ 
		DAC.DAC_a = DAC.DAC_a & 0x0000FFFF;
		DAC.DAC_b = DAC.DAC_b & 0x0000FFFF;
		DAC.DAC_c = DAC.DAC_c & 0x0000FFFF;
		DAC.DAC_d = DAC.DAC_d & 0x0000FFFF;


		DAC.C_DAC++;
		if(DAC.C_DAC > 3)
			DAC.C_DAC =0; 

		switch(DAC.C_DAC)
		{
			case 0 :	
					DAC.Indep_long1 = ((DAC.DAC_a ^ 0x8000)<<8)|0x10000000|0x00000000;
					DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
					DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
					Send_DAC();
				break;
			case 1 :
					DAC.Indep_long1 = ((DAC.DAC_b ^ 0x8000)<<8)|0x10000000|0x02000000;
					DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
					DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
					Send_DAC();
				break;
			case 2 :	
					DAC.Indep_long1 = ((DAC.DAC_c ^ 0x8000)<<8)|0x10000000|0x04000000;
					DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
					DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
					Send_DAC();
				break;
			case 3 :	
					DAC.Indep_long1 = ((DAC.DAC_d ^ 0x8000)<<8)|0x10000000|0x06000000;
					DAC.Data_high = (DAC.Indep_long1 & 0xFFFF0000)>>16;
					DAC.Data_low = DAC.Indep_long1 & 0x0000FFFF;
					Send_DAC();
				break;
		}
	}
}



