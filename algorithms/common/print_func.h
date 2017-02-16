
#ifndef _COMMON_PRINT_FUNC_H
#define _COMMON_PRINT_FUNC_H

#include <iterator>
#include <iostream>

template<typename T>
static inline void print_iterable(const T &src) {
    typedef typename T::value_type value_type;
    auto it = std::ostream_iterator<value_type>(std::cout, "; ");
    std::copy(src.begin(), src.end(), it);
    std::cout << "\n";
}

#endif  /* _COMMON_PRINT_FUNC_H */


