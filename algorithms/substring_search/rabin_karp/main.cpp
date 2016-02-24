
#include <stdio.h>
#include <string>

/* Compute hash function for string in arguments (key) using Horner's
 * method (linear time method). Parameter q is the same as in video
 * lectures. There is next logic behind choise of this parameter - q
 * should be large prime number. Why prime? It's a good question.
 */
long hash(const std::string &key) {
    
    long h = 0l;
    long q = 997l;
    for (int i = 0; i < key.size(); ++i) {
        h = (256 * h + key[i]) % q;
    }

    return h;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "unexpected arguments\n");
        fprintf(stderr, "try %s (text) (pattern)\n", argv[0]);
        return 1;
    }

    std::string text = argv[1];
    std::string patt = argv[2];

    /* Precompute pattern's hash, choose value of the modulo parameter
     * (it should be large prime number), precompute R**(M - 1) where
     * R is the base of notation and M is length of pattern. This
     * value is required for further computation of hash function from
     * text substrings. 
     */
    long phash = hash(patt);
    long qval  = 997l;
    long rm    = 1l;
    
    for (int i = 1; i < patt.size(); ++i) {
        rm = (256 * rm) % qval;
    }

    long thash = hash(text.substr(0, patt.size()));
    if (thash == phash) {
        printf("match is found at %d\n", 0);
        printf("ihha!\n");
        return 0;
    }

    for (int i = patt.size(); i < text.size(); ++i) {
        thash = (thash + qval - (rm * text[i - patt.size()]) % qval) % qval;
        thash = (thash * 256  + text[i]) % qval;
        if (phash == thash) {
            printf("match is found at %d\n", i - patt.size() - 1);
            printf("ihha!\n");
            return 0;
        }
    }

    printf("match not found\n");
    printf("...\n");
    return 0;
}
    

