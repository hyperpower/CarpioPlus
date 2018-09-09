#ifndef _S_IO_FILE_HPP
#define _S_IO_FILE_HPP

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/field/sscalar.hpp"
#include "io/io_define.hpp"

namespace carpio{

template<St DIM>
class SIOFile_ {
public:
	static const St Dim = DIM;
	typedef ArrayListT_<Vt> Arr;
	typedef Point_<Vt, Dim> Poi;
	typedef SIndex_<Dim> Index;
	typedef St size_type;
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);

	typedef SScalar_<DIM> Scalar;

public:
	static void OutputScalar(const std::string& filename, const Scalar& data) {
		// Open a file
		auto& grid = data.grid();
		// format first line
		auto n = grid.n();
		TextFile txtf(filename);
		txtf.add_line(tfm::format("## SIZE : %d", data.size()));
		txtf.add_line(tfm::format("## DIM : %d", DIM));
		txtf.add_line(tfm::format("## NX : %d", n(_X_)));
		if (DIM >= 2) {
			txtf.add_line(tfm::format("## NY : %d", n(_Y_)));
		}
		if (DIM == 3) {
			txtf.add_line(tfm::format("## NZ : %d", n(_Z_)));
		}
		//
		txtf.add_line(tfm::format("## COLUMN_NAME : %s, %s, %s, %s, %s",
				                                   "ORDER", "X", "Y", "Z", "VALUE"));
		for(auto& index : data.order()){
			std::stringstream ss;
			tfm::format(ss, "%d, ", data.order().get_order(index));
			for (St d = 0; d < 3; ++d) { // always 3 d
				Vt cor = grid.c_(d, index);
				tfm::format(ss, "%10.6e, ", cor);
			}
			Vt val = data(index);
			tfm::format(ss, "%10.6e", val);
			txtf.add_line(ss.str());
		}
		txtf.write();
	}
};

}

#endif