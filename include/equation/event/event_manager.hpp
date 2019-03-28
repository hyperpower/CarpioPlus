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
	typedef std::pair<std::string, spEvent>  StringEventPair;
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
	void add(const std::string& name, spEvent spe) {
		if(this->has(name)){
			return;
		}
		if (spe->has_flag(Event::START)) {
			_start.push_back(StringEventPair(name, spe));
		}
		if (spe->has_flag(Event::END)) {
			_end.push_back(StringEventPair(name, spe));
		}
		if (spe->has_flag(Event::BEFORE)) {
			_before.push_back(StringEventPair(name, spe));
		}
		if (spe->has_flag(Event::AFTER)) {
			_after.push_back(StringEventPair(name, spe));
		}
	}
	/**
	 *  @brief EventManager run Event
	 */
	void run(St step, Vt t, int flag, pEqu pe = nullptr) {
		switch (flag) {
		case Event::START: {
			run_a_container(_start, step, t, flag, pe);
			break;
		}
		case Event::END: {
			run_a_container(_end, step, t, flag, pe);
			break;
		}
		case Event::BEFROE: {
			run_a_container(_before, step, t, flag, pe);
			break;
		}
		case Event::AFTER: {
			run_a_container(_after, step, t, flag, pe);
			break;
		}
		}
	}
	/**
	 *  @brief EventManager has Event
	 *
	 *  @param [name] name of Event
	 */
	bool has(std::string name) const{
		std::array<const Container*, 4> arrc = {&_start, &_end, &_before, &_after};
		for(auto& pc : arrc){
			for(auto& pair : (*pc)){
				if(pair.first == name){
					return true;
				}
			}
		}
		return false;
	}
	/**
	 *  @brief EventManager show Events
	 *
	 *  @param [flag] one of START END BEFORE or AFTER
	 *                default is 0, show all of the events
	 */
	void show(int flag = -1) const {
		if(flag > 0){
		switch (flag) {
		case Event::START: {
			show_a_container(_start);
			break;
		}
		case Event::END: {
			show_a_container(_end);
			break;
		}
		case Event::BEFORE: {
			show_a_container(_before);
			break;
		}
		case Event::AFTER: {
			show_a_container(_after);
			break;
		}
		}
		} else { // flag == -1 show all
			tfm::format(std::cout, "Events START\n");
			show_a_container(_start);
			tfm::format(std::cout, "  Events BEFORE\n");
			show_a_container(_before, "  ");
			tfm::format(std::cout, "  Events AFTER\n");
			show_a_container(_after, "  ");
			tfm::format(std::cout, "Events END\n");
			show_a_container(_end, "");
		}
	}
protected:
	void run_a_container(Container& con, St step, Vt t, int flag, pEqu pe = nullptr){
		for(auto& pair : con){
			auto& event = pair.second;
			if (event->do_execute(step, t, flag)) {
				event->execute(step, t, flag, pe);
			}
		}
	}

	void show_a_container(const Container& con, std::string prefix = "") const {
		int count = 0;
		for (auto& pair : con) {
			auto& name = pair.first;
			tfm::format(std::cout, "%s %3d %s\n", prefix, count, name);
			count++;
		}
	}

};


}

#endif
