
#include <stdio.h>
#include <vector>
#include <string>

typedef std::vector<std::vector<int>> dfa_t;

/* Construct DFA from pattern string in arguments (patt). DFA is
 * specified in terms of transition function. Transition function is
 * defined in form of 2d table, such that 1'st dimention is input
 * character and 2'nd dimension is current state of automaton.
 */
dfa_t construct_dfa(const std::string &patt) {

    /* It's expected that input characters are from the string in
     * ASCII encoding. That's why size of 1'st dimension is 256.
     */
    dfa_t dfa(256u, std::vector<int>(patt.size(), 0u));

    /* Initial conditions. Rest of first column values are 0u because
     * in case of mismatch here we should return to 0 state of
     * automaton. 
     */
    dfa[patt[0]][0] = 1;
    
    for (int x = 0, j = 1; j < patt.size(); ++j) {

        /* Copy mismatch cases */
        for (int c = 0; c < 256u; ++c) {
            dfa[c][j] = dfa[c][x];
        }

        /* Match case */
        dfa[patt[j]][j] = j + 1;

        /* Update restart case */
        x = dfa[patt[j]][x];
    }

    return dfa;
}

int main(int argc, char *argv[]) {
    
    if (argc != 3){
        fprintf(stderr, "unexpected command line arguments\n");
        fprintf(stderr, "try %s (text) (pattern)\n", argv[0]);
        return 1;
    }

    /* Parse command line arguments */
    std::string text = argv[1];
    std::string patt = argv[2];

    /* Construct DFA from pattern. DFA is specified by the transition
     * function which is described in terms of 2d table 'dfa'. First
     * dimension of table is input symbol, second dimension of table
     * is current state of automaton. Code below computes next state
     * of automaton: dfa[input_symbol][current_state].
     */
    dfa_t dfa(construct_dfa(patt));

    /* Current state of DFA */
    int j = 0;
    
    for (int i = 0; i < text.size(); ++i) {

        /* Make a DFA step */
        j = dfa[text[i]][j];

        if (j == (int)patt.size()) {
            printf("match at position %d\n", i);
            printf("ihha! it works!\n");
            return 0;
        }
    }

    printf("match not found\n");
    printf("... \n");
    return 0;
}

