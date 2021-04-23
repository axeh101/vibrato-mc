#ifndef HESTON_HPP
#define HESTON_HPP

#include "process.hpp"
#include "../random/normal.hpp"

template <typename T>
class HestonProcess: public Process<T>
{
private:

	T correl_;
	Process<T>* volProcess_;
public:
	HestonProcess(State<T> initialState, Process<T>* volProcess, T rate, T correl) :
			Process<T>(initialState, "Heston"), correl_(correl), volProcess_(volProcess) {
		this->vol_ = volProcess->initialState.value;
		this->rate_ = rate;
	}
	virtual ~HestonProcess(){
		volProcess_ = nullptr;
		delete volProcess_;
	}


	virtual T drift() const {
		return this->rate_ * this->priceState_.value;
	}

	virtual T diffusion() const {
		return vol() * this->priceState_.value;
	}

	virtual T vol() const {
		return sqrt(volProcess_->priceState().value);
	}

	virtual T rate() const {
		return this->rate_;
	}

	virtual State<T> movePriceEuler(double h, T Wh) {
		this->priceState_ = this->nextPriceEuler(h, Wh);
		T Bh = Wh * correl_ + sqrt(1-correl_*correl_) * normal();
		this->volProcess_->movePriceEuler(h, Bh);
		return this->priceState_;
	}

	virtual T eulerPriceDiff(double h) {
		return Process<T>::eulerPriceDiff(h, normal());
	}

	virtual T diffDiffusionVega() const {
		return this->priceState_.value;
	}

	virtual T diffDriftRho() const {
		return this->priceState_.value;
	}

	virtual T diffDiffusionX() const {
		return this->vol_;
	}

	virtual T diffDriftX() const {
		return this->rate_;
	}

	virtual std::ostream& describe(std::ostream &o) const {
		return Process<T>::describe(o) <<
				"Rate: " << this->rate_ << std::endl <<
				"Volatility: " << *this->volProcess_ << std::endl <<
				"Correlation:" << correl_ << std::endl <<
				std::endl;
	}

private:

	NormalDistribution<T> normal = NormalDistribution<T>(0, 1);

};


#endif  // HESTON_HPP
