#include <iostream>
#include <memory> 
#include <string>
#include "utility/clock.hpp"
#ifdef OPENMP
#include <omp.h>
#endif 
#include "algebra/algebra.hpp"
#include "io/gnuplot.hpp"
#include "utility/tinyformat.hpp"
#include <unistd.h>
#include <memory>

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
// input singal task time in milliseconds
// ouput wall time of parallel runing
double TimeTest(double tt, int nt){
	tick_t start = Clock::Tick();
	omp_set_num_threads(nt);
#ifdef OPENMP
#pragma omp parallel
{
	#pragma omp for
	for(int n=0; n<nt; ++n) {
	    usleep(St(tt * 1000));
	}
}
#endif
	tick_t end = Clock::Tick();
	return Clock::TimespanToMillisecondsD(start, end);
}

std::shared_ptr<Gnuplot_actor> TimeTestRun(int nt){
	
	std::vector<double> vt = {1,2,3,4,5,6,7,8,9,
	                          10,20,30,40,50,60,70,80,90,
	                          100,200,300,400,500,600,700,800,900,
	                          1000};
	std::vector<double> wt(vt.size());
	std::vector<double> rt(vt.size()); //ratio = vt / wt * 100%

	// tfm::format(std::cout, "%10s %10s %10s\n", "Sigle", "Wall time", "Ratio");
	for(int i = 0; i < vt.size(); i++){
		wt[i] = TimeTest(vt[i], nt);
		rt[i] = wt[i]/vt[i] * 100;
		// tfm::format(std::cout, "%10d %10d %10d\n", vt[i], wt[i], rt[i]);
	}

	return GnuplotActor::XY(vt, rt);
}

void TimeTestPlot(){
	Gnuplot gnu;
	gnu.set_terminal_png(
            "./fig/timetest.png", //const std::string& filename,
            800,
            600,
            "Helvetica",
            14);
	gnu.set_grid();
	gnu.set_xlogscale();
	gnu.set("key right top");
	gnu.set_yrange(80, 300.0);
	gnu.set_xlabel("Time of single task (ms)");
	gnu.set_ylabel("Time ratio (Parallel Wall Time / Single Task Time (%)");
	std::vector<int> vnt = {2, 4, 10, 20, 40};
	for(auto& nt: vnt){
		std::cout << "Run by " << nt << " Threads" << std::endl;
		auto a = TimeTestRun(nt);
		a->command() = "using 1:2 title \"Num of Threads = " + ToString(nt) + "\" ";
		a->style()   = "with lines lw 2";
		gnu.add(a);
	}
	gnu.plot();
}

int main(int argc, char** argv) {
	TimeTestPlot();
}