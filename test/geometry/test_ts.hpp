/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */

#ifndef GEOMETRY_TEST_TS_HPP_
#define GEOMETRY_TEST_TS_HPP_
#include "geometry/geometry.hpp"
#include "geometry/io/gplotly_actor_ts.hpp"
#include "gtest/gtest.h"

namespace carpio {


TEST(TS, ts_initial){
	typedef TriSurface_<double, 3> TS;
	typedef CreateTS_<double, 3> CreateTS;
	typedef GPlotlyActor_TS_<double, 3> PA;

	TS ts;
	CreateTS::TriFaceOne(ts,
			TS::Poi(0.0, 0.0, 0.0),
			TS::Poi(0.1, 0.1, 0.1),
			TS::Poi(0.0, 0.5, 0.3));

	Plotly plot;
	plot.add(PA::Surface(ts));
//	plot.plot();

}

TEST(TS, ts_initial2){
	typedef TriSurface_<double, 3> TS;
	typedef CreateTS_<double, 3> CreateTS;
	typedef GPlotlyActor_TS_<double, 3> PA;

	TS ts;
	CreateTS::Circle(ts, 50, 1.0);

	Plotly plot;
	plot.add(PA::WireFrame(ts));
	plot.plot();

}

}

#endif
