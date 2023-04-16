/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */

struct print_content
{
    char Source[32];
    char Dest[32];
    char *Operation;
};

void
PrintInstruction(print_content PC)
{
    printf("%s %s, %s\n", PC.Operation, PC.Dest, PC.Source);
}

void
JumpAndLoop(instruction_content Content, registers *Registers, values *Values)
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
ComputeInstruction(uint16 OpCode, uint8 DestIndex, uint8 SourceIndex, values *Values, uint16 Data,
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
                    Values->RegistersValue[DestIndex] =
                        ((Values->RegistersValue[DestIndex] & 0xff00) |
                         (Values->RegistersValue[SourceIndex] & 0x00ff)); 
                }
                else if((DestIndex >= 4) && (SourceIndex >= 4))
                {
                    DestIndex -= 4;
                    SourceIndex -= 4;
                    Values->RegistersValue[DestIndex] =
                        ((Values->RegistersValue[DestIndex] & 0x00ff) |
                         (Values->RegistersValue[SourceIndex] & 0xff00)); 
                }
                else if((DestIndex < 4) && (SourceIndex >= 4))
                {
                    SourceIndex -= 4;
                    Values->RegistersValue[DestIndex] =
                        ((Values->RegistersValue[DestIndex] & 0xff00) |
                         ((Values->RegistersValue[SourceIndex] & 0xff00) >> 8)); 
                }
                else if((DestIndex >= 4) && (SourceIndex < 4))
                {
                    DestIndex -= 4;
                    Values->RegistersValue[DestIndex] =
                        ((Values->RegistersValue[DestIndex] & 0x00ff) |
                         ((Values->RegistersValue[SourceIndex] & 0x00ff) << 8)); 
                }
            }
            else
            {
                Values->RegistersValue[DestIndex] =
                    Values->RegistersValue[SourceIndex];
            }

        } break;

        case 0x00:
        {
            Values->RegistersValue[DestIndex] += Values->RegistersValue[SourceIndex];
            if(Values->RegistersValue[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersValue[DestIndex] & 0x8000)
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
            uint16 Result = Values->RegistersValue[DestIndex] - Values->RegistersValue[SourceIndex];
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
            Values->RegistersValue[DestIndex] -= Values->RegistersValue[SourceIndex];
            if(Values->RegistersValue[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersValue[DestIndex] & 0x8000)
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
            Values->RegistersValue[DestIndex] += Data;
            if(Values->RegistersValue[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersValue[DestIndex] & 0x8000)
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
            uint16 Result = Values->RegistersValue[DestIndex] - Data;
            if(Values->RegistersValue[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersValue[DestIndex] & 0x8000)
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
            Values->RegistersValue[DestIndex] -= Data;
            if(Values->RegistersValue[DestIndex] == 0)
            {
                Values->Flags |= 0x0040;
            }
            else
            {
                Values->Flags &= 0x0080;
            }
                
            if(Values->RegistersValue[DestIndex] & 0x8000)
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
                Values->RegistersValue[0] += Data;
            }
            else
            {
                uint8 LowPart = (Values->RegistersValue[0] & 0x00ff) + Data;
                Values->RegistersValue[0] = (Values->RegistersValue[0] & 0xff00) | LowPart; 
            }
            
        } break;
    };
}

void
DetermineOperation(instruction_content Content, registers *Registers, values *RegistersValues)
{
    print_content PC = {};
    
    switch(Content.OpCode)
    {
        case 0x8c:
        case 0x8e:
        {
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
                    RegistersValues->SegmentRegistersValue[Content.SR] =
                        RegistersValues->RegistersValue[Content.R_M];

                    sprintf(PC.Dest, "%s", Registers->SegmentRegisters[Content.SR]);
                    sprintf(PC.Source, "%s", Registers->MainRegisters[Content.R_M]);
                }
                else
                {
                    RegistersValues->RegistersValue[Content.R_M] =
                        RegistersValues->SegmentRegistersValue[Content.SR];

                    sprintf(PC.Dest, "%s", Registers->MainRegisters[Content.R_M]);
                    sprintf(PC.Source, "%s", Registers->SegmentRegisters[Content.SR]);
                }
            }

            PrintInstruction(PC);
            
        } break;

        case 0x88:
        case 0x00:
        case 0x38:
        case 0x28:
        {
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

            if(Content.DirectAddress)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.DBit)
                {
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "[%d]", (signed short)TwoBytesDis);
                }
                else
                {
                    sprintf(PC.Dest, "[%d]", (signed short)TwoBytesDis);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 0)
            {
                if(Content.DBit)
                {
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s]", Registers->Register_Memory[Content.R_M]);
                }
                else
                {
                    sprintf(PC.Dest, "%s]", Registers->Register_Memory[Content.R_M]);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 1)
            {
                uint8 OneByteDis = Content.DispLow; 
                if(Content.DBit)
                {
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed char)OneByteDis);
                }
                else
                {
                    sprintf(PC.Dest, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed char)OneByteDis);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 2)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.DBit)
                {
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed short)TwoBytesDis);
                }
                else
                {
                    sprintf(PC.Dest, "%s %+d]", Registers->Register_Memory[Content.R_M],
                            (signed short)TwoBytesDis);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }
            else if(Content.MOD == 3)
            {
                if(Content.DBit)
                {
                    ComputeInstruction(Content.OpCode, Content.REG, Content.R_M, RegistersValues, 0,
                                       Content.WBit);
                    sprintf(PC.Dest, "%s", Content.Reg);
                    sprintf(PC.Source, "%s", Content.RegMem);
                }

                else
                {
                    ComputeInstruction(Content.OpCode, Content.R_M, Content.REG, RegistersValues, 0,
                                       Content.WBit);
                    sprintf(PC.Dest, "%s", Content.RegMem);
                    sprintf(PC.Source, "%s", Content.Reg);
                }
            }

            PrintInstruction(PC);
        } break;

        case 0xb0:
        {
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
                RegistersValues->RegistersValue[Content.REG] = Data;
        
                printf("mov %s, %u\n", Content.Reg, Data);
            }
            else
            {
                Data = Content.Data;
                if(Content.REG < 4)
                {
                    RegistersValues->RegistersValue[Content.REG] =
                        (RegistersValues->RegistersValue[Content.REG] & 0xff00) | Data; 
                }
                else
                {
                    RegistersValues->RegistersValue[Content.REG - 4] =
                        (RegistersValues->RegistersValue[Content.REG - 4] & 0x00ff) | (Data << 8); 
                }
                printf("mov %s, %u\n", Content.Reg, Data);

            }
        } break;

        case 0xa0:
        {
            uint16 TwoBytesDis = ((uint16)Content.DispHigh << 8) | (Content.DispLow); 
            printf("mov ax, [%d]\n", (signed short)TwoBytesDis);

        } break;

        case 0xa2:
        {
            uint16 TwoBytesDis = ((uint16)Content.DispHigh << 8) | (Content.DispLow); 
            printf("mov [%d], ax\n", (signed short)TwoBytesDis);

        } break;

        case 0x04:
        case 0x3c:
        case 0x2c:
        {
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

            ComputeInstruction(Content.OpCode, 0, 0, RegistersValues, Data, Content.WBit);
            PrintInstruction(PC);

        } break;

        case 0xc6:
        {
            if(Content.DirectAddress)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.WBit)
                {
                    uint16 Data = (((uint16)Content.DataW << 8)  | Content.Data); 
                    printf("mov [%d], word [%d]\n", (signed short)TwoBytesDis, (signed short)Data);
                }
                else
                {
                    uint8 Data = Content.Data;
                    printf("mov [%d], byte [%d]\n", (signed short)TwoBytesDis, (signed char)Data);
                }
            }

            else if(Content.MOD == 0)
            {
                if(Content.WBit)
                {
                    uint16 Data = (((uint16)Content.DataW << 8)  | Content.Data); 
                    printf("mov %s], word [%d]\n", Registers->Register_Memory[Content.R_M],
                           (signed short)Data);
                }
                else
                {
                    uint8 Data = Content.Data;
                    printf("mov %s], byte %d\n", Registers->Register_Memory[Content.R_M],
                           (signed char)Data);
                }
            }

            else if(Content.MOD == 1)
            {
                uint8 OneByteDis = Content.DispLow; 
                if(Content.WBit)
                {
                    uint16 Data = (((uint16)Content.DataW << 8)  | Content.Data); 
                    printf("mov %s %+d], word %d\n", Registers->Register_Memory[Content.R_M],
                           (signed char)OneByteDis, (signed short)Data);
                }
                else
                {
                    uint8 Data = Content.Data;
                    printf("mov %s %+d], byte %d\n", Registers->Register_Memory[Content.R_M],
                           (signed char)OneByteDis, (signed char)Data);
                }
            }

            else if(Content.MOD == 2)
            {
                uint16 TwoBytesDis = (((uint16)Content.DispHigh << 8)  | Content.DispLow); 
                if(Content.WBit)
                {
                    uint16 Data = (((uint16)Content.DataW << 8)  | Content.Data); 
                    printf("mov %s %+d], word %d\n", Registers->Register_Memory[Content.R_M],
                           (signed short)TwoBytesDis, (signed short)Data);
                }
                else
                {
                    uint8 Data = Content.Data;
                    printf("mov %s %+d], byte %d\n", Registers->Register_Memory[Content.R_M],
                           (signed short)TwoBytesDis, (signed char)Data);
                }
            }

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
                ComputeInstruction(OpCode, Content.R_M, 0, RegistersValues, Data, Content.WBit);
                sprintf(PC.Dest, "%s", Registers->MainRegisters[Content.R_M]);
            }

            PrintInstruction(PC);
            
        } break;

        default:
        {
            JumpAndLoop(Content, Registers, RegistersValues);

        } break;
    };
}
