/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

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
