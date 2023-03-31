/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

void
AddSubCmpRegisterMemoryToFromRegister(uint16 Instruction, uint16 Operation, FILE* fp, char *Registers[],
                                      char *Register_Memory[])
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

uint16 *
RegisterMemoryToFromRegister(uint16 Instruction, FILE* fp, char *Registers[], char *Register_Memory[],
                             uint16 *RegistersValue)
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
            if((REGIndex < 7) && (R_MIndex < 7))
            {
                if((REGIndex < 4) && (R_MIndex < 4))
                {
                    RegistersValue[REGIndex] = ((RegistersValue[REGIndex] & 0xff00) |
                                                (RegistersValue[R_MIndex] & 0x00ff)); 
                }
                else if((REGIndex >= 4) && (R_MIndex >= 4))
                {
                    RegistersValue[REGIndex - 4] = ((RegistersValue[REGIndex - 4] & 0x00ff) |
                                                    (RegistersValue[R_MIndex - 4] & 0xff00)); 
                }
                else if((REGIndex < 4) && (R_MIndex >= 4))
                {
                    RegistersValue[REGIndex] = ((RegistersValue[REGIndex] & 0xff00) |
                                                ((RegistersValue[R_MIndex - 4] & 0xff00) >> 8)); 
                }
                else if((REGIndex >= 4) && (R_MIndex < 4))
                {
                    RegistersValue[REGIndex - 4] = ((RegistersValue[REGIndex - 4] & 0x00ff) |
                                                    ((RegistersValue[R_MIndex] & 0x00ff) << 8)); 
                }
            }
            else if(R_MIndex < 7)
            {
                if(R_MIndex < 4)
                {
                    RegistersValue[REGIndex - 8] = ((RegistersValue[REGIndex - 8] & 0xff00) |
                                                   (RegistersValue[R_MIndex] & 0x00ff));
                }
                else
                {
                    RegistersValue[REGIndex - 8] = ((RegistersValue[REGIndex - 8] & 0x00ff) |
                                                    (RegistersValue[R_MIndex - 4] & 0xff00));
                }
            }
            else
            {
                RegistersValue[REGIndex - 8] = RegistersValue[R_MIndex - 8];
            }
            printf("mov %s, %s\n", Registers[REGIndex], Registers[R_MIndex]);
        }
        else
        {
            if((R_MIndex < 7) && (REGIndex < 7))
            {
                if((R_MIndex < 4) && (REGIndex < 4))
                {
                    RegistersValue[R_MIndex] = ((RegistersValue[R_MIndex] & 0xff00) |
                                                (RegistersValue[REGIndex] & 0x00ff)); 
                }
                else if((R_MIndex >= 4) && (REGIndex >= 4))
                {
                    RegistersValue[R_MIndex - 4] = ((RegistersValue[R_MIndex - 4] & 0x00ff) |
                                                    (RegistersValue[REGIndex - 4] & 0xff00)); 
                }
                else if((R_MIndex < 4) && (REGIndex >= 4))
                {
                    RegistersValue[R_MIndex] = ((RegistersValue[R_MIndex] & 0xff00) |
                                                ((RegistersValue[REGIndex - 4] & 0xff00) >> 8)); 
                }
                else if((R_MIndex >= 4) && (REGIndex < 4))
                {
                    RegistersValue[R_MIndex - 4] = ((RegistersValue[R_MIndex - 4] & 0x00ff) |
                                                    ((RegistersValue[REGIndex] & 0x00ff) << 8)); 
                }
            }
            else if(REGIndex < 7)
            {
                if(REGIndex < 4)
                {
                    RegistersValue[R_MIndex - 8] = ((RegistersValue[R_MIndex - 8] & 0xff00) |
                                                   (RegistersValue[REGIndex] & 0x00ff));
                }
                else
                {
                    RegistersValue[R_MIndex - 8] = ((RegistersValue[R_MIndex - 8] & 0x00ff) |
                                                    (RegistersValue[REGIndex - 4] & 0xff00));
                }
            }
            else
            {
                RegistersValue[R_MIndex - 8] = RegistersValue[REGIndex - 8];
            }

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
                printf("mov %s, %s %+d]\n", (char *)Registers[REGIndex], (char *)Register_Memory[R_M],
                       (signed char)OneByteDis);
            }
            else
            {
                printf("mov %s, %s]\n", (char *)Registers[REGIndex], (char *)Register_Memory[R_M]);
            }
        }
        else
        {
            if(OneByteDis)
            {
                printf("mov %s %+d], %s\n", (char *)Register_Memory[R_M], (signed char)OneByteDis,
                       (char *)Registers[REGIndex]);
            }
            else
            {
                printf("mov %s], %s\n", (char *)Register_Memory[R_M], (char *)Registers[REGIndex]);
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
            printf("mov %s, %s %+d]\n", (char *)Registers[REGIndex], (char *)Register_Memory[R_M],
                   (signed short)TwoBytesDis);
        }
        else
        {
            printf("mov %s %+d], %s\n", (char *)Register_Memory[R_M], (signed short)TwoBytesDis,
                   (char *)Registers[REGIndex]);
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
                printf("mov %s, %s]\n", (char *)Registers[REGIndex], (char *)Register_Memory[R_M]);
            }
            else
            {
                printf("mov %s], %s\n", (char *)Register_Memory[R_M], (char *)Registers[REGIndex]);
            }

        }
    }

    return(RegistersValue);
}

void
RegisterMemoryToSegmentRegister(uint16 Instruction, FILE* fp, char *Register_Memory[], char *SegmentRegisters[],
                                char *Registers[], uint16 *RegistersValue, uint16 *SegmentRegistersValue)
{
    uint16 R_M = Instruction & 0x0007;

    uint16 ModMask = 0x00c0;
    uint16 MOD = (Instruction & ModMask) >> 6;

    uint16 SegRegIndex = (Instruction & 0x0018) >> 3;
    
    unsigned char buffer[2];
    unsigned char buf[1];

    if(MOD == 0)
    {
        if(R_M == 6)
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);
      
            }

            uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 
            printf("mov %s, [%d]\n", SegmentRegisters[SegRegIndex], (signed short)TwoBytesDis);
        }
        else
        {
            printf("mov %s, %s]\n", (char *)SegmentRegisters[SegRegIndex], (char *)Register_Memory[R_M]);
        }
    }

    else if(MOD == 1)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint8 OneByteDis = (uint8)buf[0]; 
        if(OneByteDis)
        {
            printf("mov %s, %s %+d]\n", (char *)SegmentRegisters[SegRegIndex], (char *)Register_Memory[R_M],
                   (signed char)OneByteDis);
        }
        else
        {
            printf("mov %s, %s]\n", (char *)SegmentRegisters[SegRegIndex], (char *)Register_Memory[R_M]);
        }
    }

    else if(MOD == 2)
    {
        if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 
        printf("mov %s, %s %+d]\n", (char *)SegmentRegisters[SegRegIndex], (char *)Register_Memory[R_M],
               (signed short)TwoBytesDis);
    }
    
    else if(MOD == 3)
    {

        uint16 RegIndex = R_M | 8;
        SegmentRegistersValue[SegRegIndex] = RegistersValue[RegIndex - 8];
        printf("mov %s, %s\n", SegmentRegisters[SegRegIndex], Registers[RegIndex]);
    }
}

void
SegmentRegisterToRegisterMemory(uint16 Instruction, FILE* fp, char *Register_Memory[], char *SegmentRegisters[],
                                char *Registers[], uint16 *RegistersValue, uint16 *SegmentRegistersValue)
{
    uint16 R_M = Instruction & 0x0007;

    uint16 ModMask = 0x00c0;
    uint16 MOD = (Instruction & ModMask) >> 6;

    uint16 SegRegIndex = (Instruction & 0x0018) >> 3;
    
    unsigned char buffer[2];
    unsigned char buf[1];

    if(MOD == 0)
    {
        if(R_M == 6)
        {
            if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
                printf("Failed to read data from file\n");
                fclose(fp);
      
            }

            uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 
            printf("mov [%d], %s\n", (signed short)TwoBytesDis, SegmentRegisters[SegRegIndex]);
        }
        else
        {
            printf("mov %s], %s\n", (char *)Register_Memory[R_M], (char *)SegmentRegisters[SegRegIndex]);
        }
    }

    else if(MOD == 1)
    {
        if (fread(buf, sizeof(buf), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint8 OneByteDis = (uint8)buf[0]; 
        if(OneByteDis)
        {
            printf("mov %s %+d], %s\n", (char *)Register_Memory[R_M], (signed char)OneByteDis,
                   (char *)SegmentRegisters[SegRegIndex]);
        }
        else
        {
            printf("mov %s], %s\n", (char *)Register_Memory[R_M], (char *)SegmentRegisters[SegRegIndex]);
        }
    }

    else if(MOD == 2)
    {
        if(fread(buffer, sizeof(buffer), 1, fp) != 1) {
            printf("Failed to read data from file\n");
            fclose(fp);
        }

        uint16 TwoBytesDis = (((uint16)buffer[1] << 8)  | buffer[0]); 
        printf("mov %s %+d], %s\n", (char *)Register_Memory[R_M], (signed short)TwoBytesDis,
               (char *)SegmentRegisters[SegRegIndex]);
    }
    
    else if(MOD == 3)
    {

        uint16 RegIndex = R_M | 8;
        RegistersValue[RegIndex - 8] = SegmentRegistersValue[SegRegIndex];
        printf("mov %s, %s\n", Registers[RegIndex], SegmentRegisters[SegRegIndex]);
    }
}
