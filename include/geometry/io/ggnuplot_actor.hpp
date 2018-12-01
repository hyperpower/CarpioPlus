/*
 * _actor_gnuplot.hpp
 *
 *  Created on: Jul 5, 2017
 *      Author: zhou
 */

#ifndef _ACTOR_GNUPLOT_GEOMETRY_HPP_
#define _ACTOR_GNUPLOT_GEOMETRY_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
#include "geometry/objects/objects.hpp"
#include "io/gnuplot.hpp"
#include <memory>
#include <cmath>

namespace carpio {

template<typename TYPE, St DIM>
class GGnuplotActor_ {
public:
	static const St Dim = DIM;
	typedef TYPE Vt;
	typedef Point_<TYPE, DIM> Point;
	typedef Point_<TYPE, DIM>& ref_Point;
	typedef const Point_<TYPE, DIM>& const_ref_Point;
	typedef Segment_<TYPE, DIM> Segment;
	typedef Segment& ref_Segment;
	typedef const Segment& const_ref_Segment;

	typedef PointChain_<TYPE, DIM> PointChain;

	typedef std::shared_ptr<Gnuplot_actor> spActor;
	typedef std::list<spActor> list_spActor;

public:
	static spActor Points(
			const Point& p,
			int color_idx = -1) {
		ASSERT(Dim == 2);
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style()   = "with points lc variable";

		if (color_idx >= 0) {
			actor->data().push_back(ToString(p.x(), p.y(), color_idx, " "));
		} else {
			actor->data().push_back(ToString(p.x(), p.y(), 0, " "));
		}

		actor->data().push_back("");
		return actor;
	}
	template<class ITERPOINTS>
	static spActor Points(
			ITERPOINTS begin,
			ITERPOINTS end,
			int color_idx      =-1) {
		ASSERT(Dim == 2);
		int color = color_idx > 0? color_idx : 0;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style()   = "with points lc variable";

		auto iterps = begin;
		for (; iterps != end; iterps++) {
			actor->data().push_back(
					ToString(iterps->x(), iterps->y(), color, " "));
		}
		actor->data().push_back("");
		return actor;
	}

	template<class ITERPOINTS>
	static spActor LinesPoints(
				ITERPOINTS begin,
				ITERPOINTS end,
				int color_idx      = -1,
				bool close         = false) {
		ASSERT(Dim == 2);
		int color = color_idx > 0? color_idx : 0;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style()   = "with linespoints lc variable";

		auto iterps = begin;
		auto iterpe = std::next(begin);
		for (; iterpe != end; iterps++, iterpe++) {
			actor->data().push_back(
					ToString(iterps->x(), iterps->y(), color, " "));
			actor->data().push_back(
					ToString(iterpe->x(), iterpe->y(), color, " "));
			actor->data().push_back("");
		}

		if (close) {
			auto iterf = begin;
			auto iterb = std::prev(end);
			actor->data().push_back(
					ToString(iterb->x(), iterb->y(), color, " "));
			actor->data().push_back(
					ToString(iterf->x(), iterf->y(), color, " "));
			actor->data().push_back("");
		}

		return actor;
	}

	template<class ITERPOINTS>
	static spActor Lines(ITERPOINTS begin,
			             ITERPOINTS end,
						 int color_idx = -1,
			             bool close = false) {
		ASSERT(Dim == 2);
		int color = color_idx > 0? color_idx : 0;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style()   = "with lines lc variable";

		auto iterps = begin;
		auto iterpe = std::next(begin);
		for (; iterpe != end; iterps++, iterpe++) {
			actor->data().push_back(
					ToString(iterps->x(), iterps->y(), color, " "));
			actor->data().push_back(
					ToString(iterpe->x(), iterpe->y(), color, " "));
			actor->data().push_back("");
		}

		if (close) {
			auto iterf = begin;
			auto iterb = std::prev(end);
			actor->data().push_back(
					ToString(iterb->x(), iterb->y(), color, " "));
			actor->data().push_back(
					ToString(iterf->x(), iterf->y(), color, " "));
			actor->data().push_back("");
		}

		return actor;
	}

	static spActor LinesPoints(
			const Segment& seg,
			int color_idx = -1) {
		spActor actor = spActor(new Gnuplot_actor());
		int color = color_idx > 0? color_idx : 0;
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style()   = "with linespoints lc variable";
		if (seg.empty()) {
			actor->data().push_back("");
			return actor;
		}
		actor->data().push_back(ToString(seg.psx(), seg.psy(), color, " "));
		actor->data().push_back(ToString(seg.pex(), seg.pey(), color, " "));

		actor->data().push_back("");
		return actor;
	}

	static spActor LinesPoints(
			const PointChain& pc,
			int color_idx = -1) {
		spActor actor = spActor(new Gnuplot_actor());
		int color = color_idx > 0? color_idx : 0;
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with linespoints lc variable";
		if (pc.empty()) {
			actor->data().push_back("");
			return actor;
		}

		for (auto& p : pc) {
			actor->data().push_back(
					ToString(p.x(), p.y(), color, " "));
		}

		actor->data().push_back(ToString(pc.front().x(), pc.front().y(), color, " "));
		return actor;
	}

	static spActor Arrows(
			const PointChain& pc,
			int color_idx = -1) {
		spActor actor = spActor(new Gnuplot_actor());
		int color = color_idx > 0? color_idx : 0;
		actor->command() = "using 1:2:3:4:5 title \"\" ";
		actor->style()   = "with vectors lc variable";
		if (pc.empty()) {
			actor->data().push_back("");
			return actor;
		}

		auto iterps = pc.begin();
		auto iterpe = pc.begin();
		std::advance(iterpe, 1);
		for (;iterpe !=pc.end();iterps++, iterpe++) {
			actor->data().push_back(
					ToString(iterps->x(), iterps->y(),
							 iterpe->x()-iterps->x() ,
							 iterpe->y()-iterps->y(), color, " "));
		}
		if (pc.closed()) {
			actor->data().push_back(
					ToString(pc.back().x(), pc.back().y(),
			 				 pc.front().x() - pc.back().x(),
							 pc.front().y() - pc.back().y(), color,
							 " "));
		}

		return actor;
	}
};

}

#endif /* _ACTOR_GNUPLOT_HPP_ */
