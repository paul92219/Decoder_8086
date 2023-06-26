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

struct data
{
    uint16 RegistersBuffer[8];
    uint16 SegmentRegistersBuffer[4];
    uint16 Flags;
    uint16 IPRegister;

    uint8 MemoryBuffer[65536];
    
};


char *MainRegisters[8] =
{"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

char *SubRegisters[8] =
{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };

char *Register_Memory[8] =
{"[bx + si", "[bx + di", "[bp + si", "[bp + di",
 "[si", "[di", "[bp", "[bx"};

char *JumpInstructions[16] =
{"jo", "jno", "jb", "jnb", "jz", "jnz", "jna", "ja",
 "js", "jns", "jp", "jnp", "jl", "jnl", "jng", "jg"};

char *LoopInstructions[4] = {"loopnz", "loopz", "loop", "jcxz"};

char *SegmentRegisters[4] = {"es", "cs", "ss", "ds"};

uint8 OpCodeMasks[4] = {0xff, 0xfe, 0xfc, 0xf0};

uint8 InstructionsOpCode[34] =
{
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    0xe0, 0xe1, 0xe2, 0xe3, 0x88, 0xb0, 0xa0, 0xa2,
    0xc6, 0x28, 0x38, 0x00, 0x3c, 0x2c, 0x04, 0x80,
    0x8e, 0x8c
};

#define DECIDER_H
#endif
