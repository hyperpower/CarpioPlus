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
    typedef SGrid_<DIM> Grid;
    typedef std::shared_ptr<Grid> spGrid;
    typedef SGhost_<DIM> Ghost;
    typedef std::shared_ptr<Ghost> spGhost;

    SOrder_(){}

    virtual ~SOrder_(){}

    virtual St size() const{SHOULD_NOT_REACH;}

    virtual iterator begin(){}

    virtual iterator end(){}

    virtual const_iterator begin() const{}

    virtual const_iterator end() const{}

    virtual St get_order(const Index& o) const{}

    virtual const Grid& grid() const {
    }
    virtual const Ghost& ghost() const {
    }
    virtual spGrid spgrid() const{
    }
    virtual spGhost spghost() const{
    }

    // ----------------------------------------------
#ifdef OPENMP
    virtual St num_threads() const{}
    virtual iterator begin(St) {}
    virtual const_iterator begin(St) const {}
    virtual iterator end(St) {}
    virtual const_iterator end(St) const {}
#endif

};



}



#endif
