#ifndef _S_LINEAR_CUT_HPP_
#define _S_LINEAR_CUT_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/sgrid_face_data.hpp"
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
    typedef SGrid_<DIM>          Grid;
    typedef Point_<Vt, DIM>      Point;
    typedef PointChain_<Vt, DIM> PointChain;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)>     FunXYZ_Value;

//    typedef CuboidTool_<Vt, DIM>   Tool;
	typedef typename std::conditional<DIM == 2,
				                      CuboidToolPL2_<Vt>,
									  CuboidToolPL3_<Vt> >::type Tool;//


	typedef typename std::conditional<DIM == 2,
				                      Line_<Vt>,
									  Plane_<Vt> >::type Front;//
public:
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
	static const St NumEdge   = DIM == 1 ? 1 : (DIM == 2 ? 4 : 12);
protected:
    int _type;  // cell type
    int _bid;   // boundary id
    std::array<Vt, NumEdge> _ers;     // edge aperture ratios
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

    // other data
    PointChain _piecewise;
    Front      _front;   // line or plane, original point is cell center
    Point      _nc;      // normal side center
    Point      _gc;      // ghost side center

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

    const Point& nc() const{
    	return _nc;
    }
    const Point& gc() const{
    	return _gc;
    }

    void set_data(const Point& pmin,
    		      const Point& pmax,
				  const std::array<Vt, NumEdge>& arr){
    	_set_aperture_ratio(arr);
    	_set_piecewise(pmin, pmax, arr);
    	_set_front(pmin, pmax);
    	_set_two_side_center(pmin, pmax);
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

    const std::array<Vt, NumEdge>& get_aperture_ratio() const{
    	return _ers;
    }

    void show_aperture_ratio() const{
    	for(St i = 0; i< NumEdge; i++){
    		std::cout << " Edge idx = " << i << " ap = " << _ers[i] << std::endl;
    	}
    }


    void _set_aperture_ratio(const std::array<Vt, NumEdge>& arr) {
		for (St i = 0; i < NumEdge; i++) {
			if (IsCloseTo(arr[i], 0.0)) {
				_ers[i] = 0.0;
			} else if (IsCloseTo(arr[i], 1.0)) {
				_ers[i] = 1;
			} else if (IsCloseTo(arr[i], -1.0)) {
				_ers[i] = -1;
			} else {
				_ers[i] = arr[i];
			}
		}
	}

    void _set_piecewise(const Point& pmin,  // point min
		                const Point& pmax,  // point max
		                const std::array<Vt, NumEdge>& arr) {
		// make sure the aperture ratios are set;
		Tool t;
		if (DIM == 2) {
			auto pstart = t.start_point(
					pmin.x(),
					pmin.y(),
					pmax.x() - pmin.x(),
					pmax.y() - pmin.y(), arr);
			_piecewise.push_back(pstart);
			auto pend = t.start_point(
					pmin.x(),
					pmin.y(),
					pmax.x() - pmin.x(),
					pmax.y() - pmin.y(), arr);
			_piecewise.push_back(pend);
		} else {
			SHOULD_NOT_REACH;
		}
	}

    void _set_front(const Point& pmin,
    		        const Point& pmax) {
		// make sure the aperture ratios are set;
		// make sure piecewise is set;
    	Tool t;
		if (DIM == 2) {
			ASSERT(_piecewise.size() == 2);
			Point mid = Mid(pmin, pmax);
			_front = t.interface(_piecewise.front(),_piecewise.back(), mid);
		} else {
			SHOULD_NOT_REACH;
		}
	}


    void _set_two_side_center(
    		            const Point& pmin,  // point min
		                const Point& pmax  // point max
		                ){
    	// make sure the aperture ratios are set;
    	// make sure piecewise is set;
    	if (DIM == 2){
			ASSERT(_piecewise.size() == 2);
			Tool t;
			auto sx = pmax.x() - pmin.x();
			auto sy = pmax.y() - pmin.y();
			auto pc = t.cut_cell_point_chain(
					pmin.x(), pmin.y(), sx, sy, _ers);
			_gc = pc.geometry_center();
			auto pcv = t.cut_cell_point_chain_void_side(
					pmin.x(), pmin.y(), sx, sy, _ers);
			_nc = pcv.geometry_center();
    	}
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
    	_mat(spg->N(_X_), spg->N(_Y_), spg->N(_Z_)){
	}
	virtual ~SGhostLinearCut_() {
	}

	virtual std::string type() const {
		return "SGhostLinearCut";
	}

	St type(const Index& idx) const{
		auto spc = this->operator ()(idx);
		if(spc == nullptr){
			return _NORMAL_;
		}else{
			return spc->type();
		}
	}

	spCell&
	operator()(const Index& idx) {
		auto IDX = this->_grid->to_INDEX(idx);
		return (_mat.at(IDX.i(), IDX.j(), IDX.k()));
	}
	const spCell&
	operator()(const Index& idx) const {
		auto IDX = this->_grid->to_INDEX(idx);
		return _mat.at(IDX.i(), IDX.j(), IDX.k());
	}

	virtual bool is_ghost(const Index& index) const {
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
	virtual bool is_not_ghost(const Index& index) const {
		// cell can be normal or cut;
		return (!is_ghost(index));
	}

	bool is_boundary(const Index& index, const St& a,
			const St& o) const {
		SHOULD_NOT_REACH;
	}
	bool is_normal(const Index& index) const {
		return (!is_ghost(index) && !is_cut(index));
	}

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

	virtual int boundary_id(
			const Index& indexc,
			const Index& indexg,
			const St& axe,
			const St& ori) const {
		SHOULD_NOT_REACH;
	}
	;
	virtual St size_normal() const{
		return _count_normal();
	}

	St _count_normal() const{
		St count = 0;
		for(auto& spc : this->_mat){
			if(spc == nullptr){
				count++;
			}
		}
		return count;
	}

	virtual St size_not_ghost() const{
		return _count_not_ghost();
	}

	St _count_not_ghost() const {
		St count = 0;
		for (auto spc : this->_mat) {
			if (spc != nullptr) {
				if(spc->type() == _CUT_){
					count++;
				}
			}else{
				count++;
			}

		}
		return count;
	}

	void set(FunSetByIndex fun) {
		FunIndex funi = [&fun, this](const Index& idx) {
			auto res = fun(idx);
			this->operator ()(idx) = res;
		};
		this->_grid->for_each_INDEX(funi);
	}
};



}

#endif
