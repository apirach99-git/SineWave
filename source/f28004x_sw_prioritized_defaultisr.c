//###########################################################################
//
// FILE:    f28004x_sw_prioritized_defaultisr.c
//
// TITLE:   Default Software Prioritized Interrupt Service Routines.
//
//###########################################################################
//
//
// $Copyright:
// Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include "f28004x_sw_prioritized_isr_levels.h"
#include "ND2.h"

//
// Globals
//


//
// Connected to PIEIER1_7 (use MINT1 and MG1_7 masks):
//
#if (G1_7PL != 0)
__interrupt void TIMER0_ISR(void)
{   //GPIO_writePin(37, 1);
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER1.all;
    IER |= M_INT1;
    IER    &= MINT1;                         // Set "global" priority
    PieCtrlRegs.PIEIER1.all &= MG1_7;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts
    EINT;

    //
    // Insert ISR Code here
    //

 //Move   DisplayNormal();

    interrupt_timer();

    interrupt_timerADC();

 //Move   Chk_Fault();

    Funtion_FAN_and_RchargeC();

 //   interrupt_timerPID();

//   interrupt_timerDigitalPrt();

    eCANController();               //reset module when no response,user config boud rate

    Check_Status();                 //check and determind which mode of the system

   // Update_AOUT();                  //update data to analog output pwm4,5

//p    ReadTemperature();              //read temperature

//p    ReadIVRMS();                    //read current(A2D A0,A1,A2) U,V,W and Vout

//Move  ReadVDC();                      //read VDCbus(A2D A3)

    Freq_function();                //group3(freq.) manager

    SendoutCAN();                   //scan node the send the data of CAN

//Move    Chk_FaultTimer();               //40sec Fault Timer

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER1.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //
    //GPIO_writePin(37, 0);
}
#endif

//
// Connected to PIEIER3_6 (use MINT3 and MG3_6 masks):
//
#if (G3_6PL != 0)
__interrupt void EPWM6_ISR(void)
{

   // GPIO_writePin(56, 1);
   // GPIO_writePin(57, 1);
   // GPIO_writePin(59, 1);

    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER3.all;
    IER |= M_INT3;
    IER    &= MINT3;                         // Set "global" priority
    PieCtrlRegs.PIEIER3.all &= MG3_6;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //
    GPIO_writePin(37, 1);
    DtoA();

    ReadVDC();                      //read VDCbus(A2D A3)

    Motor_Drive();
    EPwm6Regs.ETCLR.bit.INT = 1;//Clear Flag

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER3.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //

    GPIO_writePin(37, 0);
  //  GPIO_writePin(56, 0);
  //  GPIO_writePin(57, 0);
 //   GPIO_writePin(59, 0);
}
#endif



// -----------------------------------------------------------
// PIE Group 2 - MUXed into CPU INT2
// -----------------------------------------------------------

//
// Connected to PIEIER2_1 (use MINT2 and MG2_1 masks):
//
#if (G2_1PL != 0)
__interrupt void EPWM1_TZ_ISR(void)
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER2.all;
    IER |= M_INT2;
    IER    &= MINT2;                         // Set "global" priority
    PieCtrlRegs.PIEIER2.all &= MG2_1;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //

    if(ChkFault_Reg.DATA_FAULT.bit.Fault_SC==0)
    {
        ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC=1;
    }

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER2.all = TempPIEIER;

    //Add ISR to Trace


}
#endif

//
// Connected to INT13 of CPU (use MINT13 mask):
//
// Note CPU-Timer1 is reserved for TI use, however XINT13
// ISR can be used by the user.
//
#if (INT13PL != 0)
__interrupt void TIMER1_ISR(void)
{
     IER &= MINT13;                 // Set "global" priority
     EINT;

    //
    // Insert ISR Code here
    //
     /*DisplayNormal();

     interrupt_timer();

     interrupt_timerADC();

     Chk_Fault();

     Funtion_FAN_and_RchargeC();

     interrupt_timerPID();

     interrupt_timerDigitalPrt();

     eCANController();               //reset module when no response,user config boud rate

     Check_Status();                 //check and determind which mode of the system

     Update_AOUT();                  //update data to analog output pwm4,5

     ReadTemperature();              //read temperature

     ReadIVRMS();                        //read current(A2D A0,A1,A2) U,V,W and Vout

     ReadVDC();                      //read VDCbus(A2D A3)

     Freq_function();                //group3(freq.) manager

     SendoutCAN();                   //scan node the send the data of CAN

     //ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1); //Clear ADC interrupt
*/
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
    //ESTOP0;
     //for(;;);
}
#endif

//
// Connected to INT14 of CPU (use MINT14 mask):
//
#if (INT14PL != 0)
__interrupt void TIMER2_ISR(void)
{
    IER &= MINT14;                  // Set "global" priority
    EINT;

    //
    // Insert ISR Code here
    //


    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    ESTOP0;
    for(;;);
}
#endif

//
// Connected to INT15 of CPU (use MINT15 mask):
//
#if (INT15PL != 0)
__interrupt void DATALOG_ISR(void)
{
    IER &= MINT15;                 // Set "global" priority
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}
#endif

//
// Connected to INT16 of CPU (use MINT16 mask):
//
#if (INT16PL != 0)
__interrupt void RTOS_ISR(void)
{
    IER &= MINT16;                 // Set "global" priority
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}
#endif

//
// Connected to EMUINT of CPU (non-maskable):
//
__interrupt void EMU_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to NMI of CPU (non-maskable):
//
__interrupt void NMI_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to ITRAP of CPU (non-maskable):
//
__interrupt void ILLEGAL_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER1 of CPU (non-maskable):
//
__interrupt void USER1_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER2 of CPU (non-maskable):
//
__interrupt void USER2_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER3 of CPU (non-maskable):
//
__interrupt void USER3_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER4 of CPU (non-maskable):
//
__interrupt void USER4_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER5 of CPU (non-maskable):
//
__interrupt void USER5_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER6 of CPU (non-maskable):
//
__interrupt void USER6_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER7 of CPU (non-maskable):
//
__interrupt void USER7_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER8 of CPU (non-maskable):
//
__interrupt void USER8_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER9 of CPU (non-maskable):
//
__interrupt void USER9_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER10 of CPU (non-maskable):
//
__interrupt void USER10_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER11 of CPU (non-maskable):
//
__interrupt void USER11_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

//
// Connected to USER12 of CPU (non-maskable):
//
__interrupt void USER12_ISR(void)
{
    EINT;

    //
    // Insert ISR Code here
    //

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
   ESTOP0;
    for(;;);
}

// -----------------------------------------------------------
// PIE Group 1 - MUXed into CPU INT1
// -----------------------------------------------------------
//
// Connected to PIEIER1_1 (use MINT1 and MG1_1 masks):
//
#if (G1_1PL != 0)
__interrupt void ADCA1_ISR(void)     // ADC
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER1.all;
    IER |= M_INT1;
    IER    &= MINT1;                         // Set "global" priority
    PieCtrlRegs.PIEIER1.all &= MG1_1;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts
    __asm("  NOP");
    EINT;

    //
    // Insert ISR Code here
    //

    interrupt_a2d();
    //Update_AOUT();

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER1.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //


}
#endif


//
// Connected to PIEIER2_2 (use MINT2 and MG2_2 masks):
//
#if (G2_2PL != 0)
__interrupt void EPWM2_TZ_ISR(void)
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER2.all;
    IER |= M_INT2;
    IER    &= MINT2;                         // Set "global" priority
    PieCtrlRegs.PIEIER2.all &= MG2_2;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //

    if(ChkFault_Reg.DATA_FAULT.bit.Fault_SC==0)
    {
        ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC=1;
    }

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER2.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //



}
#endif

//
// Connected to PIEIER2_3 (use MINT2 and MG2_3 masks):
//
#if (G2_3PL != 0)
__interrupt void EPWM3_TZ_ISR(void)
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER2.all;
    IER |= M_INT2;
    IER    &= MINT2;                         // Set "global" priority
    PieCtrlRegs.PIEIER2.all &= MG2_3;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //

    if(ChkFault_Reg.DATA_FAULT.bit.Fault_SC==0)
    {
        ChkFault_Reg.FLAG_FAULT.bit.Fisrt_SC=1;
    }

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER2.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //


}
#endif

// -----------------------------------------------------------
// PIE Group 3 - MUXed into CPU INT3
// -----------------------------------------------------------




// -----------------------------------------------------------
// PIE Group 5 - MUXed into CPU INT5
// -----------------------------------------------------------

//
// Connected to PIEIER5_1 (use MINT5 and MG5_1 masks):
//
#if (G5_1PL != 0)
__interrupt void EQEP1_ISR(void)
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER5.all;
    IER |= M_INT5;
    IER    &= MINT5;                         // Set "global" priority
    PieCtrlRegs.PIEIER5.all &= MG5_1;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //


    if(EQep1Regs.QFLG.bit.QDC==1)
    {
        ChkDir();
    }
    if(EQep1Regs.QFLG.bit.UTO==1)
    {
        ReadSpeed();
    }
    EQep1Regs.QCLR.bit.INT=1;

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER5.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //

}
#endif

// -----------------------------------------------------------
// PIE Group 8 - MUXed into CPU INT8
// -----------------------------------------------------------

//
// Connected to PIEIER8_1 (use MINT8 and MG8_1 masks):
//
#if (G8_1PL != 0)
__interrupt void I2CA_ISR(void)
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER8.all;
    IER |= M_INT8;
    IER    &= MINT8;                         // Set "global" priority
    PieCtrlRegs.PIEIER8.all &= MG8_1;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //

    interrupt_I2C();

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER8.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //


}
#endif

//
//
// Connected to PIEIER9_5 (use MINT9 and MG9_5 masks):
//
#if (G9_5PL != 0)
__interrupt void CANA0_ISR(void)
{
    // Set interrupt priority:
    volatile uint16_t TempPIEIER = PieCtrlRegs.PIEIER9.all;
    IER |= M_INT9;
    IER    &= MINT9;                         // Set "global" priority
    PieCtrlRegs.PIEIER9.all &= MG9_5;   // Set "group"  priority
    PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts

    EINT;

    //
    // Insert ISR Code here
    //

    interrupt_ecan1();
    CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    //
    // Restore registers saved:
    //
    DINT;
    PieCtrlRegs.PIEIER9.all = TempPIEIER;

    //
    //  Add ISR to Trace
    //


}
#endif

//
// Catch All Default ISRs:
//

//
// PIE_RESERVED_ISR - Reserved PIE ISR
//
__interrupt void PIE_RESERVED_ISR(void)
{
    ESTOP0;
    for(;;);
}

//
// NOTUSED_ISR - Unused ISR
//
__interrupt void NOTUSED_ISR(void)
{
    //ESTOP0;
    //for(;;);
}

//
// End of file
//
