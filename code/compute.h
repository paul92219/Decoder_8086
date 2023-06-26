#if !defined(COMPUTE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

struct print_content
{
    char Source[32];
    char Dest[32];
    char *Operation;
    uint32 Clocks;
};

struct load_memory
{
    uint8 R_MIndex;
    uint8 REGIndex;
    uint8 WordByte;
    uint16 DispValue;
    uint16 Data;
};

#define COMPUTE_H
#endif
