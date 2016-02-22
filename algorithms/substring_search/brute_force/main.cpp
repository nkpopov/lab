
#include <stdio.h>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "unexpected arguments\n");
        fprintf(stderr, "try %s (text) (pattern)\n", argv[0]);
        return 1;
    }

    std::string text    = argv[1];
    std::string pattern = argv[2];

    if (text.size() < pattern.size())
    {
        printf("no match found\n");
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
            printf("match: %d\n", i);
            return 0;
        }
    }
    

    printf("no match found\n");
    return 0;
}

