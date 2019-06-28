#include <iostream>
#include <memory> 
#include <string>
#include "utility/clock.hpp"
#ifdef OPENMP
#include <omp.h>
#endif 
#include "algebra/algebra.hpp"
#include "algebra/io/gnuplot_actor_a.hpp"
#include "io/gnuplot.hpp"
#include "utility/tinyformat.hpp"
#include <unistd.h>
#include <memory>

using namespace carpio;
typedef MatrixV_<Vt>    Mat;
typedef MatrixSCR_<Vt>  MatSCR;
typedef MatrixSCO_<Vt>  MatSCO;
typedef ArrayListV_<Vt> Arr;
typedef Jacobi_<Vt>     Solver;


void plot_residual(const std::string& fn, const Solver& s){
	Gnuplot gnu;
	gnu.set_terminal_png(
            fn, //const std::string& filename,
            800,
            600,
            "Helvetica",
            14);
	gnu.set_grid();
	gnu.set_ylogscale();
	gnu.set_xlabel("Iterations (-)");
	gnu.set_ylabel("Residual (-)");
	gnu.add(GnuplotActor::Lines(s.get_residual_array()));
	gnu.plot();
}

void Read_Matrix(TextFile& f, const std::string& name, MatSCO& mat){
	// std::string name = "685_bus.mtx";
	mm_read_mtx_sparse("./" + name, mat);
	f.add_line(tfm::format("    \"mat_name\" : \"%s\",", name));
	f.add_line(tfm::format("    \"num_row\" : %d,", mat.size_i()));
	f.add_line(tfm::format("    \"num_col\" : %d,", mat.size_j()));
	f.add_line(tfm::format("    \"num_nonezero\": %d", mat.non_zeros()));
}

// input singal task time in milliseconds
// ouput wall time of parallel runing
void SolverTest(const MatSCR& mat){
	tick_t start = Clock::Tick();

	Arr    x(mat.size_i());
	x.assign(0.0);

	Arr    b(mat.size_i());
	b.assign(1.0);
	Solver solver(100000, 1e-4);
	solver.solve(mat, x, b);
	tick_t end = Clock::Tick();
	plot_residual("./fig/residual.png", solver);
	std::cout << "D Time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;
}

double SolverTimeTest(const MatSCR& mat, int nt){
	tick_t start = Clock::Tick();
	Solver solver(100000, 1e-4);
#ifdef OPENMP
	omp_set_num_threads(nt);
	Arr    x(mat.size_i());
	x.assign(0.0);
	Arr    b(mat.size_i());
	b.assign(1.0);
	solver.solve(mat, x, b);
#else
	std::cout << "No OpenMP " << std::endl;
#endif
	tick_t end = Clock::Tick();
	double dt  = Clock::TimespanToMillisecondsD(start, end);
	std::cout << "D Time = " << dt << "ms" << std::endl;
	return dt;
}



void TimeTestPlot(const MatSCR& mat){
	Gnuplot gnu;
	gnu.set_terminal_png(
            "./fig/timetest.png", //const std::string& filename,
            800,
            600,
            "Helvetica",
            14);
	gnu.set_grid();
	// gnu.set_xlogscale();
	gnu.set("key right top");
	// gnu.set_yrange(80, 300.0);
	gnu.set_xlabel("Number of Threads (-)");
	gnu.set_ylabel("Parallel Wall Time (ms)");
	std::vector<int> vnt = {1, 2, 4, 10, 20, 40};
	std::vector<int> vdt;
	for(auto& nt: vnt){
		std::cout << "Run by " << nt << " Threads" << std::endl;
		auto dt = SolverTimeTest(mat, nt);
		vdt.push_back(dt);
	}
	auto a = GnuplotActor::XY(vnt, vdt);
	a->command() = "using 1:2 title \"Wall time\"";
	a->style()   = "with linespoints ps 2";
	gnu.add(a);
	gnu.plot();
}

int main(int argc, char** argv) {
	std::cout << "===== Example Jacobi Solver =====" << std::endl;
	TextFile file("res.py");
	file.add_line("RES = {");
	MatSCO mato;
	Read_Matrix(file, "685_bus.mtx", mato);
	file.add_line("}");
	file.write();
	// to scr
	MatSCR mat(mato);
	SolverTest(mat);

	TimeTestPlot(mat);

	// solver time test

}