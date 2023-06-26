/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
#include "compute.h"


void
PrintInstruction(print_content PC, uint32 LastClocks)
{
    printf("%s %s, %s ; LastClocks: %u,  +Clocks: %u, TotalClocks: %u\n",
           PC.Operation, PC.Dest, PC.Source, LastClocks,
           (PC.Clocks - LastClocks), PC.Clocks);
}

void
ComputeEACicles(instruction_content *IC, uint32 *TotalCicles)
{
    int RegIndex = 0;
    if((IC->DBit && !IC->DirectAddress) || ((!IC->DBit && !IC->DirectAddress)))
    {
        RegIndex = IC->R_M;
    }
    else
    {
        RegIndex = -1;
    }
        
    if((IC->DispLow) && ((RegIndex == 3) || (RegIndex == 0)))
    {
        *TotalCicles += 11;
    }
    else if((IC->DispLow) && ((RegIndex == 2) || (RegIndex == 1)))
    {
        *TotalCicles += 12;
    }
    else if(((RegIndex == 4) || (RegIndex == 5) ||
             (RegIndex == 6) || (RegIndex == 7)) && (IC->DispLow)) 
    {
        *TotalCicles += 9;
    }
    else if(IC->DispLow)
    {
        *TotalCicles += 6;
    }
    else if((RegIndex == 3) || (RegIndex == 0))
    {
        *TotalCicles += 7;
    }
    else if((RegIndex == 2) || (RegIndex == 1))
    {
        *TotalCicles += 8;
    }
    else if((RegIndex == 4) || (RegIndex == 5) || (RegIndex == 6) || (RegIndex == 7))
    {
        *TotalCicles += 5;
    }
}

void
JumpAndLoop(instruction_content Content, registers *Registers, data *Values)
{
    if(Content.OpCode > 0x7f)
    {
        int Index = Content.OpCode - 224;
        uint8 IP = Content.Data;
        char *Ins = Registers->LoopInstructions[Index];
        printf("%s $+%u\n", Ins, IP + 2);
    }
    else
    {
        int Index = Content.OpCode - 112;
        if((Content.OpCode == 0x75) && !(Values->Flags & 0x0040))
        {
            Values->IPRegister += (signed char)Content.Data;
            char *Ins = Registers->JumpInstructions[Index];
            printf("%s $%+d\n", Ins, (signed char)Content.Data + 2);
        }
        else
        {
            uint8 IP = Content.Data;
            char *Ins = Registers->JumpInstructions[Index];
            printf("%s $%+d\n", Ins, (signed char)IP + 2);
        }
    }
}

void
ComputeInstruction(uint16 OpCode, uint8 DestIndex, uint8 SourceIndex, data *Values, uint16 Data,
                   uint8 WBit)
{
    switch(OpCode)
    {
        case 0x88:
        {
            if(!WBit)
            {
                if((DestIndex < 4) && (SourceIndex < 4))
                {
                    Values->RegistersBuffer[DestIndex] =
                        ((Values->RegistersBuffer[DestIndex] & 0xff00) |
                         (Values->RegistersBuffer[SourceIndex] & 0x00ff)); 
                }
                else if((DestIndex >= 4) && (SourceIndex >= 4))
                {
                    DestIndex -= 4;
                    SourceIndex -= 4;
                    Values->RegistersBuffer[DestIndex] =
                        ((Values->RegistersBuffer[DestIndex] & 0x00ff) |
                         (Values->RegistersBuffer[SourceIndex] & 0xff00)); 
                }
                else if((DestIndex < 4) && (SourceIndex >= 4))
                {
                    SourceIndex -= 4;
                    Values->RegistersBuffer[DestIndex] =
                        ((Values->RegistersBuffer[DestIndex] & 0xff00) |
                         ((Values->RegistersBuffer[SourceIndex] & 0xff00) >> 8)); 
                }
                else if((DestIndex >= 4) && (SourceIndex < 4))
                {
                    DestIndex -= 4;
                    Values->RegistersBuffer[DestIndex] =
                        ((Values->RegistersBuffer[DestIndex] & 0x00ff) |
                         ((Values->RegistersBuffer[SourceIndex] & 0x00ff) << 8)); 
                }
            }
            else
            {
                Values->RegistersBuffer[DestIndex] =
                    Values->RegistersBuffer[SourceIndex];
            }

        } break;

        case 0x00:
        {
            Values->RegistersBuffer[DestIndex] += Values->RegistersBuffer[SourceIndex];
            if(Values->RegistersBuffer[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersBuffer[DestIndex] & 0x8000)
            {
                Values->Flags |= 0x0080; 
            }
            else
            {
                Values->Flags &= 0x0040;
            }

        } break;
        
        case 0x38:
        {
            uint16 Result = Values->RegistersBuffer[DestIndex] - Values->RegistersBuffer[SourceIndex];
            if(Result == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Result & 0x8000)
            {
                Values->Flags |= 0x0080; 
            }
            else
            {
                Values->Flags &= 0x0040;
            }

        } break;
        
        case 0x28:
        {
            Values->RegistersBuffer[DestIndex] -= Values->RegistersBuffer[SourceIndex];
            if(Values->RegistersBuffer[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersBuffer[DestIndex] & 0x8000)
            {
                Values->Flags |= 0x0080; 
            }
            else
            {
                Values->Flags &= 0x0040;
            }
            
        } break;

        case 0x8000:
        {
            Values->RegistersBuffer[DestIndex] += Data;
            if(Values->RegistersBuffer[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersBuffer[DestIndex] & 0x8000)
            {
                Values->Flags |= 0x0080; 
            }
            else
            {
                Values->Flags &= 0x0040;
            }

        } break;

        case 0x8038:
        {
            uint16 Result = Values->RegistersBuffer[DestIndex] - Data;
            if(Result == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Result & 0x8000)
            {
                Values->Flags |= 0x0080; 
            }
            else
            {
                Values->Flags &= 0x0040;
            }

        } break;

        case 0x8028:
        {
            Values->RegistersBuffer[DestIndex] -= Data;
            if(Values->RegistersBuffer[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersBuffer[DestIndex] & 0x8000)
            {
                Values->Flags |= 0x0080; 
            }
            else
            {
                Values->Flags &= 0x0040;
            }

        } break;

        case 0x04:
        case 0x2c:
        {
            if(WBit)
            {
                Values->RegistersBuffer[0] += Data;
            }
            else
            {
                uint8 LowPart = (Values->RegistersBuffer[0] & 0x00ff) + Data;
                Values->RegistersBuffer[0] = (Values->RegistersBuffer[0] & 0xff00) | LowPart; 
            }
            
        } break;
    };
}

uint16
ComputeMemoryIndex(data *Values, uint8 R_MIndex, uint16 DispValue)
{
    uint16 Result = DispValue;
    switch(R_MIndex)
    {
        case 0:
        {
            Result += Values->RegistersBuffer[3] + Values->RegistersBuffer[6];
        } break;

        case 1:
        {
            Result += Values->RegistersBuffer[3] + Values->RegistersBuffer[7];
        } break;

        case 2:
        {
            Result += Values->RegistersBuffer[5] + Values->RegistersBuffer[6];
        } break;

        case 3:
        {
            Result += Values->RegistersBuffer[5] + Values->RegistersBuffer[7];
        } break;

        case 4:
        {
            Result += Values->RegistersBuffer[6];
        } break;

        case 5:
        {
            Result += Values->RegistersBuffer[7];
        } break;

        case 6:
        {
            Result += Values->RegistersBuffer[5];
        } break;

        case 7:
        {
            Result += Values->RegistersBuffer[3];
        } break;
    };

    return(Result);
}

void
ComputeImmediateMemoryOperations(load_memory LM, data *Values)
{
    uint16 MemIndex = ComputeMemoryIndex(Values, LM.R_MIndex, LM.DispValue);
    
    if(LM.WordByte)
    {
        Values->MemoryBuffer[MemIndex] = (uint8)(LM.Data & 0x00ff);
        Values->MemoryBuffer[MemIndex + 1] = (uint8)(LM.Data & 0xff00);
    }
    else
    {
        Values->MemoryBuffer[MemIndex] = (uint8)(LM.Data & 0x00ff);
    }
}

void
WriteIntoMemory(load_memory LM, data *Values, uint8 OpCode)
{
    uint16 MemIndex = ComputeMemoryIndex(Values, LM.R_MIndex, LM.DispValue);

    uint16 Value = Values->RegistersBuffer[LM.REGIndex];
    uint16 MemValue = (Values->MemoryBuffer[MemIndex + 1] << 8) | Values->MemoryBuffer[MemIndex];
    
    if(OpCode == 0x00)
    {
        MemValue += Value;
        Values->MemoryBuffer[MemIndex] = (uint8)(MemValue & 0x00ff);
        Values->MemoryBuffer[MemIndex + 1] = (uint8)(MemValue & 0xff00);
    }
    else if(OpCode == 0x28)
    {
        MemValue -= Value;
        Values->MemoryBuffer[MemIndex] = (uint8)(MemValue & 0x00ff);
        Values->MemoryBuffer[MemIndex + 1] = (uint8)(MemValue & 0xff00);
    }
    else if(OpCode == 0x88)
    {
        Values->MemoryBuffer[MemIndex] = (uint8)(Value & 0x00ff);
        Values->MemoryBuffer[MemIndex + 1] = (uint8)(Value & 0xff00);
    }
}

void
LoadFromMemory(load_memory LM, data *Values, uint8 OpCode)
{
    uint16 MemIndex = ComputeMemoryIndex(Values, LM.R_MIndex, LM.DispValue);

    
    uint16 Value = Values->RegistersBuffer[LM.REGIndex];
    uint16 MemValue = (Values->MemoryBuffer[MemIndex + 1] << 8) | Values->MemoryBuffer[MemIndex];
    if(OpCode == 0x00)
    {
        Value += MemValue;
        if(Value == 0)
        {
            Values->Flags |= 0x0040;
        }
        else
        {
            Values->Flags &= 0x0080;
        }
                
        if(Value & 0x8000)
        {
            Values->Flags |= 0x0080; 
        }
        else
        {
            Values->Flags &= 0x0040;
        }
    }
    else if(OpCode == 0x28)
    {
        Value -= MemValue;
        if(Value == 0)
        {
            Values->Flags |= 0x0040;
        }
        else
        {
            Values->Flags &= 0x0080;
        }
                
        if(Value & 0x8000)
        {
            Values->Flags |= 0x0080; 
        }
        else
        {
            Values->Flags &= 0x0040;
        }
    }
    else if(OpCode == 0x88)
    {
        Value = (Value & 0xff00) | Values->MemoryBuffer[MemIndex];

        Value = (Value & 0x00ff) | (Values->MemoryBuffer[MemIndex + 1] << 8);
    }

    Values->RegistersBuffer[LM.REGIndex] = Value;
}

void
DetermineOperation(instruction_content Content, registers *Registers, data *RegistersBuffer,
                   uint32 *TotalCicles)
{
    print_content PC = {};
    uint32 LastCicles = *TotalCicles;

    switch(Content.OpCode)
    {
        case 0x8c:
        case 0x8e:
        {
            if(Content.MOD != 3)
            {
                ComputeEACicles(&Content, TotalCicles);

                if(Content.OpCode == 0x8e)
                {
                    *TotalCicles += 8;
                }
                else
                {
                    *TotalCicles += 9;
                }
            }
            else
            {
                *TotalCicles += 2;
            }
            
            PC.Operation = "mov";
            if(Content.DirectAddress)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.OpCode == 0x8e)
                {
                    sprintf(PC.Dest, "%s", Registers->SegmentRegisters[Content.SR]);
                    sprintf(PC.Source, "[%d]", (signed short)TwoBytesDis);
                }
                else
                {
                    sprintf(PC.Dest, "[%d]", (signed short)TwoBytesDis);
                    sprintf(PC.Source, "%s", Registers->SegmentRegisters[Content.SR]);
                }
            }
            else if(Content.MOD == 0)
            {
                if(Content.OpCode == 0x8e)
                {
                    sprintf(PC.Dest, "%s", Registers->SegmentRegisters[Content.SR]);
                    sprintf(PC.Source, "%s]", Registers->Register_Memory[Content.R_M]);
                }
                else
                {
                    sprintf(PC.Dest, "%s]", Registers->Register_Memory[Content.R_M]);
                    sprintf(PC.Source, "%s", Registers->SegmentRegisters[Content.SR]);
                }
            }
            else if(Content.MOD == 1)
            {
                uint8 OneByteDis = Content.DispLow; 
                if(Content.OpCode == 0x8e)
                {
                    sprintf(PC.Dest, "%s", Registers->SegmentRegisters[Content.SR]);
                    sprintf(PC.Source, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed char)OneByteDis);
                }
                else
                {
                    sprintf(PC.Dest, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed char)OneByteDis);
                    sprintf(PC.Source, "%s", Registers->SegmentRegisters[Content.SR]);
                }
            }
            else if(Content.MOD == 2)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.OpCode == 0x8e)
                {
                    sprintf(PC.Dest, "%s", Registers->SegmentRegisters[Content.SR]);
                    sprintf(PC.Source, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed short)TwoBytesDis);
                }
                else
                {
                    sprintf(PC.Dest, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed short)TwoBytesDis);
                    sprintf(PC.Source, "%s", Registers->SegmentRegisters[Content.SR]);
                }
            }
            else if(Content.MOD == 3)
            {
                if(Content.OpCode == 0x8e)
                {
                    RegistersBuffer->SegmentRegistersBuffer[Content.SR] =
                        RegistersBuffer->RegistersBuffer[Content.R_M];

                    sprintf(PC.Dest, "%s", Registers->SegmentRegisters[Content.SR]);
                    sprintf(PC.Source, "%s", Registers->MainRegisters[Content.R_M]);
                }
                else
                {
                    RegistersBuffer->RegistersBuffer[Content.R_M] =
                        RegistersBuffer->SegmentRegistersBuffer[Content.SR];

                    sprintf(PC.Dest, "%s", Registers->MainRegisters[Content.R_M]);
                    sprintf(PC.Source, "%s", Registers->SegmentRegisters[Content.SR]);
                }
            }

            PC.Clocks = *TotalCicles;
            PrintInstruction(PC, LastCicles);
            
        } break;

        case 0x88:
        case 0x00:
        case 0x38:
        case 0x28:
        {
            load_memory LMemory = {};
            LMemory.REGIndex = Content.REG;
            LMemory.R_MIndex = Content.R_M;

            if(Content.OpCode == 0x00)
            {
                PC.Operation = "add";
            }
            else if(Content.OpCode == 0x28)
            {
                PC.Operation = "sub";
            }
            else if(Content.OpCode == 0x38)
            {
                PC.Operation = "cmp";
            }
            else
            {
                PC.Operation = "mov";
            }


            if(Content.MOD != 3)
            {
                ComputeEACicles(&Content, TotalCicles);

                if(Content.DBit)
                {
                    if(Content.OpCode == 0x88)
                    {
                        *TotalCicles += 8;
                    }
                    else
                    {
                        *TotalCicles += 9;
                    }
                }
                else
                {
                    if((Content.OpCode == 0x88) || (Content.OpCode == 0x38))
                    {
                        *TotalCicles += 9;
                    }
                    else
                    {
                        *TotalCicles += 16;
                    }
                }
            }
            else
            {
                if(Content.OpCode == 0x88)
                {
                    *TotalCicles += 2;
                }
                else
                {
                    *TotalCicles += 3;
                }
            }
            
            if(Content.DirectAddress)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.DBit)
                {
                    RegistersBuffer->RegistersBuffer[Content.REG] = RegistersBuffer->MemoryBuffer[TwoBytesDis];
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "[%d]", (signed short)TwoBytesDis);
                }
                else
                {
                    RegistersBuffer->MemoryBuffer[TwoBytesDis] = RegistersBuffer->RegistersBuffer[Content.REG];
                    sprintf(PC.Dest, "[%d]", (signed short)TwoBytesDis);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 0)
            {
                if(Content.DBit)
                {
                    LoadFromMemory(LMemory, RegistersBuffer, Content.OpCode);

                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s]", Registers->Register_Memory[Content.R_M]);
                }
                else
                {
                    WriteIntoMemory(LMemory, RegistersBuffer, Content.OpCode);

                    sprintf(PC.Dest, "word %s]", Registers->Register_Memory[Content.R_M]);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 1)
            {
                LMemory.DispValue = Content.DispLow; 
                if(Content.DBit)
                {
                    LoadFromMemory(LMemory, RegistersBuffer, Content.OpCode);

                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed char)LMemory.DispValue);
                }
                else
                {
                    WriteIntoMemory(LMemory, RegistersBuffer, Content.OpCode);

                    sprintf(PC.Dest, "word %s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed char)LMemory.DispValue);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 2)
            {
                LMemory.DispValue = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.DBit)
                {
                    LoadFromMemory(LMemory, RegistersBuffer, Content.OpCode);

                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed short)LMemory.DispValue);
                }
                else
                {
                    WriteIntoMemory(LMemory, RegistersBuffer, Content.OpCode);

                    sprintf(PC.Dest, "word %s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed short)LMemory.DispValue);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 3)
            {
                if(Content.DBit)
                {
                    ComputeInstruction(Content.OpCode, Content.REG, Content.R_M, RegistersBuffer, 0,
                                       Content.WBit);
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s", Content.RegMem);
                }

                else
                {
                    ComputeInstruction(Content.OpCode, Content.R_M, Content.REG, RegistersBuffer, 0,
                                       Content.WBit);
                    sprintf(PC.Dest, "%s", Content.RegMem);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }

            PC.Clocks = *TotalCicles;
            PrintInstruction(PC, LastCicles);
        } break;

        case 0xb0:
        {
            *TotalCicles += 4;
            uint16 Data;
            if(Content.WBit)
            {
                if(Content.DataW != 0)
                {
                    Data = (Content.DataW << 8) | Content.Data;
                }
                else
                {
                    Data = Content.Data;
                }
                RegistersBuffer->RegistersBuffer[Content.REG] = Data;
        
                printf("mov %s, %u ; LastClocks: %u,  +Clocks: %u, TotalClocks: %u\n",
                       Content.Reg, Data, LastCicles, (*TotalCicles - LastCicles), *TotalCicles);
            }
            else
            {
                Data = Content.Data;
                if(Content.REG < 4)
                {
                    RegistersBuffer->RegistersBuffer[Content.REG] =
                        (RegistersBuffer->RegistersBuffer[Content.REG] & 0xff00) | Data; 
                }
                else
                {
                    RegistersBuffer->RegistersBuffer[Content.REG - 4] =
                        (RegistersBuffer->RegistersBuffer[Content.REG - 4] & 0x00ff) | (Data << 8); 
                }
                printf("mov %s, %u ; LastClocks: %u,  +Clocks: %u, TotalClocks: %u\n",
                       Content.Reg, Data, LastCicles, (*TotalCicles - LastCicles), *TotalCicles);

            }
        } break;

        case 0xa0:
        {
            *TotalCicles += 10;
            uint16 TwoBytesDis = ((uint16)Content.DispHigh << 8) | (Content.DispLow); 
            printf("mov ax, [%d] ; LastClocks: %u,  +Clocks: %u, TotalClocks: %u\n",
                   (signed short)TwoBytesDis, LastCicles, (*TotalCicles - LastCicles), *TotalCicles);

        } break;

        case 0xa2:
        {
            *TotalCicles += 10;
            uint16 TwoBytesDis = ((uint16)Content.DispHigh << 8) | (Content.DispLow); 
            printf("mov [%d], ax ; LastClocks: %u,  +Clocks: %u, TotalClocks: %u\n",
                   (signed short)TwoBytesDis, LastCicles, (*TotalCicles - LastCicles), *TotalCicles);

        } break;

        case 0x04:
        case 0x3c:
        case 0x2c:
        {
            *TotalCicles += 4;
            if(Content.OpCode == 0x04)
            {
                PC.Operation = "add";
            }
            else if(Content.OpCode == 0x3c)
            {
                PC.Operation = "cmp";
            }
            else
            {
                PC.Operation = "sub";
            }

            uint16 Data;
            if(Content.WBit)
            {
                Data = ((uint16)Content.DataW << 8) | (Content.Data); 
                sprintf(PC.Dest, "ax");
                sprintf(PC.Source, "%d", Data);
            }
            else
            {
                Data = Content.Data;
                sprintf(PC.Dest, "al");
                sprintf(PC.Source, "%d", Data);
            }

            ComputeInstruction(Content.OpCode, 0, 0, RegistersBuffer, Data, Content.WBit);
            PrintInstruction(PC, LastCicles);

        } break;

        case 0xc6:
        {
            load_memory LMemory = {};
            LMemory.R_MIndex = Content.R_M;

            ComputeEACicles(&Content, TotalCicles);
            *TotalCicles += 10;
            
            char *WordByte;
            if(Content.WBit)
            {
                LMemory.Data = (((uint16)Content.DataW << 8)  | Content.Data); 
                LMemory.WordByte = 1;
                WordByte = "word";
            }
            else
            {
                LMemory.Data = Content.Data;
                LMemory.WordByte = 0;
                WordByte = "byte";
            }
            PC.Operation = "mov";
            sprintf(PC.Source, "%d", (signed short)LMemory.Data);
            
            if(Content.DirectAddress)
            {
                LMemory.DispValue = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                sprintf(PC.Dest, "%s [%d]", WordByte, (signed short)LMemory.DispValue);
            }

            else if(Content.MOD == 0)
            {
                sprintf(PC.Dest, "%s %s]", WordByte, Registers->Register_Memory[Content.R_M]);
            }

            else if(Content.MOD == 1)
            {
                LMemory.DispValue = Content.DispLow; 
                sprintf(PC.Dest, "%s %s %+d]", WordByte, Registers->Register_Memory[Content.R_M],
                        (signed char)LMemory.DispValue);
            }

            else if(Content.MOD == 2)
            {
                LMemory.DispValue = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                sprintf(PC.Dest, "%s %s %+d]", WordByte, Registers->Register_Memory[Content.R_M],
                        (signed short)LMemory.DispValue);
            }

            ComputeImmediateMemoryOperations(LMemory, RegistersBuffer);
            PC.Clocks = *TotalCicles;
            PrintInstruction(PC, LastCicles);

        } break;

        case 0x80:
        {
            uint16 OpCode;
            if(Content.REG == 0)
            {
                PC.Operation = "add";
            }
            else if(Content.REG == 5)
            {
                PC.Operation = "sub";
            }
            else if(Content.REG == 7)
            {
                PC.Operation = "cmp";
            }

            if(Content.MOD != 3)
            {
                ComputeEACicles(&Content, TotalCicles);

                if(Content.REG == 0)
                {
                    *TotalCicles += 17;
                }
                else if(Content.REG == 5)
                {
                    *TotalCicles += 17;
                }
                else if(Content.REG == 7)
                {
                    *TotalCicles += 10;
                }
            }
            else
            {
                *TotalCicles += 4;
            }
            
            OpCode = (Content.OpCode << 8) | (Content.REG << 3);

            uint16 Data;
            if(Content.WBit && !Content.SBit)
            {
                Data = (((uint16)Content.DataWS << 8)  | Content.Data);
            }
            else
            {
                Data = Content.Data;
            }
            sprintf(PC.Source, "%d", Data);
            
            if(Content.DirectAddress)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                sprintf(PC.Dest, "word [%d]", TwoBytesDis);
            }
            else if(Content.MOD == 0)
            {
                sprintf(PC.Dest, "byte %s]", Registers->Register_Memory[Content.R_M]);
            }
            else if(Content.MOD == 1)
            {
                uint8 OneByteDis = Content.DispLow; 
                sprintf(PC.Dest, "byte %s %+d]", Registers->Register_Memory[Content.R_M],
                        (signed char)OneByteDis);
            }
            else if(Content.MOD == 2)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                sprintf(PC.Dest, "word %s %+d]", Registers->Register_Memory[Content.R_M],
                        (signed short)TwoBytesDis);
            }
            else if(Content.MOD == 3)
            {
                ComputeInstruction(OpCode, Content.R_M, 0, RegistersBuffer, Data, Content.WBit);
                sprintf(PC.Dest, "%s", Registers->MainRegisters[Content.R_M]);
            }
            
            PC.Clocks = *TotalCicles;
            PrintInstruction(PC, LastCicles);
            
        } break;

        default:
        {
            JumpAndLoop(Content, Registers, RegistersBuffer);

        } break;
    };
}
