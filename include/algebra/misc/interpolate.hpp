#ifndef _INTERPOLATE_HPP_
#define _INTERPOLATE_HPP_

#include <type_define.hpp>
#include <utility>
#include <map>

namespace carpio {

// 1D Interpolation
//   linear                           OK
//   quadratic                        ....
//   cubic spline                     ....
//   monotonic spline interpolation   ....
// 2D Interpolation
//   bilinear                         OK
//   bicubic                          ....
//   thin plate spline                ....
// 3D Interpolation
//   trilinear                        OK

template<class CVT, class VT>
struct AInterpolate_ {

    ///
    template<class DIM, class POI, class ARR>
    static VT Linear_all(const DIM& dim, const POI& p, const POI& p1,
                         const POI& p2,
                         const ARR& av) {
        if (dim == 1) {
            return Linear(p[0], p1[0], p2[0], av[0], av[1]);
        }
        if (dim == 2) {
            return Bilinear(
                       p[0], p[1],
                       p1[0], p1[1], //
                       p2[0], p2[1], //
                       av[0], av[1], av[2], av[3]);
        }
        if (dim == 3) {
            return Trilinear(     //
                       p[0], p[1], p[2],  //
                       p1[0], p1[1], p1[2],  //
                       p2[0], p2[1], p2[2],  //
                       av[0], av[1], av[2], av[3], av[4], av[5], av[6], av[7]);
        }
    }

    /// Linear interpolate.
    /// Linear reference
    /// u is a normalized u
    static VT Linear_r(CVT u, const VT& a, const VT& b) {
        return ((a * (1.0 - u)) + (b * u));
    }

    // linear interpolation
    // https://en.wikipedia.org/wiki/Linear_interpolation
    static VT Linear(CVT x, CVT x1, CVT x2, const VT& a, const VT& b) {
        CVT u = (x - x1) / (x2 - x1);
        return Linear_r(u, a, b);
    }

    /// Bilinear interpolate.
    /* Corners are:
     *   1b -----2c
     * ^  |       |
     * |  |       |
     * v  |       |
     *   0a -----3d
     *     u->
     */
    static VT Bilinear_r(CVT u, CVT v, const VT& a, const VT& b, const VT& c,
                         const VT& d) {
        return ((a * (1 - u) * (1 - v)) + (d * (1 - u) * v) + (b * u * (1 - v))
                + (c * u * v));
    }

    static VT Bilinear(     //
        CVT x, CVT y,   //
        CVT x1, CVT y1, //
        CVT x2, CVT y2, //
        const VT& a, const VT& b, const VT& c, const VT& d) {
        CVT u = (x - x1) / (x2 - x1);
        CVT v = (y - y1) / (y2 - y1);
        return Bilinear_r(u, v, a, b, c, d);
    }

    /// Trilinear interpolate.
    static VT Trilinear_r(CVT u, CVT v, CVT w, const VT& a, const VT& b,
                          const VT& c, const VT& d, const VT& e, const VT& f, const VT& g,
                          const VT& h) {
        VT t0(Bilinear_r(u, v, a, b, c, d));
        VT t1(Bilinear_r(u, v, e, f, g, h));

        return (((1 - w) * t0) + (w * t1));
    }

    static VT Trilinear(
        //
        CVT x, CVT y,
        CVT z, //
        CVT x1, CVT y1,
        CVT z1, //
        CVT x2, CVT y2,
        CVT z2, //
        const VT& a, const VT& b, const VT& c, const VT& d, const VT& e,
        const VT& f, const VT& g, const VT& h) {
        CVT u = (x - x1) / (x2 - x1);
        CVT v = (y - y1) / (y2 - y1);
        CVT w = (z - z1) / (z2 - z1);
        return Trilinear_r(u, v, w, a, b, c, d, e, f, g, h);
    }

};



}

#endif
