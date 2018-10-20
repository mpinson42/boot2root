#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void	explode_bomb(void)
{
	puts("\nBOOM!!!\nThe bomb has blown up.");
	exit(0x8);
}

int		read_six_numbers(const char *line, int *tab)
{
	int			ret;

	ret = sscanf(line, "%d %d %d %d %d %d",
			&tab[0], &tab[1], &tab[2], &tab[3], &tab[4], &tab[5]);
	if (ret > 5)
		return (1);
	explode_bomb();
	return (ret);
}

void	read_line(char *buf, size_t size)
{
	fgets(buf, size, stdin);	
}

void	phase_defused(char *input, const int phase_number)
{
	char	*str;
	int		osef;

	if (phase_number != 6)
		return ;
	if ((sscanf(input, "%d %s", &osef, &str) == 2) &&
			(strings_not_equal(str, "austinpowers")))
		{
			printf("Curses, you've found the secret phase !\n");
			printf("But finding it and solving it are quite different...\n");
			secret_phase();
		}
	printf("Congratulation! You've defused the bomb !\n");
}

size_t	string_lenght(const char *str)
{
	return (strlen(str));
}

int		strings_not_equal(const char *s1, const char *s2)
{
	return (strncmp(s1, s2, strlen(s2)));
}

/*****************************************************************************/

/*
** this is not exactly what happens but the logic is exactly the same
*/

int		phase_1(const char *buf)
{
	if (!strcmp("Public speaking is very easy.\n", buf))
	{
		puts("Phase 1 defused. How about the next one?");
		return (EXIT_SUCCESS);
	}
	explode_bomb();
	return (EXIT_FAILURE);
}

/*****************************************************************************/

int		phase_2(const char *line)
{
	int			tab[6];		// sub 0x20
	int			edx;
	size_t		index;

	read_six_numbers(line, tab);
	if (tab[0] != 1)
	{
		explode_bomb();
		return (EXIT_FAILURE);
	}
	// esi = &tab[0]
	index = 1;							// mov ebx, 0x1
	while (index <= 5)					// cmp ebx, 0x5 & jle phase_2+46
	{
		edx = (index + 1) * tab[index - 1];	// imul -0x4(%esi,%ebx,4),%eax
		// printf("[%lu] need: %d got: %d\n", index, edx, tab[index]);
		if (edx != tab[index])
			explode_bomb();
		index += 1;						// inc ebx
	}
	return (EXIT_SUCCESS);				// mov rax, 0x0
}


/*****************************************************************************/

int		phase_3(const char *buf, void *ptr)
{
	char	c;		// ebp-0x4
	int		a;		// ebp-0x5
	int		b;		// ebp-0xc

	if (sscanf(buf, "%d %c %d", &a, &c, &b) <= 2)
		explode_bomb();
	if (a != 0x7)
		explode_bomb();
	//	eax = a * 4 + 134515680; and then a jump on this is perfomed
	// well, this shit just put 'b' at the end of ptr, ptr contains argv[0]
	ptr = (void*)((((size_t)ptr & 0xffffff00) | 'b'));
	if (c != (char)((size_t)ptr & 0x000000ff))
		explode_bomb();
	if (b != 0x20c)		// not sure ?
		explode_bomb();
	return (EXIT_SUCCESS);
}

/*****************************************************************************/

size_t	func4(size_t n)
{
	char	buf[16];		// sub esp, 0x10	(may be 4 int ?)

	if (n <= 1)
		return (1);
	return (func4(n - 1) + func4(n - 2));
}

int		phase_4(const char *buf, char *ptr)
{
	int		x;	// ebp-0x4

	// ptr = mov    edx,DWORD PTR [ebp+0x8]
	// this loop is SO damn fucking hardcore: they perfomed a
	if (sscanf(buf, "%d", &x) != 1)		// x must be 0x37 (see func4)
		explode_bomb();
	if (func4(x) != 0x37)
		explode_bomb();	
	return (EXIT_SUCCESS);
}

/*
** this is for you mates, to understand what the fuck func4 does
** and why the correct answer is 9
*/

void 	explain_phase_4(void)
{
	size_t		ret;

	for (size_t i = 0; i < 20; i++)
	{
		ret = func4(i);
		printf("%2ld -> %4ld -> %4lx\n", i, ret, ret);
	}
}

/*****************************************************************************/

//opekma
int		phase_5(char *input)
{
	const char *array = "isrveawhobpnutfg\260\001";
	char res[6];

	if (string_lenght(input) != 6)
		explode_bomb();
	for (int x = 0; x <= 5 ; x++)
		res[x] = array[input[x] & 0xf];
	if (strings_not_equal(res, "giants"))
		explode_bomb();
	return (EXIT_SUCCESS);
}

/*****************************************************************************/

void phase_6(char *input)
{
	// input at ebp + 0x8; 0x804b810
	const int node[6] = { 253, 725, 301, 997, 212, 432 };
	 // 0xfd; ebp - 0x34; 0x804b26c;
	 // 0x2d5; ebp - 0x3c; 0x804b264;
	 // 0x12d; ebp - 0x3c; 0x804b254;
	 // 0x3e5; ebp - 0x40; 0x804b248;
	 // 0xd4; ebp - 0x43; 0x804b23c;
	 // 0x1b0; ebp - 0x48; 0x804b238;
	int *tab; // ebp - 0x18 => ebp ; tab[0x18]; 
	int i = 0;
	int j = 0;
	int target = 0;

	if (!read_six_numbers(input, tab))
		explode_bomb();
	while (i <= 5)
	{
		if (tab[i] - 1 > 6)
			explode_bomb();
		j = i;
		++j;
		while (j <= 5)
		{
			if (tab[j] == tab[i])
				explode_bomb();
			j++;
		}
	}
	while (i <= 5)
	{
		target = node[tab[i]];
		// take the vale of in the node tab pointed by our entry and check if the next one is below
		if (target < node[tab[i + 1]])
			explode_bomb();
		++i;
	}
}

/*****************************************************************************/

int		fun7(void *ptr)
{
	// n[48] = 0x3e9; // 1001
	// n[47] = 0x63; // 99
	// n[43] = 0x14; // 18
	// n[44] = 0x23; // 39
	// n[41] = 0x1; // 
	// n[34] = 0x6b; // 107
	// n[33] = 0x2d; // 45
	// n[22] = 0x32; // 50
	// n[1] = 0x24; // 40

	return (0);
}

int		secret_phase(void)
{
	char		buf[0x14];
	size_t		x;

	x = 0;
	read_line(buf, 0x14);
	sscanf(buf, "%ld", &x);
	if ((x - 1) > 1000)
		explode_bomb();
	fun7(buf);
	return (EXIT_SUCCESS);	
}

/*****************************************************************************/



int		main(int ac, char **av)
{
	char		buf[80];
	char		phase4[80];

	puts("Welcome this is my little bomb !!!! You have 6 stages with\n"
		"only one life good luck !! Have a nice day!");
	read_line(buf, 80);
	phase_1(buf);
	phase_defuse(buf, 1);
	read_line(buf, 80);
	phase_2(buf);
	phase_defuse(buf, 2);
	puts("That's number 2.  Keep going!");	
	read_line(buf, 80);
	phase_3(buf, av[0]);
	phase_defuse(buf, 3);
	puts("Halfway there!");
	read_line(phase_4, 80);
	phase_4(phase_4, av[0]);
	phase_defuse(buf, 4);
	puts("So you got that one.  Try this one.");
	read_line(buf, 80);
	phase_5(buf);
	phase_defuse(buf, 5);
	puts("Good work!  On to the next...");
	read_line(buf, 80);
	phase_6(buf);
	phase_defuse(phase4, 6);
	return (EXIT_SUCCESS);
}
