#ifndef _EVENT_FIELD_NORM_HPP
#define _EVENT_FIELD_NORM_HPP

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
class EventNorm1Previous_ : public Event_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef typename D::IOFile IOFile;
	typedef typename D::Field  Field;

	typedef typename D::spGrid  spGrid;
	typedef typename D::spGhost spGhost;
	typedef typename D::spOrder spOrder;

	typedef std::list<std::array<Vt, 3> > List;
protected:
	std::string _fn;     //field name
	Field _fp;           //field previous
	int _stepp;        //step previous
	int   _count;        //count
	List _ln1;  //list of norm 1
public:
	EventNorm1Previous_(
			const std::string& fname,
			spGrid spg, spGhost spgh, spOrder spor,
			int is    = -1, int ie   = -1,
		    int istep = -1, int flag = Event::AFTER) :
		    	_fn(fname),
				_count(0),
				_stepp(is),
				_fp(spg, spgh, spor),
				Event(is, ie, istep, flag) {
	}
	~EventNorm1Previous_(){
	}

	virtual int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		if (pd->has_field(_fn)) {
			auto& fc = (*pd)[_fn];
			if(_count == 0){
				_fp    = fc; //store previous field
				_stepp = step;
				_count++;
				return _SUCCESS;
			}else{
				// norm1
				auto e = fc - _fp;
				_ln1.push_back({Vt(_stepp), Vt(step), e.norm1()});
				_fp    = fc;
				_stepp = step;
				_count++;
			}
		} else {
			std::cerr << "EventOutputField : " << _fn << " not found!"
					<< std::endl;
		}
		return -1;
	}

	List& get_list(){
		return _ln1;
	}
	const List& get_list() const{
		return _ln1;
	}

	void show() const{
		tfm::format(std::cout, "%13s  %8s  %10s\n",
				"Previous Step", "Step", "Norm1");
		for(auto& e: _ln1){
			tfm::format(std::cout, "%10d  %8d  %10.4e\n", e[0], e[1], e[2]);
		}
	}
};

template<St DIM, class D>
class EventStopNorm1Previous_ : public EventNorm1Previous_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef EventNorm1Previous_<DIM, D> Base;
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef typename D::IOFile IOFile;
	typedef typename D::Field  Field;

	typedef typename D::spGrid  spGrid;
	typedef typename D::spGhost spGhost;
	typedef typename D::spOrder spOrder;

	typedef std::list<std::array<Vt, 3> > List;
protected:
	Vt _cv; //critical value
public:
	EventStopNorm1Previous_(
			const Vt& cv,
			const std::string& fname,
			spGrid spg, spGhost spgh, spOrder spor,
			int is    = -1, int ie   = -1,
			int istep = -1, int flag = Event::AFTER) :
			    _cv(cv),
			    Base(fname, spg, spgh, spor, is, ie, istep, flag){
		}

	int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		if (pd->has_field(this->_fn)) {
			auto& fc = (*pd)[this->_fn];
			if (this->_count == 0) {
				this->_fp = fc; //store previous field
				this->_stepp = step;
				this->_count++;
				return _SUCCESS;
			} else {
				// norm1
				auto e = fc - this->_fp;
				Vt  n1 = e.norm1();
				this->_ln1.push_back( { Vt(this->_stepp), Vt(step), n1 });
				if(n1 < _cv){
					std::string reason =
							tfm::format("Step %d compares with previous step %d, Norm1 = %.3e < %.3e",
								step, this->_stepp, n1, _cv);
					pd->trigger_stop(reason, step, t);
				}
				this->_fp = fc;
				this->_stepp = step;
				this->_count++;
			}
		} else {
			std::cerr << "EventOutputField : " << this->_fn << " not found!"
					<< std::endl;
		}
		return -1;
	}

};

}

#endif
