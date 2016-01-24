
#pragma once

/*
 * Get number of builtin commands that are implemented in the current
 * version of interpreter.
 */
extern int lsh_num_builtins(void);

/* 
 * Array of builtin commands names. Use lsh_num_builins function for
 * finding size of the array and iterating through it.
 */
extern char *builtin_name[];

/*
 * Array of builin commnands handlers. Use lsh_num_builtins function
 * for finding size of the array and iterating through it.
 */
extern int (*builtin_func[])(char **);



