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

void
AddSubCmpRegisterMemoryToFromRegister(uint16 Instruction, uint16 Operation, FILE* fp)
{
    uint16 DBitMask = 0x0200;
    uint16 WBitMask = 0x0100;

    uint16 REG = Instruction & 0x0038;
    REG = REG >> 3;
    uint16 R_M = Instruction & 0x0007;

    uint16 ModMask = 0x00c0;
    uint16 MOD = (Instruction & ModMask) >> 6;

    unsigned char buffer[2];
    unsigned char buf[1];
    
//        printf("REG: %u\nR/M: %x\nMOD: %x\n", REG, R_M, MOD);
    
    uint16 REGIndex = 0; 
    if(Instruction & WBitMask)
    {
        REGIndex = 8 | REG;  
    }
    else
    {
        REGIndex = REG;
    }
    
    if(MOD == 3)
    {

        uint16 R_MIndex = 0;
        if(Instruction & WBitMask)
        {
            R_MIndex = 8 | R_M;
        }
        else
        {
            R_MIndex = R_M;
        }

        if(Instruction & DBitMask)
        {
            if(Operation == 0x0000)
            {
                printf("add %s, %s\n", Registers[REGIndex], Registers[R_MIndex]);
            }
            else if(Operation == 0x3800)
            {
                printf("cmp %s, %s\n", Registers[REGIndex], Registers[R_MIndex]);
            }
            else
            {
                printf("sub %s, %s\n", Registers[REGIndex], Registers[R_MIndex]);
            }
        }
        else
        {
            if(Operation == 0x0000)
            {
                printf("add %s, %s\n", Registers[R_MIndex], Registers[REGIndex]);
            }
            else if(Operation == 0x3800)
            {
                printf("cmp %s, %s\n", Registers[R_MIndex], Registers[REGIndex]);
            }
            else
            {
                printf("sub %s, %s\n", Registers[R_MIndex], Registers[REGIndex]);
            }
        }
    }

    else if(MOD == 1)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint8 OneByteDis = (uint8)buf[0]; 

        if(Instruction & DBitMask)
        {
            if(OneByteDis)
            {
                if(Operation == 0x0000)
                {
                    printf("add %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                           (signed char)OneByteDis);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                           (signed char)OneByteDis);
                }
                else
                {
                    printf("sub %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                           (signed char)OneByteDis);
                }
            }
            else
            {
                if(Operation == 0x0000)
                {
                    printf("add %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
                }
                else
                {
                    printf("sub %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
                }
            }
        }
        else
        {
            if(OneByteDis)
            {
                if(Operation == 0x0000)
                {
                    printf("add %s %+d], %s\n", Register_Memory[R_M], (signed char)OneByteDis,
                           Registers[REGIndex]);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s %+d], %s\n", Register_Memory[R_M], (signed char)OneByteDis,
                           Registers[REGIndex]);
                }
                else
                {
                    printf("sub %s %+d], %s\n", Register_Memory[R_M], (signed char)OneByteDis,
                           Registers[REGIndex]);
                }
            }
            else
            {
                if(Operation == 0x0000)
                {
                    printf("add %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
                }
                else
                {
                    printf("sub %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
                }
            }
        }
    }

    else if(MOD == 2)
    {
        if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

        if(Instruction & DBitMask)
        {
            if(Operation == 0x0000)
            {
                printf("add %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                       (signed short)TwoBytesDis);
            }
            else if(Operation == 0x3800)
            {
                printf("cmp %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                       (signed short)TwoBytesDis);
            }
            else
            {
                printf("sub %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                       (signed short)TwoBytesDis);
            }
        }
        else
        {
            if(Operation == 0x0000)
            {
                printf("add %s %+d], %s\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       Registers[REGIndex]);
            }
            else if(Operation == 0x3800)
            {
                printf("cmp %s %+d], %s\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       Registers[REGIndex]);
            }
            else
            {
                printf("sub %s %+d], %s\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       Registers[REGIndex]);
            }
        }
    }
    else if(MOD == 0)
    {
        if(R_M == 6)
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);
      
            }

            uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

            if(Instruction & DBitMask)
            {
                if(Operation == 0x0000)
                {
                    printf("add %s, [%d]\n", Registers[REGIndex], (signed short)TwoBytesDis);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s, [%d]\n", Registers[REGIndex], (signed short)TwoBytesDis);
                }
                else
                {
                    printf("sub %s, [%d]\n", Registers[REGIndex], (signed short)TwoBytesDis);
                }
            }
            else
            {
                if(Operation == 0x0000)
                {
                    printf("add [%d], %s\n", (signed short)TwoBytesDis, Registers[REGIndex]);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp [%d], %s\n", (signed short)TwoBytesDis, Registers[REGIndex]);
                }
                else
                {
                    printf("sub [%d], %s\n", (signed short)TwoBytesDis, Registers[REGIndex]);
                }
            }
        }
        else
        {
            if(Instruction & DBitMask)
            {
                if(Operation == 0x0000)
                {
                    printf("add %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
                }
                else
                {
                    printf("sub %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
                }
            }
            else
            {
                if(Operation == 0x0000)
                {
                    printf("add %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
                }
                else if(Operation == 0x3800)
                {
                    printf("cmp %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
                }
                else
                {
                    printf("sub %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
                }
            }

        }
    }
}

void
AddSubCmpImmediateToRegisterMemory(uint16 Instruction, uint16 Operation, FILE* fp)
{
    uint16 WBitMask = 0x0100;
    uint16 SBitMask = 0x0200;
    
    uint16 R_M = Instruction & 0x0007;

    uint16 ModMask = 0x00c0;
    uint16 MOD = (Instruction & ModMask) >> 6;

    unsigned char buffer[2];
    unsigned char buf[1];
    
//        printf("REG: %u\nR/M: %x\nMOD: %x\n", REG, R_M, MOD);
    if(MOD == 0)
    {
        if((Instruction & WBitMask) && !(Instruction & SBitMask))
        {
            if(R_M == 6)
            {
                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 

                if(Operation == 0x8000)
                {
                    printf("add word [%d], %d\n", (signed short)TwoBytesDis, (signed short)Data);
                }
                else if(Operation == 0x8038)
                {
                    printf("cmp word [%d], %d\n", (signed short)TwoBytesDis, (signed short)Data);
                }
                else
                {
                    printf("sub word [%d], %d\n", (signed short)TwoBytesDis, (signed short)Data);
                }
            }
            else
            {

                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 

                if(Operation == 0x8000)
                {
                    printf("add word %s], %d\n", Register_Memory[R_M], (signed short)Data);
                }
                else if(Operation == 0x8038)
                {
                    printf("cmp word %s], %d\n", Register_Memory[R_M], (signed short)Data);
                }
                else
                {
                    printf("sub word %s], %d\n", Register_Memory[R_M], (signed short)Data);
                }
            }
        }
        else
        {
            if(R_M == 6)
            {
                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

                if(fread(buf, sizeof(buf), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint8 Data = (uint8)buf[0]; 

                if(Operation == 0x8000)
                {
                    printf("add word [%d], %d\n", (signed short)TwoBytesDis, (signed char)Data);
                }
                else if(Operation == 0x8038)
                {
                    printf("cmp word [%d], %d\n", (signed short)TwoBytesDis, (signed char)Data);
                }
                else
                {
                    printf("sub word [%d], %d\n", (signed short)TwoBytesDis, (signed char)Data);
                }
            }
            else
            {

                if(fread(buf, sizeof(buf), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint8 Data = (uint8)buf[0]; 

                if(Operation == 0x8000)
                {
                    printf("add byte %s], %d\n", Register_Memory[R_M], (signed char)Data);
                }
                else if(Operation == 0x8038)
                {
                    printf("cmp byte %s], %d\n", Register_Memory[R_M], (signed char)Data);
                }
                else
                {
                    printf("sub byte %s], %d\n", Register_Memory[R_M], (signed char)Data);
                }
            }

        }
    }
            
    else if(MOD == 1)
    {
                                
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }
        uint8 OneByteDis = (uint8)buf[0]; 

        if((Instruction & WBitMask) && !(Instruction & SBitMask))
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 


            if(Operation == 0x8000)
            {
                printf("add byte %s %+d], %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                       (signed short)Data);
            }
            else if(Operation == 0x8038)
            {
                printf("cmp byte %s %+d], %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                       (signed short)Data);
            }
            else
            {
                printf("sub byte %s %+d], %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                       (signed short)Data);
            }
        }
        else
        {
            if(fread(buf, sizeof(buf), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint8 Data = (uint8)buf[0]; 


            if(Operation == 0x8000)
            {
                printf("add byte %s %+d], %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                       (signed char)Data);
            }
            else if(Operation == 0x8038)
            {
                printf("cmp byte %s %+d], %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                       (signed char)Data);
            }
            else
            {
                printf("sub byte %s %+d], %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                       (signed char)Data);
            }
        }
    }

    else if(MOD == 2)
    {
                                
        if (fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }
        uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

        if((Instruction & WBitMask) && !(Instruction & SBitMask))
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 


            if(Operation == 0x8000)
            {
                printf("add word %s %+d], %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       (signed short)Data);
            }
            else if(Operation == 0x8038)
            {
                printf("cmp word %s %+d], %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       (signed short)Data);
            }
            else
            {
                printf("sub word %s %+d], %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       (signed short)Data);
            }
        }
        else
        {
            if(fread(buf, sizeof(buf), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint8 Data = (uint8)buf[0]; 


            if(Operation == 0x8000)
            {
                printf("add word %s %+d], %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       (signed char)Data);
            }
            else if(Operation == 0x8038)
            {
                printf("cmp word %s %+d], %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       (signed char)Data);
            }
            else
            {
                printf("sub word %s %+d], %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                       (signed char)Data);
            }
        }
    }

    else if(MOD == 3)
    {
        
        uint16 REGIndex = 0; 
        if(Instruction & WBitMask)
        {
            REGIndex = 8 | R_M;  
        }
        else
        {
            REGIndex = R_M;
        }

        if((Instruction & WBitMask) && !(Instruction & SBitMask))
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 


            if(Operation == 0x8000)
            {
                printf("add %s, %d\n", Registers[REGIndex], (signed short)Data);            
            }
            else if(Operation == 0x8038)
            {
                printf("cmp %s, %d\n", Registers[REGIndex], (signed short)Data);            
            }
            else
            {
                printf("sub %s, %d\n", Registers[REGIndex], (signed short)Data);            
            }
        }
        else
        {
            if(fread(buf, sizeof(buf), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint8 Data = (uint8)buf[0]; 


            if(Operation == 0x8000)
            {
                printf("add %s, %d\n", Registers[REGIndex], (signed char)Data);
            }
            else if(Operation == 0x8038)
            {
                printf("cmp %s, %d\n", Registers[REGIndex], (signed char)Data);
            }
            else
            {
                printf("sub %s, %d\n", Registers[REGIndex], (signed char)Data);
            }
        }
    }
}

void
AddSubCmpMemoryToAccumulator(uint16 Instruction, uint16 Operation, FILE* fp)
{
    uint16 WBitMask = 0x0100;
    unsigned char buf[1];
    if(Instruction & WBitMask)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (Instruction & 0x00ff)  | ((uint16)buf[0] << 8); 

        if(Operation == 0x0400)
        {
            printf("add ax, %d\n", (signed short)TwoBytesDis);
        }
        else if(Operation == 0x3c00)
        {
            printf("cmp ax, %d\n", (signed short)TwoBytesDis);
        }
        else
        {
            printf("sub ax, %d\n", (signed short)TwoBytesDis);
        }
    }
    else
    {
        uint8 OneByteDis = (Instruction & 0x00ff); 

        if(Operation == 0x0400)
        {
            printf("add al, %d\n", (signed char)OneByteDis);
        }
        else if(Operation == 0x3c00)
        {
            printf("cmp al, %d\n", (signed char)OneByteDis);
        }
        else
        {
            printf("sub al, %d\n", (signed char)OneByteDis);
        }
    }
}

void
RegisterMemoryToFromRegister(uint16 Instruction, FILE* fp)
{
    uint16 DBitMask = 0x0200;
    uint16 WBitMask = 0x0100;

    uint16 REG = Instruction & 0x0038;
    REG = REG >> 3;
    uint16 R_M = Instruction & 0x0007;

    uint16 ModMask = 0x00c0;
    uint16 MOD = (Instruction & ModMask) >> 6;

    unsigned char buffer[2];
    unsigned char buf[1];
    
//        printf("REG: %u\nR/M: %x\nMOD: %x\n", REG, R_M, MOD);

        
    uint16 REGIndex = 0; 
    if(Instruction & WBitMask)
    {
        REGIndex = 8 | REG;  
    }
    else
    {
        REGIndex = REG;
    }
    
    if(MOD == 3)
    {

        uint16 R_MIndex = 0;
        if(Instruction & WBitMask)
        {
            R_MIndex = 8 | R_M;
        }
        else
        {
            R_MIndex = R_M;
        }

        if(Instruction & DBitMask)
        {
            printf("mov %s, %s\n", Registers[REGIndex], Registers[R_MIndex]);
        }
        else
        {
            printf("mov %s, %s\n", Registers[R_MIndex], Registers[REGIndex]);
        }
    }

    else if(MOD == 1)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint8 OneByteDis = (uint8)buf[0]; 

        if(Instruction & DBitMask)
        {
            if(OneByteDis)
            {
                printf("mov %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                       (signed char)OneByteDis);
            }
            else
            {
                printf("mov %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
            }
        }
        else
        {
            if(OneByteDis)
            {
                printf("mov %s %+d], %s\n", Register_Memory[R_M], (signed char)OneByteDis,
                       Registers[REGIndex]);
            }
            else
            {
                printf("mov %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
            }
        }
    }

    else if(MOD == 2)
    {
        if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

        if(Instruction & DBitMask)
        {
            printf("mov %s, %s %+d]\n", Registers[REGIndex], Register_Memory[R_M],
                   (signed short)TwoBytesDis);
        }
        else
        {
            printf("mov %s %+d], %s\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                   Registers[REGIndex]);
        }
    }
    else if(MOD == 0)
    {
        if(R_M == 6)
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);
      
            }

            uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

            if(Instruction & DBitMask)
            {
                printf("mov %s, [%d]\n", Registers[REGIndex], (signed short)TwoBytesDis);
            }
            else
            {
                printf("mov [%d], %s\n", (signed short)TwoBytesDis, Registers[REGIndex]);
            }
        }
        else
        {
            if(Instruction & DBitMask)
            {
                printf("mov %s, %s]\n", Registers[REGIndex], Register_Memory[R_M]);
            }
            else
            {
                printf("mov %s], %s\n", Register_Memory[R_M], Registers[REGIndex]);
            }

        }
    }
}

void
ImmediateToRegisterMemory(uint16 Instruction, FILE* fp)
{
    uint16 WBitMask = 0x0100;

    uint16 R_M = Instruction & 0x0007;

    uint16 ModMask = 0x00c0;
    uint16 MOD = (Instruction & ModMask) >> 6;

    unsigned char buffer[2];
    unsigned char buf[1];
    
//        printf("REG: %u\nR/M: %x\nMOD: %x\n", REG, R_M, MOD);
    if(MOD == 0)
    {
        if(Instruction & WBitMask)
        {
            if(R_M == 6)
            {
                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 

                printf("mov [%d], word [%d]\n", (signed short)TwoBytesDis, (signed short)Data);
            }
            else
            {

                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 

                printf("mov %s], word [%d]\n", Register_Memory[R_M], (signed short)Data);

            }
        }
        else
        {
            if(R_M == 6)
            {
                if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

                if(fread(buf, sizeof(buf), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint8 Data = (uint8)buf[0]; 

                printf("mov [%d], byte [%d]\n", (signed short)TwoBytesDis, (signed char)Data);
            }
            else
            {

                if(fread(buf, sizeof(buf), 1, fp) != 1) {
                    printf("Failed to read data from file\n");
                    fclose(fp);
      
                }

                uint8 Data = (uint8)buf[0]; 

                printf("mov %s], byte %d\n", Register_Memory[R_M], (signed char)Data);

            }

        }
    }
            
    else if(MOD == 1)
    {
                                
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }
        uint8 OneByteDis = (uint8)buf[0]; 

        if(Instruction & WBitMask)
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 

                        
            printf("mov %s %+d], word %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                   (signed short)Data);
        }
        else
        {
            if(fread(buf, sizeof(buf), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint8 Data = (uint8)buf[0]; 

                        
            printf("mov %s %+d], byte %d\n", Register_Memory[R_M], (signed char)OneByteDis,
                   (signed char)Data);
        }
    }

    else if(MOD == 2)
    {
                                
        if (fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }
        uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 

        if(Instruction & WBitMask)
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint16 Data = (((uint16)buffer[1] << 8)  | buffer[0]); 

                        
            printf("mov %s %+d], word %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                   (signed short)Data);
        }
        else
        {
            if(fread(buf, sizeof(buf), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);      
            }

            uint8 Data = (uint8)buf[0]; 

                        
            printf("mov %s %+d], byte %d\n", Register_Memory[R_M], (signed short)TwoBytesDis,
                   (signed char)Data);
        }
    }
}

void
ImmediateToRegister(uint16 Instruction, FILE* fp)
{
    uint16 WBitMask = 0x0800;
    uint16 REG = (Instruction & 0x0700) >> 8;
    unsigned char buf[1];
    
    if(((Instruction & WBitMask) >> 11) == 1)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }
        uint16 Data;
        if(buf[0] != 0)
        {
            Data = (Instruction & 0x00ff) | ((uint16)buf[0] << 8);
        }
        else
        {
            Data = Instruction & 0x0ff;
        }
        REG = REG | 8;
        printf("mov %s, %u\n", Registers[REG], Data);
    }
    else
    {
        uint16 Data = Instruction & 0x00ff;
        printf("mov %s, %u\n", Registers[REG], Data);
    }
}

void
MemoryToAccumulator(uint16 Instruction, FILE* fp)
{
    uint16 WBitMask = 0x0100;
    unsigned char buf[1];
    if(Instruction & WBitMask)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (Instruction & 0x00ff)  | ((uint16)buf[0] << 8); 

        printf("mov ax, [%d]\n", (signed short)TwoBytesDis);
    }
    else
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (Instruction & 0x00ff)  | ((uint16)buf[0] << 8); 

        printf("mov al, [%d]\n", (signed short)TwoBytesDis);
    }
}

void
AccumulatorToMemory(uint16 Instruction, FILE* fp)
{
    uint16 WBitMask = 0x0100;
    unsigned char buf[1];
    if(Instruction & WBitMask)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (Instruction & 0x00ff)  | ((uint16)buf[0] << 8); 

        printf("mov [%d], ax\n", (signed short)TwoBytesDis);
    }
    else
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (Instruction & 0x00ff)  | ((uint16)buf[0] << 8); 

        printf("mov [%d], al\n", (signed short)TwoBytesDis);
    }
}

void
JumpAndLoop(uint16 Instruction, uint16 Operation, FILE *fp)
{
    
    if(Operation > 0x7f00)
    {
        int Index = (uint8)(Operation >> 8) - 224;
        uint8 IP = Instruction & 0x00ff;
        char *Ins = LoopInstructions[Index];
        printf("%s $+%u\n", Ins, IP + 2);
    }
    else
    {
        int Index = (uint8)(Operation >> 8) - 112;
        uint8 IP = Instruction & 0x00ff;
        char *Ins = JumpInstructions[Index];
        printf("%s $+%u\n", Ins, IP + 2);
    }
}

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
                JumpAndLoop(Instruction, Operation, fp);
            } break;
            
            case 0x8800:
            {
                RegisterMemoryToFromRegister(Instruction, fp);
            } break;

            case 0xb000:
            {
                ImmediateToRegister(Instruction, fp);
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
                ImmediateToRegisterMemory(Instruction, fp);
            } break;

            case 0x0000:
            {
                AddSubCmpRegisterMemoryToFromRegister(Instruction, Operation, fp);
            } break;

            case 0x0400:
            {
                AddSubCmpMemoryToAccumulator(Instruction, Operation, fp);
            } break;

            case 0x8000:
            {
                AddSubCmpImmediateToRegisterMemory(Instruction, Operation, fp);            
            } break;

            case 0x2800:
            {
                AddSubCmpRegisterMemoryToFromRegister(Instruction, Operation, fp);
            } break;

            case 0x2c00:
            {
                AddSubCmpMemoryToAccumulator(Instruction, Operation, fp);
            } break;

            case 0x8028:
            {
                AddSubCmpImmediateToRegisterMemory(Instruction, Operation, fp);            
            } break;

            case 0x3800:
            {
                AddSubCmpRegisterMemoryToFromRegister(Instruction, Operation, fp);
            } break;

            case 0x3c00:
            {
                AddSubCmpMemoryToAccumulator(Instruction, Operation, fp);
            } break;

            case 0x8038:
            {
                AddSubCmpImmediateToRegisterMemory(Instruction, Operation, fp);            
            } break;
        }
    }
    
    fclose(fp);
    
    return(0);
}
