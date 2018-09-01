#ifndef _EVENT_HPP
#define _EVENT_HPP

#include <map>
#include <utility>
#include <iostream>
#include "type_define.hpp"

namespace carpio{

template<St DIM, class D> class Equation_;

template<St DIM, class D>
class Event_ {
public:
	static const St Dim         = DIM;
	static const St NumFaces    = DIM + DIM;
	static const St NumVertexes = (DIM == 3) ? 8 : (DIM + DIM);

	typedef Equation_<Dim, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	static const int START  = 0x10;
	static const int END    = 0x20;
	static const int BEFORE = 0x100;
	static const int AFTER  = 0x200;

protected:
	/*
	 *  _flag = 0      disabled flag
	 *  _flag = -1     forward
	 *  _flag = 1      backward      (default)
	 *  _flag = 2      both run, forward and backward
	 *  _flag = -2     both do not run, forward and backward
	 */
	int _flag;
	int _istart, _iend, _istep;

	bool _has_flag(int f) const {
		return (_flag | f) == _flag ? true : false;
	}

public:
	/**
	 *  @brief Event constructor
	 *
	 *  default constructor is running event after each step
	 */

	Event_(int is = -1, int ie = -1, int istep = -1, int flag = 0) {
		_flag   = flag;
		_istart = is;
		_iend   = ie;
		_istep  = istep;
	}

	virtual int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		std::cout << "Event : execute \n";
		return -1;
	}

	virtual int flag() const {
		return 0;
	}

	virtual void set_flag(int i) {
		return;
	}

	virtual ~Event_() {
	}

	/**
	 *  @brief Dose the event need to be executed
	 *
	 *  @param [step] is step right now
	 *  @param [t]    is time right now
	 *  @param [fob]  forward or backward,
	 *                fob = -1 is before the step,
	 *                fob = 1 is after the step
	 */
	bool do_execute(St step, Vt t, int fob) const {
		if (this->_has_flag(fob)) {
			bool res = ((step - this->_istart) % this->_istep == 0);
			if (this->_iend != -1) {
				res = res && int(step) <= this->_iend;
			}
			if (this->_istart != -1) {
				res = res && int(step) >= this->_istart;
			}
			return res;
		} else {
			return false;
		}
	}

};



}

#endif
