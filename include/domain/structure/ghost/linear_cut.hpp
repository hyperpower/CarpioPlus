#ifndef _S_LINEAR_CUT_HPP_
#define _S_LINEAR_CUT_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "ghost.hpp"

namespace carpio{
/**
 *  @brief Contain information in a linear cut cell.
 *
 *  @tparam  DIM  Dimension of element.
 */
template<St DIM>
class SCellLinearCut_{
protected:
    typedef SCellLinearCut_<DIM> Self;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;
public:
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
	static const St NumEdge   = DIM == 1 ? 1 : (DIM == 2 ? 4 : 12);
protected:
    int _type;  // cell type
    int _bid;   // boundary id
    std::array<Vt, NumEdge> _ers; // edge aperture ratios
    // aperture ratio is number with direction
    // *=====-----*  aperture ratio =  0.5
    // *-----=====*  aperture ratio = -0.5
    // *----------*  aperture ratio =  0.0
    // *==========*  aperture ratio =  1.0 or -1.0
    // |---------> positive direction

    // 2 dimensions
    // *------>*
    // ^       ^
    // |       |
    // |       |
    // *------>*
    // Arrows indicates directions

    // Number is edge order
    // For 2D edge order is face order
    // y   ___3___
    // ^  |       |
    // | 0|       |1
    // |  |_______|
    // |      2
    // O-----> x

public:
	SCellLinearCut_() :_type(_CUT_) {
		_bid = 10;
	}

    SCellLinearCut_(const int& id): _type(_CUT_){
		_bid = id;
    }

    SCellLinearCut_(const Self& self):
        _type(self._type), _bid(self._bid){
    }

    Self& operator=(const Self& o) {
        if (this == &o) {
            return *this;
        }
        _type = o._type;
        _bid = o._bid;
        _ers = o._ers;
        return *this;
    }

    const int& type() const{
        return this->_type;
    }
    int& type(){
        return this->_type;
    }

    void set_boundary_id(int id){
       _bid = id; // boundary id only on cut interfaces
    }

    int get_boundary_id() {
        return _bid;
    }

    Vt get_aperture_ratio(int axe, int o){
		if (DIM == 1) {
			return _ers;
		}
		SHOULD_NOT_REACH;
		return 0;
    }
};

template<St DIM>
class SGhostLinearCut_: public SGhost_<DIM> {
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM> Grid;


	SGhostLinearCut_() {

	}
	virtual ~SGhostLinearCut_() {

	}

	virtual Grid& grid() {
	}

	virtual const Grid& grid() const {
	}

	virtual bool is_ghost(const Index& index) const {
	}
	;
	virtual bool is_boundary(const Index& index, const St& a,
			const St& o) const {
	}
	;
	virtual bool is_normal(const Index& index) const {
	}
	;

	virtual int boundary_id(const Index& indexc, const Index& indexg,
			const St& axe, const St& ori) const {
	}
	;
	virtual St size_normal() const {
	}
};



}
