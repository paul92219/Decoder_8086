/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

void
JumpAndLoop(uint16 Instruction, uint16 Operation, FILE *fp, char *LoopInstructions[],
            char *JumpInstructions[])
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
