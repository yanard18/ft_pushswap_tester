#ifndef UNIT_H
# define UNIT_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Standard ANSI Colors */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

typedef struct s_test {
	const char *name;
	int (*func)(void);
	struct s_test *next;
}	t_test;

extern t_test *g_test_lst;

void register_test(const char *name, int (*func)(void));

# define REGISTER_TEST(func_ptr)				\
	__attribute__((constructor))				\
	static void register_##func_ptr(void) {		\
		register_test(#func_ptr, func_ptr);		\
	}											

# define TEXT(...) ({ \
    static char _text_buf[BUFFER_SIZE]; \
    snprintf(_text_buf, BUFFER_SIZE, __VA_ARGS__); \
    _text_buf; \
})

# define TEST(exp, res, msg) do {												   \
        long long _exp = (long long)(exp);                                         \
        long long _res = (long long)(res);                                         \
        if (_exp == _res) {                                                        \
            printf("  [+] %s == %s --> res: %lld\n", #exp, #res, _exp);            \
        } else {                                                                   \
            printf(RED "  [-] %s\n" RESET, msg);								   \
            return (0);                                                            \
        }                                                                          \
    } while (0)

# define TEST_STR(exp, res) do {                                                   \
        const char *_exp = (exp);                                                  \
        const char *_res = (res);                                                  \
        int _match = 0;                                                            \
        if (_exp == NULL && _res == NULL)                                          \
            _match = 1;                                                            \
        else if (_exp != NULL && _res != NULL && strcmp(_exp, _res) == 0)          \
            _match = 1;                                                            \
        if (_match) {                                                              \
            printf("  [+] %s == %s --> res: \"%s\"\n",                             \
                   #exp, #res, _res ? _res : "(null)");                            \
        } else {                                                                   \
            printf(RED "  [-] %s == %s --> expected: \"%s\", was: \"%s\"\n" RESET, \
                   #exp, #res, _exp ? _exp : "(null)", _res ? _res : "(null)");    \
            return (0);                                                            \
        }                                                                          \
    } while (0)

# define BUFFER_SIZE	1024
# define TEST_STDOUT_FUNC(exp_f, res_f, ...) do {                         \
        char    buf[BUFFER_SIZE];                                         \
        char    buf2[BUFFER_SIZE];                                        \
        int     fd[2];                                                    \
        int     saved_stdout;                                             \
        fflush(stdout);                                                   \
        saved_stdout = dup(1);                                            \
        pipe(fd);                                                         \
        dup2(fd[1], 1);                                                   \
        int exp_ret = exp_f(__VA_ARGS__);                                 \
        printf("%c", '\0');                                               \
        fflush(stdout);                                                   \
        memset(buf, 0, BUFFER_SIZE);                                      \
        read(fd[0], buf, sizeof(buf) - 1);                                \
        int res_ret = res_f(__VA_ARGS__);                                 \
        printf("%c", '\0');                                               \
        fflush(stdout);                                                   \
        memset(buf2, 0, BUFFER_SIZE);                                     \
        read(fd[0], buf2, sizeof(buf2) - 1);                              \
        int _passed = (strcmp(buf, buf2) == 0 && exp_ret == res_ret);     \
        if (_passed)                                                      \
            dprintf(saved_stdout, "[+] %s(%s) --> stdout: \"%s\" ret: %i\n", #res_f, #__VA_ARGS__, buf, exp_ret); \
        else                                                              \
            dprintf(saved_stdout, RED "[-] %s(%s) --> expected stdout: \"%s\", was: \"%s\" expected ret: %i was: %i\n" RESET, #res_f, #__VA_ARGS__, buf, buf2, exp_ret, res_ret); \
        dup2(saved_stdout, 1);                                            \
        close(fd[1]);                                                     \
        close(fd[0]);                                                     \
        close(saved_stdout);								              \
        if (!_passed) return (0);									      \
    } while (0)

#endif
