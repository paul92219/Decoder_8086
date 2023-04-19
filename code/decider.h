#if !defined(DECIDER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

struct instruction_content
{
    uint8 OpCode;
    uint8 DBit;
    uint8 WBit;
    uint8 SBit;
    uint8 MOD;
    uint8 REG;
    uint8 R_M;
    uint8 DispLow;
    uint8 DispHigh;
    uint8 Data;
    uint8 DataW;
    uint8 DataWS;
    uint8 SR;

    uint8 DirectAddress;
    char *Reg;
    char *RegMem;
};

struct registers
{
    char **MainRegisters;
    char **Register_Memory;
    char **JumpInstructions;
    char **LoopInstructions;
    char **SegmentRegisters;
    char **SubRegisters;
};

struct values
{
    uint16 RegistersValue[8];
    uint16 SegmentRegistersValue[4];
    uint16 Flags;
    uint16 IPRegister;

    uint8 Memory[65536];
    
};
#define DECIDER_H
#endif
