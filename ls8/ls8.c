#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{

  struct cpu cpu;

  if (argc != 2)
  {
    fprintf(stderr, "Error: ls8 file not found\n");
  }

  cpu_init(&cpu);
  //return 0;
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}