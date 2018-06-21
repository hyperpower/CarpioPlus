#ifndef _POLYGON_BOOLEAN_HPP_
#define _POLYGON_BOOLEAN_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/point_chain.hpp"
#include "algebra/array/array_list.hpp"

#include "io/gnuplot.hpp"

#include <memory>
#include <functional>

namespace carpio {

template<class TYPE>
class PolygonBoolean_{

public:
	typedef TYPE Vt;
	typedef Point_<TYPE, 2> Point;
	typedef PointChain_<TYPE, 2> PointChain;
	typedef PolygonBoolean_<TYPE> PB;

	PolygonBoolean_(
			const PointChain& clip,
			const PointChain& object) : cli(nullptr), obj(nullptr){
		phase_1(clip, object);
	}

protected:
	static const int _PN_ = 0; // new point
	static const int _PC_ = 1; // point from clip
	static const int _PO_ = 2; // point from object
	static const int _PCO_= 3; // point from clip and object


	struct Node{
		int type;
		Point point;

		typedef Node* pNode;

		pNode prevc; // previous clip
		pNode nextc; // next clip
		pNode prevo; // previous object
		pNode nexto; // previous object

		Node(const Point& p, const int& t):point(p){
			type = t;

			prevc = nullptr;
			nextc = nullptr;
			prevo = nullptr;
			nexto = nullptr;
		}
	};

	typedef Node* pNode;

	pNode cli;
	pNode obj;


	void phase_1(const PointChain& clip,
			     const PointChain& object){
		// copy point chain to node
		ASSERT(clip.size()   >=3);
		ASSERT(object.size() >=3);
		_build_clip_link(clip);
		_build_object_link(object);
	}

	typedef std::function<void(pNode, pNode)> FunNN;

	void for_each_edge_clip(FunNN fun){
		pNode first  = cli;
		pNode second;
		while(first->nextc !=nullptr){
			second = first->nextc;
			fun(first, second);
			first  = second;
		}
		fun(first, cli);
	}

	void for_each_edge_object(FunNN fun) {
		pNode first = obj;
		pNode second;
		while (first->nexto != nullptr) {
			second = first->nexto;
			fun(first, second);
			first = second;
		}
		fun(first, obj);
	}

protected:
	void _build_clip_link(const PointChain& pc){
		pNode last = cli;
		for(auto& p : pc){
			pNode pn = new Node(p, 1);
			if(cli == nullptr){
				cli  = pn;
				last = pn;
			}else{
				last->nextc = pn;
				pn->prevc   = last;
				last        = pn;
			}
		}
	}


	void _build_object_link(const PointChain& po) {
		pNode last = obj;
		for (auto& p : po) {
			pNode pn = new Node(p, 2);
			if (obj == nullptr) {
				obj = pn;
				last = pn;
			} else {
				last->nexto = pn;
				pn->prevo = last;
				last = pn;
			}
		}
	}


	void insert_c(pNode& p, pNode& pnew){
		pnew->nextc = p->nextc;
		pnew->prevc = p;
		p->nextc    = pnew;
	}

	void insert_o(pNode& p, pNode& pnew){
		pnew->nexto = p->nexto;
		pnew->prevo = p;
		p->nexto = pnew;
	}



public:
	GnuplotActor::spActor actor_clip(Gnuplot& gnu) {
		GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3:4:5 title \"\" ";
		actor->style() = "with vectors lc variable";

		int count = 1;
		FunNN fun = [&actor, &count, &gnu](pNode f, pNode s){
			Vt len = Distance(f->point, s->point);
			Vt dx  = s->point.x() - f->point.x();
			Vt dy  = s->point.y() - f->point.y();
			actor->data().push_back(
					ToString(f->point.x(),
							 f->point.y(),
							 dx,
							 dy, 1, " "));
			gnu.set_label(count,
						  "C ("+ ToString(f->point.x(),f->point.y(), ", ") + ")",
						  f->point.x(),f->point.y(), "center");
			count++;
		};

		for_each_edge_clip(fun);

		return actor;
	}

	GnuplotActor::spActor actor_object(Gnuplot& gnu) {
		GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3:4:5 title \"\" ";
		actor->style() = "with vectors lc variable";

		int count = 10000;
		FunNN fun = [&actor, &count, &gnu](pNode f, pNode s) {
			Vt len = Distance(f->point, s->point);
			Vt dx = s->point.x() - f->point.x();
			Vt dy = s->point.y() - f->point.y();
			actor->data().push_back(
					ToString(f->point.x(),
							f->point.y(),
							dx,
							dy, 2, " "));
			gnu.set_label(count,
					"O ("+ ToString(f->point.x(),f->point.y(), ", ") + ")",
					f->point.x(),f->point.y(), "center");
			count++;
		};

		for_each_edge_object(fun);

		return actor;
	}


};


}

#endif
