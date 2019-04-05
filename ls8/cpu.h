#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  unsigned int PC;
  unsigned char FL;
  unsigned char E;
  unsigned char G;
  unsigned char L;
  // registers (array)
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_MUL = 0b10100010,
  ALU_CMP = 0b10100111
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
#define CMP 0b10100111
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110
#define base_address 0x00

// Function declarations
extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif