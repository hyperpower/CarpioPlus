#ifndef _EVENT_SET_SCALAR_HPP
#define _EVENT_SET_SCALAR_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"
#include "event.hpp"
#include "utility/clock.hpp"
#include "utility/tinyformat.hpp"

namespace carpio{

template<St DIM, class D>
class EventSetScalar_ : public Event_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef typename D::FunXYZT_Value Fun;

protected:
	std::string _sn;    // scalar name
	Fun _fun;           // function

public:
	EventSetScalar_(const std::string& sname,
			int is    = -1, int ie   = -1,
	        int istep = -1, int flag = 0) :
		 _sn(sname),
		 Event(is, ie, istep, flag) {
		// default function
		_fun = [](Vt, Vt, Vt, Vt){return 0.0;};
	}

	EventSetScalar_(
			    const std::string& sname,
				Fun fun,
				int is    = -1, int ie   = -1,
		        int istep = -1, int flag = 0) :
		 _sn(sname),
		 _fun(fun),
		 Event(is, ie, istep, flag) {
	}

	int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		Equ& equ = *(pd);
		auto fn = _file_name(step, t, fob);
		if(pd->has_scalar(_sn)){
			equ[_sn].assign(_fun, t);
		}else{
			std::cerr<< "EventSetScalar : " << _sn << " not found!" << std::endl;
		}
		return -1;
	}
};


}
#endif
