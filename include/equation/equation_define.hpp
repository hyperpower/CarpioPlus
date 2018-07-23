#ifndef _EQUATION_DEFINE_HPP_
#define _EQUATION_DEFINE_HPP_

#include "type_define.hpp"
#include "time_term.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "utility/any.hpp"
#include <memory>

namespace carpio {

template<St DIM> class Event_;
template<St DIM> class TimeTerm_;

template<St DIM, class DOMAIN>
class Equation_{
public:
	typedef DOMAIN Domain;
	typedef typename Domain::SizeType  St;
	typedef typename Domain::ValueType Vt;
	typedef typename Domain::Grid      Grid;
	typedef typename Domain::Ghost     Ghost;
	typedef typename Domain::Order     Order;
	typedef typename Domain::Scalar    Scalar;
	typedef Event_<DIM>                Event;
	typedef TimeTerm_<DIM>             TimeTerm;

	typedef typename Domain::VectorCenter VectorCenter;
	typedef typename Domain::VectorFace   VectorFace;

	typedef std::shared_ptr<Event>  spEvent;
	typedef std::shared_ptr<Scalar> spScalar;
	typedef std::shared_ptr<Grid>   spGrid;
	typedef std::shared_ptr<Ghost>  spGhost;
	typedef std::shared_ptr<Order>  spOrder;
	typedef std::shared_ptr<VectorCenter>  spVectorCenter;
	typedef std::shared_ptr<VectorFace>    spVectorFace;
	typedef std::shared_ptr<TimeTerm>      spTimeTerm;
	typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> Function;

	typedef std::map<std::string, Any>                AFlags;
	typedef std::map<std::string, spScalar>           Scalars;
	typedef std::map<std::string, spBoundaryIndex>    BIs;
	typedef std::unordered_map<std::string, spEvent>  Events;
	typedef std::unordered_map<std::string, Function> Functions;
	typedef std::unordered_map<std::string, Vt>       Values;


protected:
	spGrid  _grid;
	spGhost _ghost;

	// time relates variables
	spTimeTerm _time;
	Events     _events;        // _events
	Functions  _functions;     // independent of grid
	Values     _values;        // values for equation
	AFlags     _aflags;        // other types of data put in this map

	Scalars    _scalars;       //!< variables on the center of node
	BIs        _bis;           //!< each variable has a Boundary Index

public:
	Equation_(spGrid pf, spGhost pg = nullptr) :
				_grid(pf), _ghost(pg) {
			this->_time = nullptr;
		}

	virtual ~Equation_() {
	}

	virtual int run_one_step(St step) {
			std::cout << step << "  Equation: run one step \n";
			return -1;
		}

	virtual int initial() {
		std::cout << "  Equation: initial \n";
		SHOULD_NOT_REACH;
		return -1;
	}

	virtual int finalize() {
		std::cout << "  Equation: finalize \n";
		return -1;
	}

	virtual int solve() {
		std::cout << "  Equation: solve \n";
		return -1;
	}

	void run() {
		// the equation don't have time
		if (!this->has_event("time_term")) {
			initial();
			// start events
			run_events(0, 0.0, Event::START);

			solve();

			run_events(1, 0.0, Event::END);
			finalize();
		} else {
			// events before calculation
			initial();
			run_events(this->_time->current_step(), //
					this->_time->current_step(),    //
					Event::START);
			// loop
			while (!this->_time->is_end() && (!this->has_event("_STOP_"))) {
				//
				// events before each step
				run_events(this->_time->current_step(),  //
						this->_time->current_time(),     //
						Event::BEFORE);

				// run one step =================
				run_one_step(this->_time->current_step());
				// ==============================

				// events after each step
				run_events(this->_time->current_step(),  //
						this->_time->current_time(),     //
						Event::AFTER);
				//
				this->_time->advance();
			}
			// events after calculation
			run_events(this->_time->current_step(),    //
					this->_time->current_time(),       //
					Event::END);
			finalize();
		}
	}

	void run_events(St step, Vt t, int fob) {
		for (auto& event : this->_events) {
			if (event.second->_do_execute(step, t, fob)) {
				event.second->execute(step, t, fob, this);
			}
		}
	}

	virtual Ghost& ghost() {
		return *(this->_ghost);
	}

	virtual Grid& grid() {
		return *(this->_grid);
	}

	virtual const Ghost& ghost() const{
		return *(this->_ghost);
	}

	virtual const Grid& grid() const{
		return *(this->_grid);
	}

	/**
	 * @brief this function check the events
	 *        if flags contain key and value == val return true
	 *        the default val == 1
	 */
	bool has_event(const std::string& key) const {
		auto it = this->_events.find(key);
		if (it != this->_events.end()) {
			return true;
		}
		return false;
	}

	bool has_function(const std::string& key) const {
		auto it = this->_functions.find(key);
		if (it != this->_functions.end()) {
			return true;
		}
		return false;
	}

	bool has_value(const std::string& key) const {
		auto it = this->_values.find(key);
		if (it != this->_values.end()) {
			return true;
		}
		return false;
	}

	bool has_flag(const std::string& key) const {
		auto it = this->_aflags.find(key);
		if (it != this->_aflags.end()) {
			return true;
		}
		return false;
	}

	void set_function(const std::string& name, Function fun) {
		this->_functions[name] = fun;
	}

	void show_events() const {
		std::cout << "Events:  (" << this->_events.size() << ")\n";
		for (auto iter = this->_events.begin(); iter != this->_events.end();
				++iter) {
			std::cout << "  " << iter->first << std::endl;
		}
	}

	void show_functions() const {
		std::cout << "Functions:  (" << this->_functions.size() << ")\n";
		for (auto iter = this->_functions.begin();
				iter != this->_functions.end(); ++iter) {
			std::cout << "  " << iter->first << std::endl;
		}
	}

	void show_values() const {
		std::cout << "Values:  (" << this->_values.size() << ")\n";
		for (auto iter = this->_values.begin(); iter != this->_values.end();
				++iter) {
			std::cout << "  " << std::setw(15) << iter->first << " : "
					<< iter->second << std::endl;
		}
	}

};



}

#endif
