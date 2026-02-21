
#ifndef DSP280X_I2C_DEFINES_H
#define DSP280X_I2C_DEFINES_H

// Clear Status Flags
#define I2C_CLR_NACK_BIT        0x02

// Interrupt Source Messages
#define I2C_ARDY_ISRC           0x03
#define I2C_SCD_ISRC            0x06

// I2C  Message Commands for I2CMSG struct
#define I2C_MSGSTAT_SEND_WITHSTOP     0x1
#define I2C_MSGSTAT_WRITE_BUSY        0x2
#define I2C_MSGSTAT_WRITE_WAIT        0x3
#define I2C_MSGSTAT_SEND_NOSTOP       0x4
#define I2C_MSGSTAT_SEND_NOSTOP_BUSY  0x5
#define I2C_MSGSTAT_RESTART           0x6
#define I2C_MSGSTAT_READ_BUSY         0x7
#define I2C_MSGSTAT_BUFFREADY         0x8

#endif    // 

//===========================================================================
// End of file.
//===========================================================================
 	
 
 





