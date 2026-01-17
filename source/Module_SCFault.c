//could got interrupt trip-zone
//completed : gen. flag "ChkFault_Reg.DATA_FAULT.bit.Fault_SC" when trip
//debugged : ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC for only one time trip checking
//2012-5-31 : moved setup GPIO into main
//2012-10-31 : remove check sc
#include "F28x_Project.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"
////////////////////////////////////////////////////////////////////////////////
void SCFault_Setup(void)
{

    //trip zone interrupt register

    //Interrupt_register(INT_EPWM1_TZ, &EPWM1_TZ_ISR);
    //Interrupt_register(INT_EPWM2_TZ, &EPWM2_TZ_ISR);
    //Interrupt_register(INT_EPWM3_TZ, &EPWM3_TZ_ISR);

	//Setup Register*************************
	EALLOW;  
    EPwm1Regs.TZSEL.bit.OSHT1  = 1;				//enable one-shot tripping in TZ1
    EPwm1Regs.TZCTL.bit.TZA    = 2;  			//force output ePWM1A to low state
	EPwm1Regs.TZCTL.bit.TZB    = 2;
    EPwm1Regs.TZEINT.bit.OST   = 1; 			//enable one-shot interrupt

	EPwm2Regs.TZSEL.bit.OSHT1  = 1;				//enable one-shot tripping in TZ1
    EPwm2Regs.TZCTL.bit.TZA    = 2;  			//force output ePWM1A to low state
	EPwm2Regs.TZCTL.bit.TZB    = 2;
    EPwm2Regs.TZEINT.bit.OST   = 1; 			//enable one-shot interrupt

	EPwm3Regs.TZSEL.bit.OSHT1  = 1;				//enable one-shot tripping in TZ1
    EPwm3Regs.TZCTL.bit.TZA    = 2;  			//force output ePWM1A to low state
	EPwm3Regs.TZCTL.bit.TZB    = 2;
    EPwm3Regs.TZEINT.bit.OST   = 1; 			//enable one-shot interrupt
    EDIS;		
	
} 

////////////////////////////////////////////////////////////////////////////////

