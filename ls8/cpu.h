#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC
  unsigned int PC;
  // unsigned int PC;
  // registers (array)
  //unsigned char *registers;
  unsigned char registers[8];
  // ram (array)
  //unsigned char *ram;
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_MUL = 0b10100010
  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define POP 0b01000110
#define PUSH 0b01000101
#define ST 0b10000100
#define JMP 01010100
// TODO: more instructions here. These can be used in cpu_run().
#define base_address 0x00
// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif