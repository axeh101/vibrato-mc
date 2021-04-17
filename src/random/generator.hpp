#ifndef GENERATOR_HPP

#define GENERATOR_HPP

template <typename T>
class Generator{

    public:
        Generator() = default;
        virtual~Generator() = default;
        virtual T operator()() = 0;  
};

#endif
