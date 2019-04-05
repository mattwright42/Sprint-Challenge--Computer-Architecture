#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

unsigned int cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char val)
{
  cpu->ram[index] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char line[1048];

  int address = base_address;

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("Error opening file %s\n", filename);
    exit(2);
  }
  while (fgets(line, 1048, fp) != NULL)
  {
    char *endptr;
    unsigned char val = (strtoul(line, &endptr, 2));

    if (line == endptr)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, val);
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;

  case ALU_CMP:
    if (cpu->registers[regA] == cpu->registers[regB])
    {
      cpu->FL = 0b00000001;
      break;
    }
    else if (cpu->registers[regA] > cpu->registers[regB])
    {
      cpu->FL = 0b00000010;
      break;
    }
    else
    {
      cpu->FL = 0b00000100;
      break;
    }
    break;
  default:
    fprintf(stderr, "Operation unrecognized\n");
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA;
  unsigned char operandB;
  while (running)
  {
    // 1. Get the value of the current instruction (in address PC).
    unsigned int instruction = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);
    unsigned int total_operands = instruction >> 6;
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (total_operands > 0)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
    }
    if (total_operands > 1)
    {
      operandB = cpu_ram_read(cpu, cpu->PC + 2);
    }
    // 4. switch() over it to decide on a course of action.
    switch (instruction)
    {
      // 5. Do whatever the instruction should do according to the spec.
    case LDI:
      cpu->registers[operandA] = operandB;
      break;

    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;

    case HLT:
      running = 0;
      break;

    case POP:
      cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
      cpu->registers[7]++;
      break;

    case PUSH:
      cpu->registers[7]--;
      cpu->registers[operandA];
      cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
      break;

    case ST:
      cpu_ram_write(cpu, cpu->registers[operandA], cpu->registers[operandB]);
      break;

    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      break;

    case JMP:
      cpu->PC = cpu->registers[operandA];
      continue;
    case JEQ:

      if (cpu->FL == 0b00000001)
      {
        cpu->PC = cpu->registers[operandA];
        continue;
      }
      break;
    case JNE:

      if ((cpu->FL & 0b00000001) == 0)
      {

        cpu->PC = cpu->registers[operandA];
        continue;
      }
      break;
    default:
      break;
      alu(cpu, instruction, operandA, operandB);
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += total_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;

  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->registers, 0, 8 * sizeof(unsigned char));
}
