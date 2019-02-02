#ifndef _STOP_HPP
#define _STOP_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"

namespace carpio{

template<St DIM, class D> class Equation_;

template<St DIM, class D>
class Stop_ {
public:
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;
public:
	Stop_(){
	}
	virtual bool is_satisfied() const{
		std::cout << "Stop is_satisfied\n";
		return true;
	}

	virtual ~Stop_(){
	}
};



}

#endif
