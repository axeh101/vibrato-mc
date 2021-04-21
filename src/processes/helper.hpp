#ifndef HELPER_HPP
#define HELPER_HPP


#include <iostream>
#include <vector>

template <typename T>
struct State {
    double time;
    T value;

    friend std::ostream & operator<<(std::ostream & o, State<T> const & s) {
        return o << s.time << ";" << s.value;
    }
};

template <typename T>
struct StateWithTangent {
    double time;
    T value;
    T tangent;

    friend std::ostream & operator<<(std::ostream & o, StateWithTangent<T> const & s) {
        return o << s.time << ";" << s.value << ";" << s.tangent;
    }
};



template <typename T>
struct Path : protected std::vector<State<T>> {
    using vec = std::vector<State<T>>;
    using vec::vec;
    using vec::operator[];
    using vec::begin;
    using vec::end;
    using vec::size;

    friend std::ostream & operator<<(std::ostream & o, Path<T> const & p) {
    for (auto const & st : p)
        o << st << std::endl;
    return o << std::endl;
    }
};

template <typename T>
struct PathWithTangent : protected std::vector<StateWithTangent<T>> {
    using vec = std::vector<State<T>>;
    using vec::vec;
    using vec::operator[];
    using vec::begin;
    using vec::end;
    using vec::size;

    friend std::ostream & operator<<(std::ostream & o, PathWithTangent<T> const & p) {
    for (auto const & st : p)
        o << st << std::endl;
    return o << std::endl;
    }
};
#endif  // HELPER_HPP
