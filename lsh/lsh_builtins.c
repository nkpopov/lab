
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include "lsh_builtins.h"

extern int lsh_cd   (char **args);
extern int lsh_help (char **args);
extern int lsh_exit (char **args);

char *builtin_name[] =
{
    "cd",
    "help",
    "exit"
};

int (*builtin_func[])(char **) =
{
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins(void)
{
    return sizeof(builtin_name) / sizeof(char *);
}

int lsh_cd(char **args)
{
    struct passwd *pw;
    char *dir;
    
    if (args[1] == NULL)
    {
        pw = getpwuid(geteuid());

        if (pw == NULL)
        {
            fprintf(stderr, "lsh_cd: getpwuid error\n");
            fprintf(stderr, "lsh_cd: %s\n", strerror(errno));
            return 1;
        }

        dir = pw->pw_dir;
    }
    else
    {
        dir = args[1];
    }
    
    if (chdir(dir) != 0)
    {
        fprintf(stderr, "lsh_cd: chdir error\n");
        fprintf(stderr, "lsh_cd: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

int lsh_help(char **args)
{
    int i;
    printf("Stephen Brennan's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < lsh_num_builtins(); ++i)
        printf(" %s\n", builtin_name[i]);

    printf("Use the man command for information on other programs.\n");
    return 0;
}

int lsh_exit(char **args)
{
    int status = (args[1] != NULL) ? atoi(args[1]) : 0;
    exit(status);
    return 0;
}

