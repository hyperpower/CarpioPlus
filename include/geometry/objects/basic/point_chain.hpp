/*
 * _point_chain.hpp
 *
 *  Created on: Jul 6, 2017
 *      Author: zhou
 */

#ifndef _POINT_CHAIN_HPP_
#define _POINT_CHAIN_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include "box.hpp"
#include "algebra/array/array_list.hpp"
#include "segment.hpp"
#include <array>
#include <vector>
#include <limits>
#include <list>
#include <fstream>
#include <algorithm>
#include <functional>

namespace carpio {

template<class TYPE, St DIM>
class PointChain_ {
public:
	static const St Dim = DIM;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;
	typedef Segment_<TYPE, DIM> Segment;
	typedef Segment& ref_Segment;
	typedef Box_<TYPE, DIM> Box;
	typedef PointChain_<TYPE, DIM>& PointChain;
	typedef TYPE vt;
	typedef typename std::list<Point>::iterator iterator;
	typedef typename std::list<Point>::const_iterator const_iterator;
	typedef typename std::list<Point>::reference reference;
	typedef typename std::list<Point>::const_reference const_reference;
	typedef Operation_<TYPE, DIM> Op;
	typedef Point value_type;
	protected:
	/** Linked point chain */
	std::list<Point> _lpoints;
	bool _closed; // is the chain closed, that is, is the first point is linked with the last one?
public:
	PointChain_() :
			_lpoints(), _closed(false) {
	}
	PointChain_(const Point& p1) :
			_closed(false) {
		_lpoints.push_back(p1);
	}
	PointChain_(const Point& p1, const Point& p2) :
			_closed(false) {
		_lpoints.push_back(p1);
		_lpoints.push_back(p2);
	}
	PointChain_(const Point& p1, const Point& p2, const Point& p3, bool close =
			true) :
			_closed(close) {
		_lpoints.push_back(p1);
		_lpoints.push_back(p2);
		_lpoints.push_back(p3);
	}
	PointChain_(const Point& p1, const Point& p2, const Point& p3,
			const Point& p4, bool close = true) :
			_closed(close) {
		_lpoints.push_back(p1);
		_lpoints.push_back(p2);
		_lpoints.push_back(p3);
		_lpoints.push_back(p4);
	}

	template<class Container_Point>
	PointChain_(const Container_Point& ver,
			bool closed = true) : _closed(closed){
		std::copy(ver.begin(), ver.end(), std::back_inserter(_lpoints));
	}

	void init(const Segment& s) {
		_lpoints.push_back(s.ps());
		_lpoints.push_back(s.pe());
	}

	void set_close() {
		_closed = true;
	}

	bool closed() const {
		return _closed;
	}

	bool empty() const {
		return _lpoints.empty();
	}

	iterator begin() {
		return _lpoints.begin();
	}

	iterator end() {
		return _lpoints.end();
	}

	const_iterator begin() const {
		return _lpoints.begin();
	}
	const_iterator end() const {
		return _lpoints.end();
	}

	reference front() {
		return _lpoints.front();
	}
	const_reference front() const {
		return _lpoints.front();
	}

	reference back() {
		return _lpoints.back();
	}
	const_reference back() const {
		return _lpoints.back();
	}

	void push_back(const Point& p) {
		_lpoints.push_back(p);
	}

	void pop_back() {
		_lpoints.pop_back();
	}

	void clear() {
		_lpoints.clear();
	}

	unsigned int size() const {
		return _lpoints.size();
	}

	bool is_simple() const {
		return Op::IsSimple(_lpoints.begin(), _lpoints.end(), closed());
	}

	double perimeter() const {
		if (empty()) {
			return 0;
		}
		double res = 0;
		const_iterator iter_end = std::prev(_lpoints.end(), 1);
		for (const_iterator iter = _lpoints.begin();
				iter != iter_end; ++iter) {
			const_iterator iter1 = std::next(iter);
			res += Distance(*iter, *iter1);
		}
		if (closed()) {
			res += Distance(*iter_end, *(_lpoints.begin()));
		}
		return res;
	}

	Box box() const {
		Point min = this->_lpoints.front();
		Point max = this->_lpoints.front();
		for(auto& p: this->_lpoints){
			min = Min(min, p);
			max = Max(max, p);
		}
		return Box(min, max);
	}

	typedef std::function<void(Point&, Point&)> FunPP;
	typedef std::function<void(const Point&, const Point&)> FunConstPConstP;

	void for_each_edge(FunPP fun){
		auto iters = _lpoints.begin();
		auto itere = _lpoints.begin();
		std::advance(itere, 1);
		for(; itere != _lpoints.end();){
			fun((*iters), (*itere));
			iters++;
			itere++;
		}
	}

	void for_each_edge(FunConstPConstP fun) const{
		auto iters = _lpoints.begin();
		auto itere = _lpoints.begin();
		std::advance(itere, 1);
		for (; itere != _lpoints.end();) {
			fun((*iters), (*itere));
			iters++;
			itere++;
		}
		if(this->closed()){
			fun(_lpoints.back(), _lpoints.front());
		}
	}

	void show() const {
		int count = 0;
		for (auto& p : _lpoints) {
			std::cout << "i = " << count << " " << p << std::endl;
			count++;
		}
	}
};

template<class TYPE>
bool IsInOn(const Point_<TYPE, 2>& p, const PointChain_<TYPE, 2>& pc){
	// winding number method
	int    wn = 0;    // the  winding number counter

	typedef Point_<TYPE, 2> Point;
	pc.for_each_edge([&p, &wn](const Point& ps, const Point& pe){
		if(ps.y() <= p.y()){
			if(pe.y() >= p.y()){
				auto pos = OnWhichSide7(ps, pe, p);
				if(pos == _PS_LEFT_){
					++wn;
				}else if(  pos == _PS_ON_END_
						|| pos == _PS_ON_START_
						|| pos == _PS_IN_){
					return true;
				}
			}
		}else{
			if(pe.y() <= p.y()){
				auto pos = OnWhichSide7(ps, pe, p);
				if(pos == _PS_RIGHT_){
					--wn;
				}else if(  pos == _PS_ON_END_
						|| pos == _PS_ON_START_
						|| pos == _PS_IN_){
					return true;
				}
			}
		}
	});
	return wn > 0;
}

}

#endif
