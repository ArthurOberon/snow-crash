#include "stdio.h"

int main(int argc, char **argv)
{
	if (argc != 2 || argv[1] == NULL)
		return 1;

	int i = -1;
	while (argv[1][++i])
	{
		printf("%c", argv[1][i] - i);
	}
	return 0;
	printf("\n");
}

