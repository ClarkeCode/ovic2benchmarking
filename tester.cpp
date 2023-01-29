#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <thread> //sleeping
template <const int Lowest, const int Highest>
auto dice() {
	static std::uniform_int_distribution<int> distr{Lowest, Highest};
	static std::random_device device;
	static std::mt19937 engine{device()};
	return distr(engine);
}



class Timer {
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point stopTime;
	public:
	Timer() {
		startTime = std::chrono::high_resolution_clock::now();
	}
	void stop() {
		stopTime = std::chrono::high_resolution_clock::now();
	}
	long long inspect() {
		return std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count();
	}
};

class Pop {
	public:
	int provinceID;
	int popType;
	int popSize;
	double conciousness;
	double militancy;
	double literacy;
	double lifeNeedsSatisfaction;
	double dailyNeedsSatisfaction;
	double luxuryNeedsSatisfaction;
};

struct SoAPop {
	std::vector<int> entityID;
	std::vector<int> provinceID;
	std::vector<int> popType;
	std::vector<int> popSize;
	std::vector<double> conciousness;
	std::vector<double> militancy;
	std::vector<double> literacy;
	std::vector<double> lifeNeedsSatisfaction;
	std::vector<double> dailyNeedsSatisfaction;
	std::vector<double> luxuryNeedsSatisfaction;
};

int main() {
	using namespace std;

	static std::uniform_real_distribution<double> distr{0.5, 1.8};
	static std::random_device device;
	static std::mt19937 engine{device()};
	// return distr(engine);
	size_t bignum = 2000;
	std::vector<double> pregenerated(bignum);
	for (size_t x = 0; x < bignum; x++) {
		pregenerated.push_back(distr(engine));
	}
	
	size_t x = 0;
	auto dummypercent = [&]() {
		if (x >= bignum) x = 0;
		return pregenerated[x++];
	};


	cout << "Hello there!" << endl;
	size_t numprov = 2700;
	{
		std::vector<int> provsiz(numprov);
		std::vector<double> provoutputs(numprov);
		std::generate(provsiz.begin(), provsiz.end(), dice<1,6>);

		auto rgo = [&](int psize) {
			double baseProduction = psize * (1.0 + dummypercent());
			double throughput = dummypercent() * (1 + dummypercent() - dummypercent()/3) * (-dummypercent());
			double outputEfficiency = 1 + dummypercent() + dummypercent() + dummypercent() + dummypercent();
			return baseProduction * throughput * outputEfficiency;
		};

		Timer RGOtest;
		std::transform(provsiz.cbegin(), provsiz.cend(), provoutputs.begin(), rgo);
		RGOtest.stop();
		cout << "Took " << RGOtest.inspect() << " microseconds for test simulation with " << numprov << " provinces\n";
	}

	{
		std::vector<Pop> pops(numprov * 100);
		for (auto pop : pops) {
			pop.provinceID = dice<1,2700>();
			pop.popType = dice<1,12>();
			pop.popSize = dice<10000, 2500000>();
			pop.conciousness = dummypercent();
			pop.militancy = dummypercent();
			pop.literacy = dummypercent();
			pop.lifeNeedsSatisfaction = dummypercent();
			pop.dailyNeedsSatisfaction = dummypercent();
			pop.luxuryNeedsSatisfaction = dummypercent();
		}

		auto popLife = [&](Pop pop) {
			pop.popSize += dice<-10000,10000>();
			pop.conciousness *= dummypercent();
			pop.militancy *= dummypercent();
			pop.literacy *= dummypercent();
			pop.lifeNeedsSatisfaction *= dummypercent();
			pop.dailyNeedsSatisfaction *= dummypercent();
			pop.luxuryNeedsSatisfaction *= dummypercent();
			return pop;
		};

		Timer Poptest;
		std::transform(pops.cbegin(), pops.cend(), pops.begin(), popLife);
		Poptest.stop();
		cout << "Took " << Poptest.inspect() << " microseconds for test simulation with " << pops.size() << " pops (AoS style)\n";
	}

	SoAPop pops;
	for (size_t x = 0; x < numprov * 100; x ++) {
		pops.entityID.push_back(x);
		pops.provinceID.push_back(dice<1,2700>());
		pops.popType.push_back(dice<1,12>());
		pops.popSize.push_back(dice<10000, 2500000>());
		pops.conciousness.push_back(dummypercent());
		pops.militancy.push_back(dummypercent());
		pops.literacy.push_back(dummypercent());
		pops.lifeNeedsSatisfaction.push_back(dummypercent());
		pops.dailyNeedsSatisfaction.push_back(dummypercent());
		pops.luxuryNeedsSatisfaction.push_back(dummypercent());
	}

	auto sizeModify = [](int size) { return size + dice<-10000,10000>(); };
	auto floatingModify = [&](double val) { return val * dummypercent(); };

	Timer soaTest;
	std::transform(pops.popSize.cbegin(), pops.popSize.cend(), pops.popSize.begin(), sizeModify);
	std::transform(pops.conciousness.cbegin(), pops.conciousness.cend(), pops.conciousness.begin(), floatingModify);
	std::transform(pops.militancy.cbegin(), pops.militancy.cend(), pops.militancy.begin(), floatingModify);
	std::transform(pops.literacy.cbegin(), pops.literacy.cend(), pops.literacy.begin(), floatingModify);
	std::transform(pops.lifeNeedsSatisfaction.cbegin(), pops.lifeNeedsSatisfaction.cend(), pops.lifeNeedsSatisfaction.begin(), floatingModify);
	std::transform(pops.dailyNeedsSatisfaction.cbegin(), pops.dailyNeedsSatisfaction.cend(), pops.dailyNeedsSatisfaction.begin(), floatingModify);
	std::transform(pops.luxuryNeedsSatisfaction.cbegin(), pops.luxuryNeedsSatisfaction.cend(), pops.luxuryNeedsSatisfaction.begin(), floatingModify);
	soaTest.stop();
	cout << "Took " << soaTest.inspect() << " microseconds for test simulation with " << pops.entityID.size() << " pops (SoA style)\n";

}