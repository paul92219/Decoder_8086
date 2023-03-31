/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

void
AddSubCmpImmediateToRegisterMemory(uint16 Instruction, uint16 Operation, FILE* fp,
                                   char *Registers[], char *Register_Memory[])
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
ImmediateToRegisterMemory(uint16 Instruction, FILE* fp, char *Registers[],
                          char *Register_Memory[])
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

uint16 *
ImmediateToRegister(uint16 Instruction, FILE* fp, char *Registers[], uint16 *RegistersValue)
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
            Data = Instruction & 0x00ff;
        }
        REG = REG | 8;
        RegistersValue[REG] = Data;
        RegistersValue[REG - 8] = Data & 0x00ff;
        RegistersValue[REG - 4] = Data >> 8;
        
        printf("mov %s, %u\n", Registers[REG], Data);
    }
    else
    {
        uint16 Data = Instruction & 0x00ff;
        RegistersValue[REG] = Data;
        if(REG < 4)
        {
            RegistersValue[REG + 8] = (RegistersValue[REG + 8] & 0xff00) | Data; 
        }
        else
        {
            RegistersValue[REG + 4] = (RegistersValue[REG + 4] & 0x00ff) | (Data << 8); 
        }
        printf("mov %s, %u\n", Registers[REG], Data);
    }

    return(RegistersValue);
}
