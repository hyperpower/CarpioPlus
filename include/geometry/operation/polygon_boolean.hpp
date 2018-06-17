#ifndef _POLYGON_BOOLEAN_HPP_
#define _POLYGON_BOOLEAN_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/point_chain.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {

template<class TYPE>
class PolygonBoolean_{

public:
	typedef TYPE Vt;
	typedef Point_<TYPE, 2> Point;
	typedef PointChain_<TYPE, 2> PointChain;

protected:
	static const int _PN_ = 0; // new point
	static const int _PC_ = 1; // point from clip
	static const int _PO_ = 2; // point from object
	static const int _PCO_= 3; // point from clip and object


	struct Node{
		int type;
		Point poi;

		typedef Node* pNode;

		pNode prevc; // previous clip
		pNode nextc; // next clip
		pNode prevo; // previous object
		pNode nexto; // previous object
	};

	typedef Node* pNode;

	pNode cli;
	pNode obj;

	PolygonBoolean_(
			const PointChain& clip,
			const PointChain& obj){

	}

	void phase_1(){
		// copy point chain to node list
	}

protected:

};


}

#endif
