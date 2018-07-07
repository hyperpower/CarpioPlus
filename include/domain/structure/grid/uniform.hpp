#ifndef _S_SGRIDUNIFORM_HPP
#define _S_SGRIDUNIFORM_HPP

#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "grid.hpp"
#include <array>

namespace carpio{

template<St DIM>
class SGridUniform_ :public SGrid_<DIM>{
public:
	static const St Dim = DIM;
	typedef ArrayListV<double> Arr;
	typedef Point_<double, Dim> Poi;
	typedef SIndex_<Dim> Index;
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
protected:
	Poi   _min;
	St    _n, _ng;    // number of node and number of node and ghost
	St    _gl;        // ghost layer
	Vt    _cs;        // cell size
public:




};



}

#endif
