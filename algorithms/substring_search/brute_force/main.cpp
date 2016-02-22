
#include <stdio.h>
#include <string>

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "unexpected arguments\n");
        fprintf(stderr, "try %s (text) (pattern)\n", argv[0]);
        return 1;
    }

    std::string text = argv[1];
    std::string patt = argv[2];

    if (text.size() < patt.size()) {
        printf("no match found\n");
        printf("pattern length greater than text length\n");
        return 0;
    }

    for (int j, i = 0; i < text.size() - patt.size() + 1; ++i) {
        for (j = 0; j < patt.size(); ++j) {
            if (patt[j] != text[i + j])
                break;
        }

        if (j == patt.size()) {
            printf("match found %d\n", i);
            printf("ihha! 1 2 3 4\n");
            return 0;
        }
    }
    
    printf("no match found\n");
    printf("...\n");
    return 0;
}

