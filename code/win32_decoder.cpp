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

#include "accumulator.cpp"
#include "register_memory.cpp"
#include "immediate_register_memory.cpp"
#include "other_instructions.cpp"

char *Registers[16] =
{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
 "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};


char *Register_Memory[8] =
{"[bx + si", "[bx + di", "[bp + si", "[bp + di",
 "[si", "[di", "[bp", "[bx"};

char *JumpInstructions[16] =
{"jo", "jno", "jb", "jnb", "jz", "jnz", "jna", "ja",
 "js", "jns", "jp", "jnp", "jl", "jnl", "jng", "jg"};

char *LoopInstructions[4] = {"loopnz", "loopz", "loop", "jcxz"};

int main()
{
    FILE* fp;
    uint16 Instruction;
    char filename[] = "D:\\Projects\\Decoder_8086\\misc\\many_instructions";


    // Open the file in read binary mode
    fp = fopen(filename, "rb");

    int num_values = 118; // Number of 16-bit values to read
    
    // Check if the file was opened successfully
    if (fp == NULL) {
        printf("Failed to open file %s\n", filename);
        return 1;
    }

    // Allocate buffer to hold binary data
    unsigned char buffer[2];
    unsigned char buf[1];
                
    // Read binary data from file in a loop
    for (int i = 0; i < num_values; i++)
    {

        if (fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
            return 1;
        }
        
        // Convert binary data to uint16
        Instruction = (((uint16)buffer[0] << 8) | buffer[1]);
        
        uint16 Operation;
        if(((Instruction & 0xfc00) == 0x8800) || ((Instruction & 0xfc00) == 0x0000) ||
           ((Instruction & 0xfc00) == 0x2800) || ((Instruction & 0xfc00) == 0x3800))
        {
            Operation = Instruction & 0xfc00;
        }
        else if(((Instruction & 0xfe00) == 0xa000) || ((Instruction & 0xfe00) == 0xa200) ||
                ((Instruction & 0xfe00) == 0x0400) || ((Instruction & 0xfe00) == 0x2c00) ||
                ((Instruction & 0xfe00) == 0x3c00))
        {
            Operation = Instruction & 0xfe00;
        }
        else if(((Instruction & 0xfc38) == 0x8000) || ((Instruction & 0xfc38) == 0x8028) ||
                ((Instruction & 0xfc38) == 0x8038))
        {
            Operation = Instruction & 0xfc38;
        }
        else if(((Instruction & 0xf000) == 0xb000))
        {
            Operation = Instruction & 0xb000;
        }
        else if(((Instruction & 0xfe38) == 0xc600))
        {
            Operation = Instruction & 0xc600;
        }
        else
        {
            Operation = Instruction & 0xff00;
        }

        switch(Operation)
        {
            case 0x7000:
            case 0x7100:
            case 0x7200:
            case 0x7300:
            case 0x7400:
            case 0x7500:
            case 0x7600:
            case 0x7700:
            case 0x7800:
            case 0x7900:
            case 0x7a00:
            case 0x7b00:
            case 0x7c00:
            case 0x7d00:
            case 0x7e00:
            case 0x7f00:
            case 0xe000:
            case 0xe100:
            case 0xe200:
            case 0xe300:
            {
                JumpAndLoop(Instruction, Operation, fp, LoopInstructions,
                            JumpInstructions);
            } break;
            
            case 0x8800:
            {
                RegisterMemoryToFromRegister(Instruction, fp, Registers, Register_Memory);
            } break;

            case 0xb000:
            {
                ImmediateToRegister(Instruction, fp, Registers);
            } break;

            case 0xa000:
            {
                MemoryToAccumulator(Instruction, fp);
            } break;

            case 0xa200:
            {
                AccumulatorToMemory(Instruction, fp);
            } break;

            case 0xc600:
            {
                ImmediateToRegisterMemory(Instruction, fp, Registers, Register_Memory);
            } break;

            case 0x0000:
            {
                AddSubCmpRegisterMemoryToFromRegister(Instruction, Operation, fp,
                                                      Registers, Register_Memory);
            } break;

            case 0x0400:
            {
                AddSubCmpMemoryToAccumulator(Instruction, Operation, fp);
            } break;

            case 0x8000:
            {
                AddSubCmpImmediateToRegisterMemory(Instruction, Operation, fp,
                                                   Registers, Register_Memory);            
            } break;

            case 0x2800:
            {
                AddSubCmpRegisterMemoryToFromRegister(Instruction, Operation, fp,
                                                      Registers, Register_Memory);
            } break;

            case 0x2c00:
            {
                AddSubCmpMemoryToAccumulator(Instruction, Operation, fp);
            } break;

            case 0x8028:
            {
                AddSubCmpImmediateToRegisterMemory(Instruction, Operation, fp,
                                                   Registers, Register_Memory);            
            } break;

            case 0x3800:
            {
                AddSubCmpRegisterMemoryToFromRegister(Instruction, Operation, fp,
                                                      Registers, Register_Memory);
            } break;

            case 0x3c00:
            {
                AddSubCmpMemoryToAccumulator(Instruction, Operation, fp);
            } break;

            case 0x8038:
            {
                AddSubCmpImmediateToRegisterMemory(Instruction, Operation, fp,
                                                   Registers, Register_Memory);            
            } break;
        }
    }
    
    fclose(fp);
    
    return(0);
}
