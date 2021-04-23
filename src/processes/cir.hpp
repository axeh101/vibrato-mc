#ifndef CIR_HPP
#define CIR_HPP



#include "process.hpp"
#include "../random/normal.hpp"

template <typename T>
class CoxIngersollRossProcess: public Process<T>
{
private:

	T eta_;
	T volvol_;



public:

	CoxIngersollRossProcess(State<T> initialState, T kappa, T eta, T volvol) :
			Process<T>(initialState, "Cox-Ingresoll-Ross"), eta_(eta), volvol_(volvol) {
		assert(2*kappa*eta > volvol*volvol);
		this->rate_ = kappa;
	}
	virtual~CoxIngersollRossProcess() = default;

	virtual T drift() const {
		return this->rate_ * (eta_- this->priceState_.value);
	}

	virtual T diffusion() const {
		return volvol_ * sqrt(this->priceState_.value);
	}

	virtual T vol() const {
		return volvol_;
	}

	virtual T rate() const {
		return this->rate_;
	}


	virtual T eulerPriceDiff(double h) {
		return Process<T>::eulerPriceDiff(h, normal());
	}


	virtual std::ostream& describe(std::ostream &o) const {
		return Process<T>::describe(o) <<
				"Rate: " << this->rate_ << std::endl <<
				"Kappa:" << this->rate_ << std::endl <<
				"Eta:" << eta_ << std::endl <<
				"Vol Vol:" << volvol_ << std::endl <<
				std::endl;
	}

private:

	NormalDistribution<T> normal = NormalDistribution<T>(0, 1);


};


#endif // CIR_HPP
