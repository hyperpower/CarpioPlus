#ifndef _EVENT_OUTPUT_TIME_HPP
#define _EVENT_OUTPUT_TIME_HPP

#include <map>
#include <utility>
#include <iostream>
#include "type_define.hpp"
#include "event.hpp"

namespace carpio{

template<St DIM, class D>
class EventOutputTime_ : public Event_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef Equation_<Dim, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

protected:
	std::ostream _stream;
public:

	/**
	 *  @brief Event constructor
	 *
	 *  default constructor is running event after each step
	 */

	EventOutputTime_(std::ostream stream,
			         int is = -1,    int ie = -1,
					 int istep = -1, int flag = 0) :
						 _stream(steam),
						 Event(is, ie, istep, flag) {
	}



};


}

#endif
