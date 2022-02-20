#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>

// Source: http://c-faq.com/misc/bitsets.html
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) (((nb) + CHAR_BIT - 1) / CHAR_BIT)

#define USER_INPUT_BUFFER_SIZE 100
#define SMALLEST_PRIME 2L
#define MIN_UPPER_LIMIT SMALLEST_PRIME

static char gUserInputBuffer[USER_INPUT_BUFFER_SIZE];
static long gUpperLimit;
static char *gIntegersTable;

void print_header(void)
{
	printf("Welcome to the 'Sieve of Eratosthenes'\n");
}

bool clear_stdin(const char *buffer, size_t bufferLength)
{
	for (size_t i = 0; i < (bufferLength - 1); ++i)
		if ((buffer[i] == '\n') && (buffer[i + 1] == '\0'))
			return false;
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	return true;
}

void get_user_input(void)
{
	while (true)
	{
		printf("Please enter upper limit(%li <= integer < %li):\n", MIN_UPPER_LIMIT, LONG_MAX);
		if (!fgets(gUserInputBuffer, USER_INPUT_BUFFER_SIZE, stdin))
		{
			printf("Error occurred when trying to get input from user. Exiting program\n");
			exit(EXIT_FAILURE);
		}
		if (clear_stdin(gUserInputBuffer, USER_INPUT_BUFFER_SIZE))
		{
			printf("User input exceeded input buffer size\n");
			continue;
		}
		char *endPointer;
		gUpperLimit = strtol(gUserInputBuffer, &endPointer, 0);
		if ((gUpperLimit == 0) && (endPointer == gUserInputBuffer))
		{
			printf("Invalid input\n");
			continue;
		}
		if ((errno == ERANGE) && (gUpperLimit == LONG_MAX))
		{
			printf("Input number is too large\n");
			continue;
		}
		if (gUpperLimit < MIN_UPPER_LIMIT)
		{
			printf("Input cant be smaller than %li\n", MIN_UPPER_LIMIT);
			continue;
		}
		break;
	}
	printf("You chose %li\n", gUpperLimit);
}

void print_header_and_get_user_input(void)
{
	print_header();
	get_user_input();
}

void allocate_integers_table(void)
{
	gIntegersTable = calloc(BITNSLOTS(gUpperLimit), sizeof(char));
	if (!gIntegersTable)
	{
		printf("Couldn't allocate memory. Exiting program");
		exit(EXIT_FAILURE);
	}
}

void iterate_table(void)
{
	long current_prime = SMALLEST_PRIME;
	long primes_count = 0;
	printf("The primes up to %li are:\n", gUpperLimit);
	while (true)
	{
		++primes_count;
		printf("%li, ", current_prime);
		long current_num = current_prime * 2;
		while (current_num <= gUpperLimit)
		{
			BITSET(gIntegersTable, current_num);
			current_num += current_prime;
		}
		long i = current_prime + 1;
		for (; i <= gUpperLimit; ++i)
		{
			if (!BITTEST(gIntegersTable, i))
			{
				current_prime = i;
				break;
			}
		}
		if (i >= gUpperLimit)
		{
			break;
		}
	}
	printf("\nTotal number of primes found: %li\n", primes_count);
}

void calculate_primes(void)
{
	allocate_integers_table();
	iterate_table();
}

int main(void)
{
	print_header_and_get_user_input();
    calculate_primes();
	return 0;
}
