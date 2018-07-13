#ifndef _S_ORDER_HPP_
#define _S_ORDER_HPP_


#include "domain/structure/structure_define.hpp"

namespace carpio{

template<St DIM>
class SOrder_{
public:
	typedef SIndex_<DIM> Index;

	typedef std::vector<Index> OrderVector;
	typedef typename OrderVector::iterator iterator;
	typedef typename OrderVector::const_iterator const_iterator;

	SOrder_(){

	}

	virtual ~SOrder_(){

	}

	virtual iterator begin(){

	}

	virtual iterator end(){

	}

	virtual const_iterator begin() const{

	}

	virtual const_iterator end() const{

	}

	virtual St get_order(const Index& o) const{

	}

};


}



#endif
