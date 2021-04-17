#ifndef HELPER_HPP
#define HELPER_HPP


#include <iostream>
#include <vector>

template <typename T>
struct state {
    double time;
    T value;
};

template <typename T>
std::ostream & operator<<(std::ostream & o, state<T> const & s) {
    return o << s.time << ";" << s.value;
}

template <typename T>
struct path : protected std::vector<state<T>> {
    using vec = std::vector<state<T>>;
    using vec::vec;
    using vec::operator[];
    using vec::begin; 
    using vec::end;
    using vec::size;
};

template <typename T>
std::ostream & operator<<(std::ostream & o, path<T> const & p) {
    for (auto const & st : p)
        o << st << std::endl;
    return o << std::endl;
}

#endif  // HELPER_HPP
