
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> compute_shift_table(const std::string &patt) {
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
    int skip     = 0;
    
    for (int i = 0; i < text_len - patt_len + 1; i += skip) {
        skip = 0;

        for (int j = patt_len - 1; j >= 0; --j) {
            if (patt[j] != text[i + j]) {
                skip = std::max(1, j - right[text[i + j]]);
                break;
            }
        }

        if (skip == 0) {
            printf("match is found at %d\n", i);
            printf("ihha! 1 2 3 4\n");
            return 0;
        }
    }

    printf("match isn't found\n");
    printf("...\n");
    return 0;
}


