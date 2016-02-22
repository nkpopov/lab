
#include <stdio.h>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "\n unexpected arguments\n");
        fprintf(stderr, " try %s (text) (pattern)\n\n", argv[0]);
        return 1;
    }

    std::string text    = argv[1];
    std::string pattern = argv[2];

    if (text.size() < pattern.size())
    {
        printf("\n no match found\n\n");
        return 0;
    }

    for (int j, i = 0; i < text.size() - pattern.size() + 1; ++i)
    {
        for (j = 0; j < pattern.size(); ++j)
        {
            if (pattern[j] != text[i + j])
                break;
        }

        if (j == pattern.size())
        {
            printf("\n match: %d\n\n", i);
            return 0;
        }
    }
    

    printf("\n no match found\n\n");
    return 0;
}

