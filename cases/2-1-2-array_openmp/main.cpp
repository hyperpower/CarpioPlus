#include <iostream>
#include <memory> 
#include <string>
#include "utility/clock.hpp"
#ifdef OPENMP
#include <omp.h>
#endif 
#include "algebra/algebra.hpp"
#include <unistd.h>

using namespace carpio;

struct ThreadInfo{
	int t_num;
	tick_t t_start;
	tick_t t_end;

	void show() const{
		std::cout << "Thread Num      = " << t_num << std::endl;
		std::cout << "Wall Time dt    = " << Clock::TimespanToMillisecondsD(t_start, t_end) << " mm"<< std::endl;
	}
};

void fun(int n){
	for(int i =0; i < 1000000; i++){
		double r = n + i;
		double c = r * n;
	}
}

int main(int argc, char** argv) {
	tick_t start = Clock::Tick();
	std::list<ThreadInfo*> listti;
	int nt = 10;
    omp_set_num_threads(nt);
#ifdef OPENMP
	std::cout << "OpenMP" << std::endl;
#pragma omp parallel
{
	#pragma omp for
	for(int n=0; n<10; ++n) {
	    usleep(1000);
	}
}
#endif
	for(auto& pti : listti){
		pti->show();
	}
	tick_t end = Clock::Tick();
	std::cout << "NT = " << nt;
	std::cout << " WT = " << Clock::TimespanToMillisecondsD(start, end) << " mm"<< std::endl;
}