#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

unsigned int cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
  //printf("This is read.\n");
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char val)
{
  //printf("This is write.\n");
  cpu->ram[index] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  //printf("This is load.\n");
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  FILE *fp;
  char line[1048];

  int address = base_address;

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("Error opening file %s\n", filename);
    exit(2);
    //return 2;
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

  // if (argc != 2)
  // {
  //   printf("usage: fileio filename\n");
  //   //return 1;
  // }

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;
  default:
    fprintf(stderr, "Operation unrecognized\n");
    //exit(1);
    break;

    // TODO: implement more ALU ops
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
    // TODO
    // 1. Get the value of the current instruction (in address PC).

    unsigned int instruction = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);
    unsigned int total_operands = instruction >> 6;
    //printf("%u, %c, %c\n", instruction, operandA, operandB);
    // 2. Figure out how many operands this next instruction requires

    if (total_operands > 0)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
    }
    if (total_operands > 1)
    {
      operandB = cpu_ram_read(cpu, cpu->PC + 2);
    }
    switch (instruction)
    {
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

    default:
      //break;
      alu(cpu, instruction, operandA, operandB);
    }
    cpu->PC += total_operands + 1;
    // {
    // case 0b10000010: //LDI
    //   printf("Operands: %d %d\n", operandA, operandB);
    //   cpu->registers[operandA] = operandB;
    //   break;
    // case 0b01000111: //PRN
    //   printf("%d\n", cpu->registers[operandA]);
    //   break;
    // case 0b00000001: //HLT
    //   running = 0;
    //   break;
    // default:
    //   printf("ERROR: invalid instruction.\n");
    //   exit(1);
    // };
    // printf("register 0: %d\n", cpu->registers[0]);
    // 3. Get the appropriate value(s) of the operands following this instruction

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->registers, 0, 8 * sizeof(unsigned char));
}
