#ifndef _EVENTMANAGER_HPP
#define _EVENTMANAGER_HPP

#include <map>
#include <utility>
#include <iostream>
#include <memory>
#include <vector>
#include "type_define.hpp"
#include "event.hpp"

namespace carpio{

template<St DIM, class D>
class EventManager_ {
public:
	static const St Dim         = DIM;
	static const St NumFaces    = DIM + DIM;
	static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);

	typedef Equation_<Dim, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef Event_<DIM, D> Event;
	typedef std::shared_ptr<Event_<DIM, D> > spEvent;

	typedef std::vector<std::pair<std::string, spEvent> > Container;

protected:
	Container _start;
	Container _end;
	Container _before;
	Container _after;

public:
	EventManager_(){
	}
	/**
	 *  @brief EventManager add Event
	 */
	void add(const std::string& name, spEvent spe){

	}
	/**
	 *  @brief EventManager run Event
	 */
	void run(St step, Vt t, int flag, pEqu pe= nullptr){

	}
	/**
	 *  @brief EventManager has Event
	 *
	 *  @param [name] name of Event
	 */
	bool has(std::string name) const{

	}
	/**
	 *  @brief EventManager show Events
	 *
	 *  @param [flag] one of START END BEFORE or AFTER
	 *                default is 0, show all of the events
	 */
	void show(int flag = 0) const{

	}


};


}

#endif
