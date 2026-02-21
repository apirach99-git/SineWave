//2013-5-10 : Got cycle/Turn-on time Bootstrap
//2013-5-10 : run phase and cycle with RamTest
//2013-5-10 : got 2phase
//2013-5-10 : revise 2phase
//2013-5-10 : use loop 5k
//2013-5-14 : completed bootstrap
#include "F28x_Project.h"
#include "include\ND2.h" 

struct BOOTSTRAP_REG BtStrp_reg={0,0,0};
//---External variable import--------------------------------------
struct data_grp{
	Uint16 value[100];
	Uint16 factor[100];
};
extern struct data_grp grp_num[GrpNumber]; 
extern int flag_PWMEnable;
Uint16 RamTestTrnOn = 0,
        RamTestCycle = 0,
        RamTestPhs = 0;
/////////////////////////////////////////////////////////////////////////////////
void ClrBtStrp(void)
{
	BtStrp_reg.BTSTRP_FLAG1.bit.CycleBtStrap        = grp_num[0].value[16]/10;
	BtStrp_reg.BTSTRP_FLAG1.bit.CntrCycleBtStrap    = 0;
	BtStrp_reg.BTSTRP_FLAG2.bit.TrnOnBtStrp         = (grp_num[0].value[16]%10)*5;
	BtStrp_reg.BTSTRP_FLAG2.bit.CntrTrnOnBtStrp     = 0;
	BtStrp_reg.BTSTRP_FLAG1.bit.PhaseBtStrp         = 0;
	BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp         = 0;
}
/////////////////////////////////////////////////////////////////////////////////
void Function_BtStrp(void)
{
struct{
	Uint16 onbtstrp:1;	//turn-on bootstrap
	Uint16 runphase:1;	//run phase x,y,z
}flagbtstrp={0};  

	if(ND2System_Reg.STATE_FLAG.bit.State == state_stop)
	{
		BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp=0;
	}else
	{
		if(BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp==1)
		{		
			BtStrp_reg.BTSTRP_FLAG1.bit.StateBtStrp = 0;	
		}
		else
		{
			if(BtStrp_reg.BTSTRP_FLAG1.bit.StateBtStrp==0)
			{
				if(flag_PWMEnable==2)
				{
					ClrBtStrp();
					BtStrp_reg.BTSTRP_FLAG1.bit.StateBtStrp = 1;
				}				
			}
			else if(BtStrp_reg.BTSTRP_FLAG1.bit.StateBtStrp == 1)
			{
				if(BtStrp_reg.BTSTRP_FLAG2.bit.CntrTrnOnBtStrp>=BtStrp_reg.BTSTRP_FLAG2.bit.TrnOnBtStrp)//(RamTestTrnOn>=5000)
				{
					BtStrp_reg.BTSTRP_FLAG2.bit.CntrTrnOnBtStrp=1;//RamTestTrnOn=0;
					if(BtStrp_reg.BTSTRP_FLAG1.bit.PhaseBtStrp>=2)
					{
						BtStrp_reg.BTSTRP_FLAG1.bit.PhaseBtStrp=0;
						BtStrp_reg.BTSTRP_FLAG1.bit.CntrCycleBtStrap++;//RamTestCycle++;
						if(BtStrp_reg.BTSTRP_FLAG1.bit.CntrCycleBtStrap>=BtStrp_reg.BTSTRP_FLAG1.bit.CycleBtStrap)//(RamTestCycle>=1)
						{
							BtStrp_reg.BTSTRP_FLAG1.bit.StateBtStrp=2;																							
						}else
						{
							flagbtstrp.runphase=1;
						}
					}else
					{
						BtStrp_reg.BTSTRP_FLAG1.bit.PhaseBtStrp++;
						flagbtstrp.runphase=1;
					}
				}else
				{
					BtStrp_reg.BTSTRP_FLAG2.bit.CntrTrnOnBtStrp++;//RamTestTrnOn++;
					flagbtstrp.runphase=1;
				}
			}else if(BtStrp_reg.BTSTRP_FLAG1.bit.StateBtStrp==2)
			{								
				BtStrp_reg.BTSTRP_FLAG1.bit.CmpltBtStrp=1;					
			}
		}
	}
	if(flagbtstrp.runphase==1)
	{
		if(BtStrp_reg.BTSTRP_FLAG2.bit.TrnOnBtStrp>0)
		{
			switch(BtStrp_reg.BTSTRP_FLAG1.bit.PhaseBtStrp)
			{
				case 0:		
					BtStrp_reg.BTSTRP_FLAG1.bit.OnX = 1;
					BtStrp_reg.BTSTRP_FLAG1.bit.OnY = 0;
					BtStrp_reg.BTSTRP_FLAG1.bit.OnZ = 0;
					flagbtstrp.onbtstrp             = 1;
					break;
				case 1:								
					BtStrp_reg.BTSTRP_FLAG1.bit.OnX = 0;
					BtStrp_reg.BTSTRP_FLAG1.bit.OnY = 1;
					BtStrp_reg.BTSTRP_FLAG1.bit.OnZ = 0;
					flagbtstrp.onbtstrp             = 1;
					break;
				case 2:		
					BtStrp_reg.BTSTRP_FLAG1.bit.OnX = 0;
					BtStrp_reg.BTSTRP_FLAG1.bit.OnY = 0;
					BtStrp_reg.BTSTRP_FLAG1.bit.OnZ = 1;
					flagbtstrp.onbtstrp             = 1;
					break;
			}
		}
	}
	BtStrp_reg.BTSTRP_FLAG1.bit.TrnOnBtStrp = flagbtstrp.onbtstrp;
}
/////////////////////////////////////////////////////////////////////////////////
