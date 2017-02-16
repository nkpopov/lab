
/* This file contains simple implementation of the insertion sorting
 * algorithm. For more infromation about insertion sort you can read
 * the corresponding wikipedia article.
 */

#include "../../common/print_func.h"
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

template<typename T>
void insertion_sort(T &src) {
    for (auto i = src.begin(); i != src.end(); ++i) {
        auto i0 = i;
        auto i1 = i;
        --i0;

        /* .. sorted part .. i0 i1 .. unsorted part .. */
        while (i1 != src.begin() && *i0 > *i1) {
            std::swap(*i0, *i1);
            --i1;
            --i0;
        }
    }
}

/* Get length of the source vector from the command line arguments. 
 * If no command line arguments provided the lenght is expected to 
 * be zero. If no -n parameter is found of if wrong option is
 * specified then return -1 and print usage message to stderr.
 */
int get_len(int argc, char *argv[]) {
    int len = 0;
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt == 'n') {
            len = std::stoi(optarg);
        } else {
            fprintf(stderr, "Usage: %s [-n len]\n", argv[0]);
            return -1;
        }
    }

    return len;
}

int main(int argc, char *argv[]) {
    int len = get_len(argc, argv);
    if (len < 0) {
        return 1;
    }

    std::vector<int> src(len);
    std::generate_n(src.begin(), len,
        [](){return std::rand() % 100;});

    print_iterable(src);
    insertion_sort(src);
    print_iterable(src);
    return 0;
}

