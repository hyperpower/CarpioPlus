#ifndef _CREATE_TS_HPP_
#define _CREATE_TS_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/ts/trisurface.hpp"
#include "geometry/objects/ts/triface.hpp"
#include "geometry/objects/ts/vertex.hpp"
#include "geometry/objects/ts/edge.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {

template<typename TYPE, St DIM>
class CreateTS_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;
	typedef Segment_<TYPE, DIM> Segment;
	typedef Segment& ref_Segment;
	typedef const Segment& const_ref_Segment;

	typedef TriSurface_<TYPE, DIM>          TriSurface;
	typedef TriFace_<TYPE, DIM, TriSurface> TriFace;
	typedef Edge_<TYPE, DIM, TriFace>       Edge;
	typedef Vertex_<TYPE, DIM, Edge>        Vertex;
	typedef Vertex*                        pVertex;
	typedef Edge*                          pEdge;
	typedef TriFace*                       pTriFace;

	typedef TriSurface_<TYPE, 2> TriSurface2;
	typedef TriSurface_<TYPE, 3> TriSurface3;

public:
	static void TriFaceOne(
			    TriSurface& sur,
				const Point& a,
				const Point& b,
				const Point& c) {
			pVertex pv1 = new Vertex(a);
			pVertex pv2 = new Vertex(b);
			pVertex pv3 = new Vertex(c);

			pEdge pe1 = new Edge(pv1, pv2);
			pEdge pe2 = new Edge(pv2, pv3);
			pEdge pe3 = new Edge(pv3, pv1);

			sur.clear();
			pTriFace pfac = new TriFace(pe1, pe2, pe3, &sur);
			sur.insert(pfac);
		}



};




}

#endif
