
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

/* Compute shift table for Boyer-Moore algorithm. This version of
 * algorithm is provided by Introduction To Algorithms video lectures
 * by Robert Sedgewick from coursera and differs from wikipedia
 * version of algorithm. 
 * 
 * Actual shift is computed in case of mismatch at the j'th character
 * of pattern is computed using equation: shift = j - table[text[i + j]]. 
 * This formula describes next sequence of rules:
 * 
 * (1) If mismatched character not in the pattern then we want to shift
 *     to position right after it.
 * (2) If mismatched character in the pattern then situation is more
 *     complicated. In this case we want to align text with the same
 *     right most character in the pattern, but we want to avoid
 *     backup. That's why we are using std::max in the computation of
 *     shift parameter.
 */
std::vector<int> compute_shift_table(const std::string &patt) {

    /* First of all, set initial values for characters that are not in
     * the pattern. Second step is to initialize values for characters
     * that are in the pattern.
     */   
    std::vector<int> right(256u, -1);
    for (int i = 0; i < patt.size(); ++i) {
        right[patt[i]] = i;
    }

    return right;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "unexpected command line arguments\n");
        fprintf(stderr, "try %s (text) (pattern)\n", argv[0]);
        return 1;
    }

    std::string text = argv[1];
    std::string patt = argv[2];

    if (text.size() < patt.size()) {
        fprintf(stderr, "unexpected text size\n");
        fprintf(stderr, "text size should be >= pattern size\n");
        return 1;
    }

    std::vector<int> right(compute_shift_table(patt));
    
    int text_len = text.size();
    int patt_len = patt.size();
    int shift     = 0;
    
    for (int i = 0; i < text_len - patt_len + 1; i += shift) {
        shift = 0;
        for (int j = patt_len - 1; j >= 0; --j) {
            if (patt[j] != text[i + j]) {
                shift = std::max(1, j - right[text[i + j]]);
                break;
            }
        }

        if (shift == 0) {
            printf("match is found at %d\n", i);
            printf("ihha! 1 2 3 4\n");
            return 0;
        }
    }

    printf("match isn't found\n");
    printf("...\n");
    return 0;
}


