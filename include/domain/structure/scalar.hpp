#ifndef _S_SCALAR_HPP_
#define _S_SCALAR_HPP_

namespace carpio{

template<St DIM>
class SScalar_{
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM>  Grid;
	typedef SGhost_<DIM> Ghost;
	typedef SOrder_<DIM> Order;

	typedef std::shared_ptr<SIndex_<DIM> > spIndex;
	typedef std::shared_ptr<SGrid_<DIM>  > spGrid;
	typedef std::shared_ptr<SGhost_<DIM> > spGhost;
	typedef std::shared_ptr<SOrder_<DIM> > spOrder;

	typedef Multi_Array<Vt, DIM> Mat;

	typedef SScalar_<DIM> Self;
protected:
	spGrid _grid;
	spGhost _ghost;
	spOrder _order;

	Mat _mat;

public:
	SScalar_(spGrid spg, spGhost spgh): 
	    _grid(spg), _ghost(spgh){
	    // Initall a default order_xyz

	}

	SScalar_(const Self& other):
	    _grid(other._grid), 
	    _ghost(other._ghost), 
	    _order(other._order), 
	    _mat(other._mat){
	}

	Self& operator+=(const Self& rhs){
    	// actual addition of rhs to *this
    	return *this;
    }

    Self& operator+=(const Vt& rhs){
    	// actual addition of rhs to *this
    	return *this;
    }

};
template<St DIM>
inline SScalar_<DIM> operator+(SScalar_<DIM> lhs, const SScalar_<DIM>& rhs)
{
  lhs += rhs;
  return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator+(SScalar_<DIM> lhs, const Vt& rhs)
{
  lhs += rhs;
  return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator+(const Vt& rhs, SScalar_<DIM> lhs)
{
  lhs += rhs;
  return lhs;
}


}


#endif