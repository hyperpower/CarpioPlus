#ifndef _POLYGON_BOOLEAN_HPP_
#define _POLYGON_BOOLEAN_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/basic/point_chain.hpp"
#include "geometry/operation/intersection_ss.hpp"
#include "algebra/array/array_list.hpp"

#include "io/gnuplot.hpp"

#include <memory>
#include <set>
#include <functional>
#include <iomanip>

namespace carpio {

template<class TYPE>
class PolygonBoolean_{

public:
	static const int _PN_ = 0;  // new point
	static const int _PC_ = 1;  // point from clip
	static const int _PO_ = 2;  // point from object
	static const int _PCO_ = 3; // point from clip and object

	typedef TYPE Vt;
	typedef Point_<TYPE, 2> Point;
	typedef PointChain_<TYPE, 2> PointChain;
	typedef PolygonBoolean_<TYPE> PB;

	PolygonBoolean_(
			const PointChain& clip,
			const PointChain& object) : cli(nullptr), obj(nullptr){
		phase_1(clip, object);
		phase_2();
		phase_3();
	}


protected:

	struct Node{
		int type;
		Point point;

		typedef Node* pNode;

		pNode prevc; // previous clip
		pNode nextc; // next clip
		pNode prevo; // previous object
		pNode nexto; // previous object

		Node(const Point& p, int t):point(p){
			type = t;

			prevc = nullptr;
			nextc = nullptr;
			prevo = nullptr;
			nexto = nullptr;
		}
	};

	typedef Node* pNode;

	typedef std::function<void(pNode)> FunN;
	typedef std::function<void(pNode, pNode)> FunNN;
	typedef std::function<void(pNode, pNode, pNode, pNode)> FunNNNN;



	struct IntersectionGroup {
		pNode cs;     // clip start
		pNode ce;     // clip end
		pNode os;     // object start
		pNode oe;     // object end
		pNode inter;  // intersection
		int   type;   // type -1 error
		              //       0 two  start point connect
		              //       1 clip start point touch
		              //       2 object start point touch
		              //       3 normal intersection

		IntersectionGroup():
			cs(nullptr), ce(nullptr),
			os(nullptr), oe(nullptr), inter(nullptr), type(-1){
		}

		void set(pNode pncs, pNode pnce,
				 pNode pnos, pNode pnoe,
				 pNode pinter, int t){
			cs    = pncs;
			ce    = pnce;
			os    = pnos;
			oe    = pnoe;
			inter = pinter;
			type  = t;
		}
	};

	typedef std::vector<IntersectionGroup> IntersectionTable;


	pNode cli;
	pNode obj;

	IntersectionTable _table;


	void phase_1(const PointChain& clip,
			     const PointChain& object){
		// copy point chain to node
		ASSERT(clip.size()   >=3);
		ASSERT(object.size() >=3);
		_build_clip_link(clip);
		_build_object_link(object);
	}

	void phase_2(){
		// build intersection table
		FunNNNN fun = [this](pNode pncs, pNode pnce, pNode pnos, pNode pnoe){
			IntersectionPairSS_<TYPE, 2> ssp(pncs->point,
					                         pnce->point,
											 pnos->point,
											 pnoe->point);
			IntersectionTypeSS itss = ssp.cal_intersection_type();
			IntersectionGroup  row;
			if( itss == _SS_CONNECT_ ||
				itss == _SS_OVERLAP_ ||
				itss == _SS_SAME_       ){
				// two start point connect
				if(_PS_ON_START_ == ssp.point_postion(0)){
					row.set(pncs, pnce, pnos, pnoe, pncs, 0);
				}
			}else if(itss == _SS_TOUCH_){
				if(_PS_IN_ == ssp.point_postion(0)) {
					row.set(pncs, pnce, pnos, pnoe, pncs, 1);
				}else if(_PS_IN_ == ssp.point_postion(2)){
					row.set(pncs, pnce, pnos, pnoe, pnos, 2);
				}
			}else if(itss == _SS_INTERSECT_){
				Point iterp = ssp.cal_intersection_point();
				pNode pnn   = new Node(iterp, _PN_);
				row.set(pncs, pnce, pnos, pnoe, pnn, 3);
			}
			if(row.type != -1){
				this->_table.push_back(row);
			}
		};

		for_each_for_each(fun);
	}

	void phase_3(){
		// combine two list
		// 1 combine two start point connect
		for(auto& row : this->_table){
			if(row.type == 0){
				this->_merge(row.cs, row.os);
			}
		}
		for (auto it = this->_table.begin(); it != this->_table.end();) {
			if (it->type == 0) {
				it = this->_table.erase(it);    //
			} else {
				++it;    //
			}
		}
		// 2 merge to clip
		std::set<typename IntersectionTable::size_type> usedic;
		for(typename IntersectionTable::size_type ic = 0;
				ic!=this->_table.size(); ic++){

			for(typename IntersectionTable::size_type io= ic+1;
					io!=this->_table.size(); io++){
					if(  this->_table[ic].cs == this->_table[io].cs
					   ||this->_table[ic].ce == this->_table[io].ce){

					}

				}

		}
	}

	void for_each_node(FunN fun){
		std::set<pNode> setp;
		pNode cur = cli;
		while (cur != nullptr) {
			setp.insert(cur);
			cur = cur->nextc;
		}
		cur = obj;
		while (cur != nullptr) {
			setp.insert(cur);
			cur = cur->nexto;
		}
		for(auto& pn : setp){
			fun(pn);
		}
	}


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

	void for_each_for_each(FunNNNN fun) {
		pNode fc = cli;
		pNode sc;
		pNode fo = obj;
		pNode so;
		while (fc->nextc != nullptr) {
			sc = fc->nextc;
			fo = obj;
			while( fo->nexto != nullptr){
				so = fo->nexto;
				fun(fc, sc, fo, so);
				fo = so;
			}
			fun(fc, sc, fo, obj);
			fc = sc;
		}

		fo = obj;
		while (fo->nexto != nullptr) {
			so = fo->nexto;
			fun(fc, cli, fo, so);
			fo = so;
		}
		fun(fc, cli, fo, obj);
	}

protected:
	void _build_clip_link(const PointChain& pc){
		pNode last = cli;
		for(auto& p : pc){
			pNode pn = new Node(p, _PC_);
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
		p->nexto    = pnew;
	}

	void _merge(pNode& pc, pNode& po){
		pc->prevo = po->prevo;
		pc->nexto = po->nexto;

		if (po->prevo != nullptr) {
			po->prevo->nexto = pc;
		}
		if (po->nexto != nullptr) {
			po->nexto->prevo = pc;
		}

		pc->type  = _PCO_;

		delete po;
	}

	std::string _to_string_node_type(int type){
		switch(type){
			case _PN_  : return "N";
			case _PC_  : return "C";
			case _PO_  : return "O";
			case _PCO_ : return "CO";
			default: return "ERROR TYPE";
		}
	}

public:
	void show_table(){
		int count = 0;
		std::cout << std::setw(5) << "Idx"
				  << std::setw(7) << "type"
				  << std::setw(7) << "c str"
				  << std::setw(8) << "c end"
				  << std::setw(9) << "o str"
				  << std::setw(9) << "o end"
				  << std::setw(9) << "n p"

				  << std::endl;
		for(auto& row : this->_table){
			std::cout << std::setw(5) << count
					  << std::setw(7) << row.type
					  << std::setw(3) << row.cs->point
					  << std::setw(3) << row.ce->point
					  << std::setw(3) << row.os->point
					  << std::setw(3) << row.oe->point
					  << std::setw(3) << row.inter->point
					  <<std::endl;
			count++;
		}
	}

	GnuplotActor::spActor actor_clip(Gnuplot& gnu) {
		GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3:4:5 title \"\" ";
		actor->style() = "with vectors lc variable";

		FunNN fun = [&actor, &gnu, this](pNode f, pNode s){
			Vt len = Distance(f->point, s->point);
			Vt dx  = s->point.x() - f->point.x();
			Vt dy  = s->point.y() - f->point.y();
			actor->data().push_back(
					ToString(f->point.x(),
							 f->point.y(),
							 dx,
							 dy, 1, " "));
		};

		for_each_edge_clip(fun);

		return actor;
	}

GnuplotActor::spActor actor_object(Gnuplot& gnu) {
		GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3:4:5 title \"\" ";
		actor->style() = "with vectors lc variable";

		FunNN fun = [&actor, &gnu, this](pNode f, pNode s) {
			Vt len = Distance(f->point, s->point);
			Vt dx = s->point.x() - f->point.x();
			Vt dy = s->point.y() - f->point.y();
			actor->data().push_back(
					ToString(f->point.x(),
							f->point.y(),
							dx,
							dy, 2, " "));
		};

		for_each_edge_object(fun);

		return actor;
	}

	GnuplotActor::spActor actor_label(Gnuplot& gnu) {
		GnuplotActor::spActor actor = Gnuplot::spActor(new Gnuplot_actor());

		int count = 1;
		FunN fun = [&actor, &count, &gnu, this](pNode pn) {
			gnu.set_label(count,
					this->_to_string_node_type(pn->type) +
					" ("+ ToString(pn->point.x(),pn->point.y(), ", ") + ")",
					pn->point.x(),pn->point.y(), "center");
			count++;
		};

		for_each_node(fun);

		return actor;
	}




};


}

#endif
