//2013-2-15 : got fail upload
//2013-2-15 : completed uploading
//2013-2-15 : completed downloading
//2013-4-1 : no reset bit Syst_ready when up/dnload

#include "F28x_Project.h"
#include "include\DSP280x_I2C_defines.h"
#include "include\ND2.h"

struct BCKUP_REG Bckup_reg = {0,0,0,0,0,0};
extern Uint16 *PntrDataE2Prom;
//----------------------------------------------------------------
void Uploading(void)
{		
Uint32 buff=0,buff2=0;	

	if(KeyPad_reg.GENERAL_FLAG.bit.BuffBRP == 0)
	{
		Bckup_reg.BCKUP_FLAG.bit.UpldSt = 1;									
		DispTab(121);							
		KeyPad_reg.PNTR_MODE.bit.PntrGrp=0;		//first group for uploading
		KeyPad_reg.E2PROM_ADRS.all = 0x0100;	//start e2prom adrs at value group0
		do	
		{	
			if(Bckup_reg.BCKUP_FLAG.bit.SendDataUpld == 0)
			{
			    //ServiceDog();
				Run_AdrsGRP();	//run adrs of e2prom
				//Packaging data to be sent
				buff  = *PntrDataE2Prom;//data of function
				buff  = buff<<16;
				buff2 = KeyPad_reg.E2PROM_ADRS.all-2;
				Bckup_reg.MSG_MBX2DataL = buff|buff2;		//MSG_MBX2DataL	

				Bckup_reg.DataH_Bckup.bit.U_P = 1;	//write backup command
				Bckup_reg.MSG_MBX2DataH       = Bckup_reg.DataH_Bckup.all;
				//--------------------------------------
				Bckup_reg.BCKUP_FLAG.bit.SendDataUpld = 1;	//command to start send data 
				Bckup_reg.BCKUP_FLAG.bit.AlrdySntUpld = 0;	//reset already sent
			}
			ServiceDog();
		}
		while(KeyPad_reg.PNTR_MODE.bit.PntrGrp<GrpNumber
		        &&Bckup_reg.BCKUP_FLAG.bit.UpldFail == 0);

		KeyPad_reg.PNTR_MODE.bit.PntrGrp      = 0;
		Bckup_reg.BCKUP_FLAG.bit.UpldSt       = 0;
		Bckup_reg.BCKUP_FLAG.bit.SendDataUpld = 0;	
		Bckup_reg.BCKUP_FLAG.bit.AlrdySntUpld = 0;
		Bckup_reg.BCKUP_Timer.bit.CANUpld     = 0;

	}
	else
	{
		Bckup_reg.BCKUP_FLAG.bit.UpldFail = 1;
	}														 
}		
////////////////////////////////////////////////////////////////
void Downloading(void)
{
Uint32 buff=0,buff2=0;
	if(KeyPad_reg.GENERAL_FLAG.bit.BuffBRP == 0)
	{
		Bckup_reg.BCKUP_FLAG.bit.DnldSt = 1;									
		DispTab(122);							
		KeyPad_reg.PNTR_MODE.bit.PntrGrp = 0;		//first group for uploading
		KeyPad_reg.E2PROM_ADRS.all = 0x0100;	//start e2prom adrs at value group0						
		
		do						
		{	
			Run_AdrsGRP();	//run adrs of e2prom
			if(KeyPad_reg.PNTR_MODE.bit.PntrGrp<GrpNumber)
			{
				//Packaging data to be sent
				buff  = 0;//data of function
				buff  = buff<<16;
				buff2 = KeyPad_reg.E2PROM_ADRS.all-2;
				Bckup_reg.MSG_MBX2DataL = buff|buff2;		//load adrs e2prom that need to read from slave	

				Bckup_reg.DataH_Bckup.bit.U_P = 0;	//read backup command
				Bckup_reg.MSG_MBX2DataH = Bckup_reg.DataH_Bckup.all;
				//--------------------------------------
				Bckup_reg.BCKUP_FLAG.bit.SendDataDnld = 1;	//ready to send adrs need to read
				Bckup_reg.BCKUP_Timer.bit.CANDnld     = 0;		//clear timeout timer
				Bckup_reg.BCKUP_FLAG.bit.SlvSntData   = 0;
				do{
					Bckup_reg.BCKUP_FLAG.bit.TimeoutDnld = 1;
					ServiceDog();
				}while(Bckup_reg.BCKUP_FLAG.bit.SlvSntData == 0 &&
    				   Bckup_reg.BCKUP_FLAG.bit.DnldFail == 0);	//waiting for slave sentback the data
				//--------------------------------------
				if(Bckup_reg.BCKUP_FLAG.bit.DnldFail == 0)
				{							
					//Bckup_reg.BufferMBX3L = ECanaMboxes.MBOX3.MDL.all>>16;
					*PntrDataE2Prom = Bckup_reg.BufferMBX3L;							
					WriteE2prom(KeyPad_reg.E2PROM_ADRS.bit.Adrs_H,KeyPad_reg.E2PROM_ADRS.bit.Adrs_L-2,*PntrDataE2Prom);
					while(KeyPad_reg.E2PROM_FLAG1.bit.W_Status!=I2C_MSGSTAT_RESTART &&
						  KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)//waiting for re-writting 7ms
					{
						ServiceDog();
					}	
				}
			}																		
		}while(KeyPad_reg.PNTR_MODE.bit.PntrGrp<GrpNumber&&
		      Bckup_reg.BCKUP_FLAG.bit.DnldFail == 0 &&
		      KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0);

		KeyPad_reg.PNTR_MODE.bit.PntrGrp      = 0;
		Bckup_reg.BCKUP_FLAG.bit.DnldSt       = 0;
		Bckup_reg.BCKUP_FLAG.bit.SendDataDnld = 0;
		Bckup_reg.BCKUP_FLAG.bit.TimeoutDnld  = 0;
		Bckup_reg.BCKUP_Timer.bit.CANDnld     = 0;
	}else
	{
		Bckup_reg.BCKUP_FLAG.bit.DnldFail = 1;
	} 
}				
////////////////////////////////////////////////////////////////
