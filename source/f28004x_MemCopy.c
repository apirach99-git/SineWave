/*
 * f28004x_MemCopy.c
 *
 *  Created on: Dec 22, 2022
 *      Author: Piyasak
 */

#include "F28x_Project.h"

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;  //copy value in addrs from source(FLASHD) to dest(RAML0)
    }
    return;
}


