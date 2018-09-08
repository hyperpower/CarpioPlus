#ifndef _EVENT_OUTPUT_TIME_HPP
#define _EVENT_OUTPUT_TIME_HPP

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
class EventOutputTime_ : public Event_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

protected:
	std::ostream* _stream;

	std::list<Vt>     _l_t;     //time
	std::list<St>     _l_step;  //step
	std::list<tick_t> _l_cpu;   //cpu time
	std::list<tick_t> _l_wall;  //wall time
public:

	/**
	 *  @brief Event constructor
	 *
	 *  default constructor is running event after each step
	 */
	EventOutputTime_(std::ostream& stream,
			         int is = -1,    int ie = -1,
					 int istep = -1, int flag = 0) :
						 _stream(&stream),
						 Event(is, ie, istep, flag) {
	}

	int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		_record_time(step, t);
		_output(step, t);
		return -1;
	}
protected:
	tick_t _d_last(const std::list<tick_t>& list) const {
		tick_t res = 0;
		if (list.size() < 2) {
			return res;
		}
		auto last = list.rbegin();
		tick_t cpu_l = (*last);
		last++;
		tick_t cpu_ll = (*last);
		return (cpu_l - cpu_ll);
	}

	void _output(const St& step, const Vt& t) {
		Vt d_cpu  = Clock::TicksToSecondsD(_d_last(_l_cpu));
		Vt d_wall = Clock::TicksToSecondsD(_d_last(_l_wall));

		tfm::format(*_stream,
					"step: %8d t: %8.5f cpu: %8.5f wall: %8.5f\n",
					step, t, d_cpu, d_wall);
	}

	void _record_time(const St& step, Vt& t) {
		_l_t.push_back(step);
		_l_t.push_back(t);
		_l_cpu.push_back(Clock::SystemTime());
		_l_wall.push_back(Clock::Tick());
	}

	std::string name() const {
		return "OutputTime";
	}



};


}

#endif
