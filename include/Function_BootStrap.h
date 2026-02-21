
#ifndef FUNCTION_BOOTSTRAP_H
#define FUNCTION_BOOTSTRAP_H

//------------------------------------------------------------------------------------------------------------
struct BootStrap_Flag1_bit{ 	// bits   description 
   Uint16 CmpltBtStrp:1;		//Completed boot strap bit
   Uint16 CycleBtStrap:3;		//Number of Cycle bootstrap operation
   Uint16 CntrCycleBtStrap:3;	//Counter of Cycle bootstrap operation
   Uint16 PhaseBtStrp:2;		//Index phase turn-on bootstrap
   Uint16 StateBtStrp:2;		//state of bootstrap
   Uint16 OnX:1;				//TurnOn BootStrap X
   Uint16 OnY:1;				//TurnOn BootStrap Y
   Uint16 OnZ:1;				//TurnOn BootStrap Z
   Uint16 TrnOnBtStrp:1;		//TurnOn PWM in bootstrap
} ;

union BootStrap_Flag1 {
	Uint16	all;
	struct BootStrap_Flag1_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------
struct BootStrap_Flag2_bit{ 		// bits   description 
	Uint16 TrnOnBtStrp:6;			//Turn-on time of bootstrap
	Uint16 CntrTrnOnBtStrp:6;		//Counter Turn-on time of bootstrap
	Uint16 DelayCmpltdBtStrp:3;		//Delay completed boot strap
} ;

union BootStrap_Flag2 {
	Uint16	all;
	struct BootStrap_Flag2_bit	bit;
} ; 
//------------------------------------------------------------------------------------------------------------ 
   
struct BOOTSTRAP_REG{
	union BootStrap_Flag1	BTSTRP_FLAG1;
	union BootStrap_Flag2	BTSTRP_FLAG2;
	Uint32 					BufferIO;		//Buffer IO BootStrap
};
extern struct BOOTSTRAP_REG BtStrp_reg;
#endif  // 

//===========================================================================
// End of file.
//===========================================================================
