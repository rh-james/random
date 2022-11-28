#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * Build:
 * ```
 * make setargv0
 * ```
 * Run:
 * ```
 * ./setargv0 [args]
 * ```
 */

void
array_printf(int count, char **arr)
{
	for (int i = 0; i < count; i++) {
		int written = 0;
		printf("%s ", arr[i], &written);
	}
	printf("\n");
}


int main(int argc, char **argv)
{
	strcpy(argv[0], "your mom");
	array_printf(argc, argv);

	//pid_t pid = getpid();
	//printf("run `ps aux|grep %d`\n", pid);
	//sleep(60);
	return 0;
}
