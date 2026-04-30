#include "unit.h"

t_test *g_test_lst = NULL;
static int	g_total_tests = 0;
static int	g_passed = 0;

void register_test(const char *name, int (*func)(void)) {
    t_test *new_test = malloc(sizeof(t_test));
	if (!new_test)
		return ;
    new_test->name = name;
    new_test->func = func;
    new_test->next = g_test_lst;
    g_test_lst = new_test;
	g_total_tests++;
	printf("register: %s\n", name);
}

int	main(int argc, char **argv)
{
	int verbose = (argc > 1 && strcmp(argv[1], "-v") == 0);
	
	printf("-----------------------------------------\n");
	while (g_test_lst)
	{
		FILE *tmp = tmpfile();
		int tmp_fd = fileno(tmp);
		fflush(stdout);
		int real_stdout = dup(1);
		dup2(tmp_fd, 1);
		int test_result = g_test_lst->func();
		fflush(stdout);
		dup2(real_stdout, 1);
		close(real_stdout);
		if (test_result)
		{
			printf(GREEN "OK: %s\n" RESET, g_test_lst->name);
			g_passed++;
		}
		else
		{
			printf(RED "KO: %s\n" RESET, g_test_lst->name);
		}
		if (!test_result || verbose)
		{
			rewind(tmp);
			char line[1024];
			while (fgets(line, sizeof(line), tmp))
				printf("\t%s", line); 
		}
		fclose(tmp);
		t_test *next_test = g_test_lst->next;
		free(g_test_lst);
		g_test_lst = next_test;
	}
	if (g_passed == g_total_tests)
		printf(GREEN "RESULT: %d/%d\n" RESET, g_passed, g_total_tests);
	else
		printf(RED "RESULT: %d/%d\n" RESET, g_passed, g_total_tests);
	return (0);
}
