
/* This file contains simple implementation of the selection sort
 * algorithm. For more information about the algorithm you can read
 * related wikipedia page.
 */

#include "../../common/print_func.h"
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

template<typename T>
static void selection_sort(T &src) {
    for (auto i = src.begin(); i != src.end(); ++i) {
        auto j = std::min_element(i, src.end());
        std::swap(*i, *j);
    }
}

int main(int argc, char *argv[]) {
    int len = 0;
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
        case 'n':
            len = std::stoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-n len]\n", argv[0]);
            return 1;
        }
    }

    std::vector<int> src(len);
    std::generate_n(src.begin(), len,
        [](){return std::rand() % 100;});

    print_iterable(src);
    selection_sort(src);
    print_iterable(src);
    return 0;
}


