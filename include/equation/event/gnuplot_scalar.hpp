#ifndef _EVENT_GNUPLOT_SCALAR_HPP
#define _EVENT_GNUPLOT_SCALAR_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"
#include "event.hpp"
#include "utility/clock.hpp"
#include "utility/tinyformat.hpp"
#include "io/gnuplot.hpp"

namespace carpio{

template<St DIM, class D>
class EventGnuplotScalar_ : public Event_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef typename D::GnuplotActor GnuplotActor;
	typedef typename D::Scalar       Scalar;

protected:
	std::string _sn;     // scalar name
	std::string _format; // format string
	std::string _path;   // path for output file

	std::shared_ptr<Gnuplot> _spgnu;
	std::string _terminal_name;
	Vt          _size_x;
	Vt          _size_y;
	std::string _font;
	St          _size_font;

public:
	EventGnuplotScalar_(const std::string& sname,
			int is    = -1, int ie   = -1,
	        int istep = -1, int flag = 0) :
		 _sn(sname),
		 Event(is, ie, istep, flag) {
		_format = "%s_%d_%8.4e";
		_path   = "./";

		_spgnu         = std::shared_ptr<Gnuplot>(new Gnuplot());
		_terminal_name = "png";
		_size_x        = 800;
		_size_y        = 600;
		_font          = "Helvetica";
		_size_font     = 12;
	}

	int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		auto fn = _file_name(step, t, fob);
		if(pd->has_scalar(_sn)){
			_set_terminal(_terminal_name, fn);
			_plot((*pd)[_sn]);
		}else{
			std::cerr<< "EventGnuplotScalar : " << _sn << " not found!" << std::endl;
		}
		return -1;
	}

	void set_format_string(const std::string& format){
		_format = format;
	}

	void set_path(const std::string& path){
		_path = path;
	}

	Gnuplot& gnuplot(){
		return *(_spgnu);
	}

protected:
	void _set_terminal(
			const std::string& tn, // terminal name
			const std::string& fn  // file name
			){
		if(tn == "png"){
			_spgnu->set_terminal_png(fn, _size_x, _size_y, _font, _size_font);
			return;
		}
		std::cerr << "EventGnuplotScalar: " << "Wrong terminal!" << std::endl;
	}

	void _plot(const Scalar& s){
		if(DIM == 1){
			_spgnu->add(GnuplotActor::Lines(s));
		}
		_spgnu->plot();
		_spgnu->clear();
	}

	std::string _file_name(St step, Vt t, int fob) const{
		std::stringstream ss;
		ss << _path;
		tfm::format(ss, _format.c_str() , _sn, step, t);
		return ss.str();
	}
};


}
#endif
