//2012-5-31 : moved setup GPIO into main
//2013-2-15 : revise i2c e2prom algorithm


/*
 * Module_I2CGus.c
 *
 *  Created on: Nov 23, 2022
 *      Author: Piyasak
 */

#include "F28x_Project.h"
#include "DSP280x_I2C_defines.h"
#include "ND2.h"
#include "f28004x_sw_prioritized_isr_levels.h"

//Uint16 intSource;

//---External variable import--------------------------------------
extern Uint16  TimeOutE2p;

void I2C_Init(void)
{
    //Interrupt_register(INT_I2CA, &I2CA_ISR);
    //Interrupt_enable(INT_I2CA);

    I2C_disableModule(I2CA_BASE);

    I2caRegs.I2CSAR.all  = 0x0050;    // Slave address - EEPROM control code
    I2caRegs.I2CPSC.all  = 9;         // Prescaler - need 7-12 Mhz on module clk
    I2caRegs.I2CCLKL     = 45;        // NOTE: must be non zero
    I2caRegs.I2CCLKH     = 45;        // NOTE: must be non zero
    I2caRegs.I2CIER.all  = 0x24;      // Enable SCD & ARDY interrupts

    I2caRegs.I2CMDR.all  = 0x0020;    // Take I2C out of reset
                                      // Stop I2C when suspended
    I2caRegs.I2CFFTX.all = 0x0040;    // reset FIFO
    I2caRegs.I2CFFRX.all = 0x0040;
    I2caRegs.I2CFFTX.all = 0x6000;    // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2040;    // Enable RXFIFO, clear RXFFINT,
}
//////////////////////////////////////////////////////////////////
Uint16 ReadE2prom(Uint16 AdrsH, Uint16 AdrsL)
{
    TimeOutE2p = 0;
    while (I2caRegs.I2CMDR.bit.STP == 1 &&
           KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
    {
        Chk_TimeOutE2P();
    }

    KeyPad_reg.E2PROM_FLAG.bit.Writting = 0;
    I2caRegs.I2CSAR.all = 0x50;

    TimeOutE2p = 0;
    while(KeyPad_reg.E2PROM_FLAG1.bit.R_Status != I2C_MSGSTAT_BUFFREADY &&
          KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
    {
        Chk_TimeOutE2P();
        if(KeyPad_reg.E2PROM_FLAG1.bit.R_Status == I2C_MSGSTAT_SEND_NOSTOP)//check first step is performed or not?
        {
            TimeOutE2p = 0;
           while (I2caRegs.I2CSTR.bit.BB == 1 &&
                  KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
           {
                Chk_TimeOutE2P();
           }

           I2caRegs.I2CCNT = 2;//send adrsH,L
           I2caRegs.I2CDXR.all = AdrsH;
           I2caRegs.I2CDXR.all = AdrsL;
           I2caRegs.I2CMDR.all = 0x6620;

           KeyPad_reg.E2PROM_FLAG1.bit.R_Status = I2C_MSGSTAT_SEND_NOSTOP_BUSY;//start read first step
        }else if(KeyPad_reg.E2PROM_FLAG1.bit.R_Status == I2C_MSGSTAT_RESTART)//check second step is performed or not?
        {
           I2caRegs.I2CCNT = 2; // Setup how many bytes to expect
           I2caRegs.I2CMDR.all = 0x6C20;//I2CSTR.ARDY cleared here
           KeyPad_reg.E2PROM_FLAG1.bit.R_Status = I2C_MSGSTAT_READ_BUSY;
        }
    }
    KeyPad_reg.E2PROM_FLAG1.bit.R_Status = I2C_MSGSTAT_SEND_NOSTOP;//clear to first state for next reading
    if(KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==1)
    {
        if(KeyPad_reg.GENERAL_FLAG.bit.End_InitKeyPAd==0)//check after reset
        {
            LoadDefaultE2(0);
        }
    }else
    {
        KeyPad_reg.E2PROM_BUFF_CDRR.bit.Buff_CDRR_H = I2caRegs.I2CDRR.all;
        KeyPad_reg.E2PROM_BUFF_CDRR.bit.Buff_CDRR_L = I2caRegs.I2CDRR.all;
    }

    return KeyPad_reg.E2PROM_BUFF_CDRR.all;
}
//////////////////////////////////////////////////////////////////
void WriteE2prom(Uint16 AdrsH, Uint16 AdrsL, Uint16 Data)
{
    if(KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
    {
        KeyPad_reg.E2PROM_FLAG.bit.Writting = 1;
        TimeOutE2p = 0;
        while (I2caRegs.I2CMDR.bit.STP == 1 &&
               KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
        {
            Chk_TimeOutE2P();
        }
        TimeOutE2p = 0;
        while (I2caRegs.I2CSTR.bit.BB == 1 &&
               KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
        {
            Chk_TimeOutE2P();
        }

        I2caRegs.I2CFFTX.all = 0x0040;  //reset FIFO
        I2caRegs.I2CFFTX.all = 0x6000;  // Enable FIFO mode and TXFIFO
        I2caRegs.I2CSAR.all  = 0x50;
        I2caRegs.I2CCNT      = 4;//send adrsH,L+data
        I2caRegs.I2CDXR.all  = AdrsH;
        I2caRegs.I2CDXR.all  = AdrsL;
        I2caRegs.I2CDXR.all  = Data>>8;//load data high byte
        I2caRegs.I2CDXR.all  = Data & 0x00FF;//load data low byte

        KeyPad_reg.E2PROM_FLAG1.bit.W_Status = I2C_MSGSTAT_WRITE_BUSY;
        I2caRegs.I2CMDR.all = 0x6E20;

        TimeOutE2p = 0;
        while(KeyPad_reg.E2PROM_FLAG1.bit.W_Status !=I2C_MSGSTAT_RESTART &&
               KeyPad_reg.E2PROM_FLAG.bit.EEP_r_w_error==0)
        {
            Chk_TimeOutE2P();
        }
        KeyPad_reg.E2PROM_FLAG.bit.Writting = 0;
    }

}

void Chk_TimeOutE2P(void)
{
    ServiceDog();
    KeyPad_reg.TRIGGER_BIT.bit.TimeOutE2p_Trg=1;
}

void interrupt_I2C(void)
{
    I2C_InterruptSource intSource;
    intSource = I2C_getInterruptSource(I2CA_BASE);

    if(intSource == I2C_SCD_ISRC)//check whether stop
    {
        if(KeyPad_reg.E2PROM_FLAG.bit.Writting == 1)
        {//the system is writing the e2prom
            if (KeyPad_reg.E2PROM_FLAG1.bit.W_Status == I2C_MSGSTAT_WRITE_BUSY)
            {
                KeyPad_reg.E2PROM_FLAG1.bit.W_Status = I2C_MSGSTAT_WRITE_WAIT;
                KeyPad_reg.TRIGGER_BIT.bit.E2Prom_Trg = 1;
            }
        }

        else
        {//the system is reading the e2prom
           if(KeyPad_reg.E2PROM_FLAG1.bit.R_Status == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
           {
              KeyPad_reg.E2PROM_FLAG1.bit.R_Status = I2C_MSGSTAT_SEND_NOSTOP;
           }

           else if (KeyPad_reg.E2PROM_FLAG1.bit.R_Status == I2C_MSGSTAT_READ_BUSY)
           {
              KeyPad_reg.E2PROM_FLAG1.bit.R_Status = I2C_MSGSTAT_BUFFREADY;//I2C module receive data
           }
        }
    }

    else if(intSource == I2C_ARDY_ISRC)//register ready to access
    {
       if(I2caRegs.I2CSTR.bit.NACK == 1)
       {
          I2caRegs.I2CMDR.bit.STP = 1;
          I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
       }

       else if(KeyPad_reg.E2PROM_FLAG.bit.Writting == 0)
       {
            if(KeyPad_reg.E2PROM_FLAG1.bit.R_Status == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
            {
               KeyPad_reg.E2PROM_FLAG1.bit.R_Status = I2C_MSGSTAT_RESTART;
            }
       }

    }
}

//===========================================================================
// End of file.
//===========================================================================



