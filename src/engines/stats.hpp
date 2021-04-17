#include <iostream>
#include <math.h>

using namespace std;

/*! \class Statistics
 *  \brief Computes the sample statistics of a random variable
 *
 *  Computes the sample statistics for  multithreading support for Monte Carlo simulation
 *  It can be suited for other multi threaded objectives
 */
class Statistics
{
public:
    Statistics(unsigned size, double sum=0, double sum_squared=0)
        :size(size), sum(sum), sum_squared(sum_squared){};
    virtual ~Statistics();

    double mean() const
    {
        return sum / size;
    }

    double var() const
    {
        return (sum_squared - size * mean() * mean()) / (size - 1);
    }

    double std() const
    {
        return sqrt(var());
    }

    double confidence_size() const
    {
        return 1.96 * sqrt(var()/size);
    }
    Statistics & operator+=(Statistics const & s)
    {
        size += s.size;
        sum += s.sum;
        sum_squared += s.sum_squared;
        return *this;
    }

    friend Statistics operator+(Statistics const & lhs, Statistics const & rhs)
    {
        return {lhs.size + rhs.size, lhs.sum+rhs.sum, lhs.sum_squared + rhs.sum_squared};
    }

    friend Statistics operator*(double alpha, Statistics const & stat) {
        return { stat.size, alpha * stat.sum, alpha * stat.sum_squared};
    }

    friend std::ostream & operator<<(std::ostream & o, Statistics const & stat) {
        return o << "Size: " << stat.size
         << "\tMean: " << stat.mean()
         << "\tVar: " << stat.var();
    }
        
protected:
    unsigned size;
    double sum;
    double sum_squared;
};
