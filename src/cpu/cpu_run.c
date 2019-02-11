/*!
 * \brief cpu_code.c
 * \author Exdev
 * \version 0.5
 */
/*
#include "cpu_t.h"
#include "time.h"
#include <assert.h>

const size_t MEMORY_SIZE = 4096;

int main()
{
	char *text = calloc(MEMORY_SIZE, sizeof(char));
	uint8_t *code = calloc(MEMORY_SIZE, sizeof(uint8_t));

	FILE *fin = NULL;

	fin = fopen("D:/Programs/eclipse/eclipse/git/proglang/src/cpu/factorial_cpu.txt", "r");

	if (fin == NULL)
		assert (0);
	fread(text, 1024, 1, fin);

	code_asm(text, &code);

	struct cpu_t cpu = {};
	cpu_init(&cpu, &code);


	fprintf(stderr, "----Running----\n");
	clock_t exec_time = clock();
	long tmp = cpu_run(&cpu);
	exec_time = clock() - exec_time;
	printf("done:\n ncmds: %ld\n time: %lg\n freq: %lg MHz",
		   tmp, (double) exec_time / CLOCKS_PER_SEC, ((double) tmp )
		   / exec_time * CLOCKS_PER_SEC / 1000000);

	return 0;
}
*/
