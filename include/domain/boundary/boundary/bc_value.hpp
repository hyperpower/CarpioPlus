#ifndef _S_BOUNDARY_CONDITION_VALUE_HPP_
#define _S_BOUNDARY_CONDITION_VALUE_HPP_

#include "domain/structure/structure_define.hpp"
#include "boundary_condition.hpp"

namespace carpio{

class BoundaryConditionValue :public BoundaryCondition {
public:
	typedef BoundaryConditionValue Self;
protected:
	// data
	// 1 Bondary conditon type
	int _type;
	Vt  _value;
public:
	// Constructor
	BoundaryConditionValue() {
		// default boundary condition is symmetric boundary condition
		_type = BoundaryCondition::_BC2_;
		_value = 0;
	}
	/*
	 * this constructor should not used to BC2
	 */
	BoundaryConditionValue(int type, Vt v) :
			_type(type), _value(v) {
	}

	// get
	int type() const {
		return _type;
	}
	Vt value(Vt x) const {
		return _value;
	}
	Vt value(Vt x, Vt y) const {
		return _value;
	}
	Vt value(Vt x, Vt y, Vt z) const {
		return _value;
	}
	Vt value(Vt x, Vt y, Vt z, Vt t) const {
		return _value;
	}

};




}

#endif
