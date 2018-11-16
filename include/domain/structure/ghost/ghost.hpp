#ifndef _S_GHOST_HPP_
#define _S_GHOST_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"

namespace carpio{

template<St DIM>
class SGhost_ {
public:
	typedef	SIndex_<DIM> Index;

	SGhost_(){

	}
	virtual ~SGhost_(){

	}

	virtual St num_layer() const{};

	virtual bool is_ghost(const Index& index) const{};
	virtual bool is_boundary(
			const Index& index,
			const St& a,
			const St&) const{};
	virtual bool is_normal(const Index& index) const{};


	virtual int boundary_id(const Index& indexc,
			                const Index& indexg,
							const St&  axe,
							const St& ori) const{};
	virtual St size_normal() const{
	}
};


}



#endif
