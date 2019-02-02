#ifndef _STOP_MANAGER_HPP
#define _STOP_MANAGER_HPP

#include <memory>
#include <iostream>
#include <vector>
#include "type_define.hpp"
#include "stop.hpp"

namespace carpio{

template<St DIM, class D> class Equation_;

template<St DIM, class D>
class StopManager_ {
public:

	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef Stop_<DIM, D> Stop;
	typedef std::shared_ptr<Stop> spStop;

	typedef std::vector<std::pair<std::string, spStop> > Container;
protected:
	Container _con;
public:
	StopManager_(){
	}

	~StopManager_(){

	}
};


}

#endif
