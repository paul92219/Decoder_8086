/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;

#include "decider.h"
#include "register_memory.cpp"

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

int
ReadBinaryFile(char *FileName, uint8 *Buffer)
{
    int Result = 0;
    
    FILE* fp;
    // Open the file in read binary mode
    fp = fopen(FileName, "rb");


    // Check if the file was opened successfully
    if (fp == NULL) {
        printf("Failed to open file %s\n", FileName);
    }
    // Read binary data from file in a loop

    // Allocate buffer to hold binary data
    Result = fread(Buffer, sizeof(uint8), 512, fp);
    fclose(fp);

    return(Result);
}

instruction_content
FindInstructionContent(uint8 *Buffer, uint8 Instruction, values *Values)
{

    uint8 Byte_2 = Buffer[Values->IPRegister + 1];
    uint8 Byte_3 = Buffer[Values->IPRegister + 2];
    uint8 Byte_4 = Buffer[Values->IPRegister + 3];
    uint8 Byte_5 = Buffer[Values->IPRegister + 4];
    uint8 Byte_6 = Buffer[Values->IPRegister + 5];

    instruction_content Result = {};

    uint8 Operation = 0;
    for(int MaskIndex = 0;
        MaskIndex < 4;
        ++MaskIndex)
    {
        for(int InsOpIndex = 0;
            InsOpIndex < 34;
            ++InsOpIndex)
        {
            if((Instruction & OpCodeMasks[MaskIndex]) == InstructionsOpCode[InsOpIndex])
            {
                Operation = Instruction & OpCodeMasks[MaskIndex];
                break;
            }
        }

        if(Operation)
        {
            break;
        }
    }

    switch(Operation)
    {
        // NOTE(handy): Data
        case 0x70: 
        case 0x71: 
        case 0x72: 
        case 0x73: 
        case 0x74: 
        case 0x75: 
        case 0x76: 
        case 0x77: 
        case 0x78: 
        case 0x79: 
        case 0x7a: 
        case 0x7b: 
        case 0x7c: 
        case 0x7d: 
        case 0x7e: 
        case 0x7f: 
        case 0xe0: 
        case 0xe1: 
        case 0xe2: 
        case 0xe3:
        {
            Result.OpCode = Operation;
            Result.Data = Byte_2;
            Values->IPRegister += 1;

        } break;

        // NOTE(handy): mod, SR, R/M, DISP-LO, DISP-HI
        case 0x8e:
        case 0x8c:
        {
            Result.OpCode = Operation;
            Result.MOD = (Byte_2 & 0xc0) >> 6;
            Result.SR = (Byte_2 & 0x18) >> 3;
            Result.R_M = Byte_2 & 0x07;
            Result.Reg = SegmentRegisters[Result.SR];
            Result.RegMem = Register_Memory[Result.R_M];
            Values->IPRegister += 1;

            if((Result.MOD == 0) && (Result.R_M == 6))
            {
                Result.DispLow = Byte_3;
                Result.DispHigh = Byte_4;
                Values->IPRegister += 2;
                Result.DirectAddress = 1;
            }
            else if(Result.MOD == 1)
            {
                Result.DispLow = Byte_3;
                Values->IPRegister += 1;
            }
            else if(Result.MOD == 2)
            {
                Result.DispLow = Byte_3;
                Result.DispHigh = Byte_4;
                Values->IPRegister += 2;
            }

        } break;

        // NOTE(handy): Dbit, WBit, mod|REG|R/M, DISP-LO, DISP-HI
        case 0x88:
        case 0x00:
        case 0x28:
        case 0x38:
        {
            Result.OpCode = Operation;
            Result.DBit = (Instruction & 0x02) >> 1; 
            Result.WBit = Instruction & 0x01; 
            Result.MOD = (Byte_2 & 0xc0) >> 6;
            Result.R_M = Byte_2 & 0x07;
            Result.REG = (Byte_2 & 0x38) >> 3;

    
            if(Result.WBit)
            {
                Result.Reg = MainRegisters[Result.REG];
            }
            else
            {
                Result.Reg = SubRegisters[Result.REG];
            }

            if(!Result.WBit && (Result.MOD == 3))
            {
                Result.RegMem = SubRegisters[Result.R_M];
            }
            else if(Result.MOD == 3)
            {
                Result.RegMem = MainRegisters[Result.R_M];
            }
            
            Values->IPRegister += 1;

            if((Result.MOD == 0) && (Result.R_M == 6))
            {
                Result.DispLow = Byte_3;
                Result.DispHigh = Byte_4;
                Values->IPRegister += 2;
                Result.DirectAddress = 1;
            }
            else if(Result.MOD == 1)
            {
                Result.DispLow = Byte_3;
                Values->IPRegister += 1;
            }
            else if(Result.MOD == 2)
            {
                Result.DispLow = Byte_3;
                Result.DispHigh = Byte_4;
                Values->IPRegister += 2;
            }
            
        } break;

        // NOTE(handy): WBit, REG, Data, DataW
        case 0xb0:
        {
            Result.OpCode = Operation;
            Result.WBit = (Instruction & 0x08) >> 3;
            Result.REG = Instruction & 0x07;
            Result.Data = Byte_2;
            Values->IPRegister += 1;

            if(Result.WBit)
            {
                Result.Reg = MainRegisters[Result.REG];
                Result.DataW = Byte_3;
                Values->IPRegister += 1;
            }
            else
            {
                Result.Reg = SubRegisters[Result.REG];
            }

        } break;

        // NOTE(handy): WBit, addr-lo, addr-hi
        case 0xa0:
        case 0xa2:
        {
            Result.OpCode = Operation;
            Result.WBit = Instruction & 0x01;
            Result.DispLow = Byte_2;
            Result.DispHigh = Byte_3;
            Values->IPRegister += 2;

        } break;

        // NOTE(handy): WBit, Data, DataW
        case 0x3c:
        case 0x2c:
        case 0x04:
        {
            Result.OpCode = Operation;
            Result.WBit = Instruction & 0x01;
            Result.Data = Byte_2;
            Values->IPRegister += 1;

            if(Result.WBit)
            {
                Result.DataW = Byte_3;
                Values->IPRegister += 1;
            }

        } break;

        // NOTE(handy):
        // WBit, MOD, R/M, DISP-LO, DISP-HI, Data, DataW
        // if 0x80 SBit, WBit, MOD, R/M, DISP-LO, DISP-HI, Data, DataSW 
        case 0xc6:
        case 0x80:
        {
            if(Operation == 0x80)
            {
                Result.OpCode = Operation;
                Result.SBit = (Instruction & 0x02) >> 1;
                Result.WBit = Instruction & 0x01;
                Result.MOD = (Byte_2 & 0xc0) >> 6;
                Result.REG = (Byte_2 & 0x38) >> 3;
                Result.R_M = Byte_2 & 0x07;
                Values->IPRegister += 1;
                        
                if(Result.WBit && !Result.SBit)
                {
                    if(Result.MOD == 0)
                    {
                        if(Result.R_M == 6)
                        {
                            Result.DispLow = Byte_3;
                            Result.DispHigh = Byte_4;
                            Result.Data = Byte_5;
                            Result.DataWS = Byte_6;
                            Values->IPRegister += 4;
                            Result.DirectAddress = 1;
                        }
                        else
                        {
                            Result.Data = Byte_3;
                            Result.DataWS = Byte_4;
                            Values->IPRegister += 2;
                        }
                    }
                    else if(Result.MOD == 1)
                    {
                        Result.DispLow = Byte_3;
                        Result.Data = Byte_4;
                        Result.DataWS = Byte_5;
                        Values->IPRegister += 3;
                        
                    }
                    else if(Result.MOD == 2)
                    {
                        Result.DispLow = Byte_3;
                        Result.DispHigh = Byte_4;
                        Result.Data = Byte_5;
                        Result.DataWS = Byte_6;
                        Values->IPRegister += 4;
                    }
                    else if(Result.MOD == 3)
                    {
                        Result.Data = Byte_3;
                        Result.DataWS = Byte_4;
                        Values->IPRegister += 2;
                    }
                }
                else
                {
                    if(Result.MOD == 0)
                    {
                        if(Result.R_M == 6)
                        {
                            Result.DispLow = Byte_3;
                            Result.DispHigh = Byte_4;
                            Result.Data = Byte_5;
                            Values->IPRegister += 3;
                            Result.DirectAddress = 1;
                        }
                        else
                        {
                            Result.Data = Byte_3;
                            Values->IPRegister += 1;
                        }
                    }
                    else if(Result.MOD == 1)
                    {
                        Result.DispLow = Byte_3;
                        Result.Data = Byte_4;
                        Values->IPRegister += 2;
                        
                    }
                    else if(Result.MOD == 2)
                    {
                        Result.DispLow = Byte_3;
                        Result.DispHigh = Byte_4;
                        Result.Data = Byte_5;
                        Values->IPRegister += 3;
                    }
                    else if(Result.MOD == 3)
                    {
                        Result.Data = Byte_3;
                        Values->IPRegister += 1;
                    }
                }
            }
            else
            {
                Result.OpCode = Operation;
                Result.WBit = Instruction & 0x01;
                Result.MOD = (Byte_2 & 0xc0) >> 6;
                Result.R_M = Byte_2 & 0x07;
                Values->IPRegister += 1;

                if(Result.WBit)
                {
                    if(Result.MOD == 0)
                    {
                        if(Result.R_M == 6)
                        {
                            Result.DispLow = Byte_3;
                            Result.DispHigh = Byte_4;
                            Result.Data = Byte_5;
                            Result.DataW = Byte_6;
                            Values->IPRegister += 4;
                            Result.DirectAddress = 1;
                        }
                        else
                        {
                            Result.Data = Byte_3;
                            Result.DataW = Byte_4;
                            Values->IPRegister += 2;
                        }
                    }
                    else if(Result.MOD == 1)
                    {
                        Result.DispLow = Byte_3;
                        Result.Data = Byte_4;
                        Result.DataW = Byte_5;
                        Values->IPRegister += 3;
                    }
                    else if(Result.MOD == 2)
                    {
                        Result.DispLow = Byte_3;
                        Result.DispHigh = Byte_4;
                        Result.Data = Byte_5;
                        Result.DataW = Byte_6;
                        Values->IPRegister += 4;
                    }
                }
                else
                {
                    if(Result.MOD == 0)
                    {
                        if(Result.R_M == 6)
                        {
                            Result.DispLow = Byte_3;
                            Result.DispHigh = Byte_4;
                            Result.Data = Byte_5;
                            Values->IPRegister += 3;
                            Result.DirectAddress = 1;
                        }
                        else
                        {
                            Result.Data = Byte_3;
                            Values->IPRegister += 1;
                        }
                    }
                    else if(Result.MOD == 1)
                    {
                        Result.DispLow = Byte_3;
                        Result.Data = Byte_4;
                        Values->IPRegister += 2;
                    }
                    else if(Result.MOD == 2)
                    {
                        Result.DispLow = Byte_3;
                        Result.DispHigh = Byte_4;
                        Result.Data = Byte_5;
                        Values->IPRegister += 3;
                    }
                }
            }
            
        } break;
        
    };

    return(Result);
}

int main()
{
    char FileName[] = "D:\\Projects\\Decoder_8086\\data\\listing_53";
    uint8 GlobalBuffer[512] = {};
    int End = ReadBinaryFile(FileName, GlobalBuffer);

    registers Registers = {};
    Registers.MainRegisters = MainRegisters;
    Registers.Register_Memory = Register_Memory;
    Registers.JumpInstructions = JumpInstructions;
    Registers.LoopInstructions = LoopInstructions;
    Registers.SegmentRegisters = SegmentRegisters;
    Registers.SubRegisters = SubRegisters;
    
    values RegistersValues = {};
    
    while(RegistersValues.IPRegister != End)
    {
        uint8 Instruction = GlobalBuffer[RegistersValues.IPRegister];
        instruction_content IC = FindInstructionContent(GlobalBuffer, Instruction, &RegistersValues);
        RegistersValues.IPRegister++;

        DetermineOperation(IC, &Registers, &RegistersValues);
    }
#if 1
    printf("Finaly registers:\n");
    for(int RegIndex = 0;
        RegIndex < 8;
        ++RegIndex)
    {
        if(RegIndex == 8)
        {
            printf("\n");
        }
        printf("    %s: 0x%x (%u)\n", MainRegisters[RegIndex], RegistersValues.RegistersValue[RegIndex],
               RegistersValues.RegistersValue[RegIndex]);
    }

    for(int SegRegIndex = 0;
        SegRegIndex < 4;
        ++SegRegIndex)
    {
        printf("    %s: 0x%x (%u)\n", SegmentRegisters[SegRegIndex],
               RegistersValues.SegmentRegistersValue[SegRegIndex],
               RegistersValues.SegmentRegistersValue[SegRegIndex]);
    }

    printf("Flags: %x\n", RegistersValues.Flags);
    printf("IP: %u", RegistersValues.IPRegister);
#endif
    return(0);
}
