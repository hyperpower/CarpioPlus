#ifndef _S_LINEAR_CUT_HPP_
#define _S_LINEAR_CUT_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "regular.hpp"

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

    typedef CuboidTool_<Vt, DIM>   Tool;
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
    // y  *---3-->*
    // ^  ^       ^
    // | 0|       |1
    // |  *------>*
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
    void set_type(int t){
    	this->_type = t;
    }
    void set_boundary_id(int id){
       _bid = id; // boundary id only on cut interfaces
    }

    int get_boundary_id() const{
        return _bid;
    }

    void set_aperture_ratio(const std::array<Vt, NumEdge>& arr){
    	for(St i = 0; i< NumEdge; i++){
    		_ers[i] = arr[i];
    	}
    }

    Vt get_aperture_ratio(int axe, int o) const{
		if (DIM == 1) {
			return _ers;
		}
		Tool tool;
		if (DIM == 2) {
			auto fo = tool.face_order(ToAxes(axe), ToOrientation(o));
			return _ers[fo];
		}
		SHOULD_NOT_REACH;
		return 0;
    }


};

template<St DIM>
class SGhostLinearCut_: public SGhostRegular_<DIM> {
public:
	typedef SGhostRegular_<DIM>   Base;
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM>  Grid;
	typedef std::shared_ptr<Grid> spGrid;
	typedef SCellLinearCut_<DIM>    Cell;
	typedef std::shared_ptr<Cell> spCell;

	typedef typename Grid::FunIndex FunIndex;

	typedef MultiArray_<spCell, DIM> Mat;
	typedef typename Mat::reference reference;
	typedef typename Mat::const_reference const_reference;

	typedef std::function<
	             spCell(const Index&)
	             > FunSetByIndex;

	typedef std::function<
	             //input cell center coordinates (x,y,z)
	             spCell(const Vt&, const Vt&, const Vt&)
	             > FunSetByXYZ;

	typedef std::function<
	    	     spCell(const Vt&, const Vt&, const Vt&, const Vt&)
			     > FunSetByXYZT;


protected:
	Mat _mat;
public:
	SGhostLinearCut_(spGrid spg): Base(spg),
    	_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
	}
	~SGhostLinearCut_() {
	}

	spCell&
	operator()(const Index& idx) {
		return (_mat.at(idx.i(), idx.j(), idx.k()));
	}
	const spCell&
	operator()(const Index& idx) const {
		return (_mat.at(idx.i(), idx.j(), idx.k()));
	}

	bool is_ghost(const Index& index) const {
		bool bres = Base::is_ghost(index);
		if(bres == false){
			auto& pc = this->operator ()(index);
			if(pc !=nullptr ){
				return (pc->type() == _GHOST_) ? true : false;
			}else{ //pc == nullptr
				return false;
			}
		}
		return bres;
	};
	bool is_boundary(const Index& index, const St& a,
			const St& o) const {
		SHOULD_NOT_REACH;
	};
	bool is_normal(const Index& index) const {
		return (!is_ghost(index) && !is_cut(index));
	}
	;
	bool is_cut(const Index& index) const{
		bool bres = Base::is_ghost(index);
		if (bres == true) {
			return false;
		} else {
			auto& pc = this->operator ()(index);
			if (pc != nullptr) {
				return (pc->type() == _CUT_) ? true : false;
			} else { //pc == nullptr
				return false;
			}
		}
	}

	virtual int boundary_id(const Index& indexc, const Index& indexg,
			const St& axe, const St& ori) const {
	}
	;
	virtual St size_normal() const {
	}

	void set(FunSetByIndex fun) {
		FunIndex funi = [&fun, this](const Index& idx) {
			auto res = fun(idx);
			this->operator ()(idx) = res;
		};
		this->_grid->for_each(funi);
	}
};



}

#endif
