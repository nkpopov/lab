
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "lsh_builtins.h"

/* 
 * Get username of calling process. This function returns pointer to
 * the string which is stored deep in OS internals. 
 */
static const char *get_username()
{
    uid_t id = geteuid();
    struct passwd *pw = getpwuid(id);

    if (!pw)
    {
        fprintf(stderr, "get_username: getpwuid error\n");
        fprintf(stderr, "get_username: %s\n", strerror(errno));
        exit(1);
    }

    return pw->pw_name;
}

/* 
 * Get hostname of calling process. This function returns pointer to
 * the string which is stored deep in OS internals.
 */
static const char *get_hostname()
{
    static char s_hn[32];

    if (gethostname(s_hn, sizeof(s_hn) / sizeof(char)) != 0)
    {
        fprintf(stderr, "get_hostname: gethostname error\n");
        fprintf(stderr, "get_hostname: %s\n", strerror(errno));
        exit(1);
    }

    return s_hn;
}

/* 
 * Read next command from stdin and return string containing it. There
 * is no string continuation symbol ('\') handling implemented.
 */
static char *lsh_readline()
{
    static size_t s_blen = 0;
    static char  *s_buff = NULL;
    
    if (getline(&s_buff, &s_blen, stdin) == -1)
    {
        fprintf(stderr, "lsh_readline: getline error\n");
        fprintf(stderr, "lsh_readline: %s\n", strerror(errno));
        exit(1);
    }
    
    return s_buff;
}

/*
 * Split command string on the sequence of tokens. Delimeters of
 * fields are space, \t, \r and \n.
 */
static char **lsh_splitline(char *line)
{
    static int    s_blen = 1024;
    static char **s_buff = NULL;
    static char  *s_delm = " \t\r\n";

    char *token;
    int pos = 0;

    s_buff = realloc(s_buff, s_blen * sizeof(char **));

    if (!s_buff)
    {
        fprintf(stderr, "lsh_splitline: realloc error\n");
        fprintf(stderr, "lsh_splitline: %s\n", strerror(errno));
        exit(1);
    }

    token = strtok(line, s_delm);
    
    while (token != NULL)
    {
        s_buff[pos] = token;
        ++pos;

        if (pos >= s_blen)
        {
            s_blen *= 2;
            s_buff  = realloc(s_buff, s_blen * sizeof(char **));

            if (!s_buff)
            {
                fprintf(stderr, "lsh_splitline: realloc error\n");
                fprintf(stderr, "lsh_splitline: %s\n", strerror(errno));
                exit(1);
            }
        }

        token = strtok(NULL, s_delm);
    }

    s_buff[pos] = NULL;
    return s_buff;
}

/*
 * Launch new process wich is described by the name args[0] and
 * arguments list args. Return 0 if launch is succesfull, otherwise
 * return 1.
 */
static int lsh_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        if (execvp(args[0], args) < 0)
        {
            fprintf(stderr, "lsh_execute: execvp error\n");
            fprintf(stderr, "lsh_exectue: %s\n", strerror(errno));
            return 1;
        }
    }
    else if (pid < 0)
    {
        fprintf(stderr, "lsh_execute: fork error\n");
        fprintf(stderr, "lsh_execute: %s\n", strerror(errno));
        return 1;
    }

    do
    {
        if (waitpid(pid, &status, WUNTRACED) < 0)
        {
            fprintf(stderr, "lsh_execute: waitpid error\n");
            fprintf(stderr, "lsh_execute: %s\n", strerror(errno));
            return 1;
        }
        
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    return 0;
}

/*
 * Check if command specified by arguments list is builtin or not and
 * handle it properly.
 */
int lsh_execute(char **args)
{
    int i;
    
    if (args[0] == NULL)
        return 0;

    for (i = 0; i < lsh_num_builtins(); ++i)
        if (strcmp(args[0], builtin_name[i]) == 0)
            return (*builtin_func[i])(args);

    return lsh_launch(args);
}

/* Main execution loop */
void lsh_loop(void)
{
    const char *user = get_username();
    const char *host = get_hostname();

    char  *line;
    char **args;
    
    while (1)
    {
        /* Print invitation */
        char *cwd = getcwd(NULL, 0);
        printf("%s@%s:%s# ", user, host, cwd);
        fflush(stdout);
        free(cwd);

        /* Read command from stdin */
        line = lsh_readline();

        /* Run tokenizer */
        args = lsh_splitline(line);

        /* Execute command */
        lsh_execute(args);
    }
}

int main(int argc, char *args[])
{
    /* Main interpreter loop */
    lsh_loop();
    return 0;
}

    
