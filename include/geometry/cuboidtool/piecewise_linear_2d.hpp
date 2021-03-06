#ifndef _PIECEWISE_LINEAR_2D_HPP_
#define _PIECEWISE_LINEAR_2D_HPP_

#include "cuboidtool_define.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/box.hpp"
#include "algebra/algebra.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {


template<class TYPE>
class CuboidToolPL2_:public CuboidTool_<TYPE, 2>{
public:
    static const St Dim = 2;
    static const St NumFaces = 4;
    static const St NumVertexes = 4;

    typedef TYPE Vt;
    typedef CuboidTool_<TYPE, 2>                Base;
    typedef Point_<TYPE, Dim>                  Point;
    typedef Point_<TYPE, Dim>&             ref_Point;
    typedef const Point_<TYPE, Dim>& const_ref_Point;

    typedef Line_<Vt>                           Line;
    typedef Line*                              pLine;
    typedef std::shared_ptr<Line>             spLine;
    typedef const pLine                  const_pLine;

    typedef Box_<Vt, Dim>                      Box;
    typedef Box*                              pBox;
    typedef std::shared_ptr<Box>             spBox;
    typedef const pBox                  const_pBox;

    typedef Segment_<Vt, Dim>                Segment;
    typedef Segment*                        pSegment;
    typedef std::shared_ptr<Segment>       spSegment;
    typedef const pSegment            const_pSegment;

    typedef PointChain_<Vt, Dim>          PointChain;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

public:
    CuboidToolPL2_():Base(){}
    /*****************************************************
     * Cut cell functions
     * 1 calculate edge aperture ratio by FunXYZT
     * 2 calculate cell aperture ratios by FunXYZT
     * 3 get start point of the piecewise linear -- 2D
     * 4 get end point of the piecewise linear -- 2D
     ****************************************************/
    Point start_point(
            const Vt& xo,
            const Vt& yo,
            const Vt& dx,
            const Vt& dy,
            const std::array<Vt, 4>& arr
            ){
        // 0 p
        // 1 n
        // 2 n
        // 3 p
        for(int i = 0; i< 4; i++){
            int eo = CCEDGEORDER2[i];
            Vt  ar = arr[eo];  // get aperture ratio
            if(ar >= 1e-12 && !IsCloseTo(std::abs(ar), 1.0)){
                if (eo == 0) {
                    return Point(xo, yo + dy * ar);
                }
                if (eo == 3){
                    return Point(xo + dx * ar, yo + dy);
                }
            }
            if(ar <= -1e-12 && !IsCloseTo(std::abs(ar), 1.0)){
                if (eo == 1) {
                    return Point(xo + dx, yo + dy * (1 + ar));
                }
                if (eo == 2) {
                    return Point(xo + dx * (1 + ar), yo);
                }
            }
        }
        // trivial case : 1 or -1 follow 0
        // trivial case : 1 or -1 follow 0
        if ((IsCloseTo(std::abs(arr[0]), 1.0) && IsCloseToZero(arr[2]))
                || (IsCloseTo(std::abs(arr[2]), 1.0) && IsCloseToZero(arr[0]))) {
            return Point(xo, yo);
        }
        if ((IsCloseTo(std::abs(arr[2]), 1.0) && IsCloseToZero(arr[1]))
                || (IsCloseTo(std::abs(arr[1]), 1.0) && IsCloseToZero(arr[2]))) {
            return Point(xo + dy, yo);
        }
        if ((IsCloseTo(std::abs(arr[1]), 1.0) && IsCloseToZero(arr[3]))
                || (IsCloseTo(std::abs(arr[3]), 1.0) && IsCloseToZero(arr[1]))) {
            return Point(xo + dy, yo + dy);
        }
        if ((IsCloseTo(std::abs(arr[3]), 1.0) && IsCloseToZero(arr[0]))
                || (IsCloseTo(std::abs(arr[0]), 1.0) && IsCloseToZero(arr[3]))) {
            return Point(xo, yo + dy);
        }
    }

    Point end_point(
                const Vt& xo,
                const Vt& yo,
                const Vt& dx,
                const Vt& dy,
                const std::array<Vt, 4>& arr
                ){
        // 0 n
        // 1 p
        // 2 p
        // 3 n
        for (int i = 0; i < 4; i++) {
            int eo = CCEDGEORDER2[i];
            Vt ar = arr[eo];  // get aperture ratio
            if (ar <= -1e-12 && !IsCloseTo(std::abs(ar), 1.0)) {
                if (eo == 0) {
                    return Point(xo, yo + dy * (1 + ar));
                }
                if (eo == 3) {
                    return Point(xo + dx * (1 + ar), yo + dy);
                }
            }
            if (ar >= 1e-12 && !IsCloseTo(std::abs(ar), 1.0)) {
                if (eo == 1) {
                    return Point(xo + dx, yo + dy * ar);
                }
                if (eo == 2) {
                    return Point(xo + dx * ar, yo);
                }
            }
        }
        // trivial case : 1 or -1 follow 0
        Point pres;  // get the second one
        if ((IsCloseTo(std::abs(arr[0]), 1.0) && IsCloseToZero(arr[2]))
            || (IsCloseTo(std::abs(arr[2]), 1.0) && IsCloseToZero(arr[0]))) {
            pres = Point(xo, yo);
        }
        if ((IsCloseTo(std::abs(arr[2]), 1.0) && IsCloseToZero(arr[1]))
            || (IsCloseTo(std::abs(arr[1]), 1.0) && IsCloseToZero(arr[2]))) {
            pres = Point(xo + dy, yo);
        }
        if ((IsCloseTo(std::abs(arr[1]),1.0) && IsCloseToZero(arr[3]))
            || (IsCloseTo(std::abs(arr[3]),1.0) && IsCloseToZero(arr[1]))) {
            pres = Point(xo + dy, yo + dy);
        }
        if ((IsCloseTo(std::abs(arr[3]), 1.0) && IsCloseToZero(arr[0]))
            || (IsCloseTo(std::abs(arr[0]), 1.0) && IsCloseToZero(arr[3]))) {
            pres = Point(xo, yo + dy);
        }
        return pres;
    }

    Line interface(const Point& start,
                   const Point& end){
        Point ori = Point(0.0, 0.0);
        Point sstart = start - ori;  // shift start
        Point send   = end   - ori;  // shift end
        return Line(sstart, send);
    }

    Line interface(const Point& start,
                   const Point& end,
                   const Point& ori){
        auto sstart = start - ori; // shift start
        auto send   = end   - ori;  // shift end
        return Line(sstart, send);
    }

    PointChain cut_cell_point_chain_ghost_side(
            const Vt& xo,
            const Vt& yo,
            const Vt& dx,
            const Vt& dy,
            const std::array<Vt, NumFaces>& arr){
        auto ccarr = _to_cc_aperture_ratios(arr);
        PointChain res;
        _push_back_each_edge(
                Point(xo, yo), Point(xo + dx, yo),
                ccarr[3], ccarr[0], ccarr[1], res);
        _push_back_each_edge(
                Point(xo + dx, yo), Point(xo + dx, yo + dy),
                ccarr[0], ccarr[1], ccarr[2], res);
        _push_back_each_edge(
                Point(xo + dx, yo + dy), Point(xo, yo + dy),
                ccarr[1], ccarr[2], ccarr[3], res);
        _push_back_each_edge(
                Point(xo, yo + dy), Point(xo, yo),
                ccarr[2], ccarr[3], ccarr[0], res);
        return res;
    }


    PointChain cut_cell_point_chain_normal_side(
            const Vt& xo, const Vt& yo,
            const Vt& dx, const Vt& dy,
            const std::array<Vt, NumFaces>& arr) {
        auto ccarr = _to_cc_aperture_ratios(arr);
        PointChain res;
        _push_back_each_edge_void_side(
                Point(xo, yo), Point(xo + dx, yo),
                ccarr[3], ccarr[0], ccarr[1],
                res);
        _push_back_each_edge_void_side(
                Point(xo + dx, yo), Point(xo + dx, yo + dy),
                ccarr[0], ccarr[1], ccarr[2], res);
        _push_back_each_edge_void_side(
                Point(xo + dx, yo + dy), Point(xo, yo + dy),
                ccarr[1], ccarr[2], ccarr[3], res);
        _push_back_each_edge_void_side(
                Point(xo, yo + dy), Point(xo, yo),
                ccarr[2], ccarr[3], ccarr[0], res);
        return res;
    }


    void _push_back_each_edge_void_side(
            const Point& p0, // vertex 0
            const Point& p1, // vertex 1
            const Vt& prev,
            const Vt& r,
            const Vt& next, PointChain& pc) {
        if (std::abs(r) == 1.0) {
            return;
        }
        if (r == 0.0) {
            if (std::abs(prev) == 1 || prev == 0) {
                pc.push_back(p0);
            }
            if (std::abs(next) == 1) {
                pc.push_back(p1);
            }
        } else if (r < 0.0) {
            pc.push_back(p0);
            pc.push_back(Between(p0, p1, r));
        } else { // r > 0.0
            pc.push_back(Between(p0, p1, r));
            pc.push_back(p1);
        }
    }


    void _push_back_each_edge(
            const Point& p0, // vertex 0
            const Point& p1, // vertex 1
            const Vt&    prev,
            const Vt&    r,
            const Vt&    next,
            PointChain&  pc){
        if (r == 0.0) {
            return;
        }
        if (std::abs(r) == 1.0){
            if(prev == 0 || std::abs(prev) == 1){
                pc.push_back(p0);
            }
            if(next == 0){
                pc.push_back(p1);
            }
        }else if (r < 0.0) {
            pc.push_back(Between(p0, p1, r));
            pc.push_back(p1);
        } else { // r > 0.0
            pc.push_back(p0);
            pc.push_back(Between(p0, p1, r));
        }
    }
    // to count-clock wise aperture ratios
    std::array<Vt, NumFaces> _to_cc_aperture_ratios(
            const std::array<Vt, NumFaces>& arr
            ){
        std::array<Vt, NumFaces> res;
        // edge order   count-clock wise order
        //     3              2
        // *------>*      *<------*
        // ^       ^      |       ^
        // |0      |1     |3      |1
        // |       |      _       |
        // *------>*      *------>*
        //     2              0
        res[0] = arr[2];
        res[1] = arr[1];
        res[2] = _change_direction(arr[3]);
        res[3] = _change_direction(arr[0]);
        return res;
    }

    Vt _change_direction(const Vt& ap){
        if(ap == 0.0){
            return 0.0;
        }else{
            return -ap;
        }
    }

    std::array<Vt, NumFaces> cut_cell_aperture_ratios(
            const Vt& xo,
            const Vt& yo,
            const Vt& dx,
            const Vt& dy,
            const Vt& time,
            const Vt& th,
            FunXYZT_Value fun,
            const Vt& tol) const{
        std::array<Vt, NumVertexes> arrv;
        arrv[0] = fun(xo,   yo,   0.0,time);
        arrv[1] = fun(xo+dx,yo,   0.0,time);
        arrv[2] = fun(xo,   yo+dy,0.0,time);
        arrv[3] = fun(xo+dx,yo+dy,0.0,time);
        return cut_cell_aperture_ratios(xo, yo, dx, dy, time, th, fun, tol, arrv);
    }

    std::array<Vt, NumFaces> cut_cell_aperture_ratios(
            const Vt& xo,
            const Vt& yo,
            const Vt& dx,
            const Vt& dy,
            const Vt& time,
            const Vt& th,
            FunXYZT_Value fun,
            const Vt& tol,
            const std::array<Vt, NumVertexes>& arrv) const{
        // v0 - 2 => e0
        // v1 - 3 => e1
        // v0 - 1 => e2
        // v2 - 3 => e3
        std::array<Vt, NumFaces> arrres;
        auto se0  = _aperture_state(arrv[0], arrv[2], th);
        arrres[0] = edge_aperture_ratio(xo,   yo, _Y_, dy,
                                            time, th, fun, tol, se0);
        auto se1  = _aperture_state(arrv[1], arrv[3], th);
        arrres[1] = edge_aperture_ratio(xo + dx, yo, _Y_, dy,
                                            time,    th, fun, tol, se1);

        auto se2  = _aperture_state(arrv[0], arrv[1], th);
        arrres[2] = edge_aperture_ratio(xo, yo, _X_, dx,
                                            time, th, fun, tol, se2);

        auto se3  = _aperture_state(arrv[2], arrv[3], th);
        arrres[3] = edge_aperture_ratio(xo, yo + dy, _X_, dx,
                                            time, th, fun, tol, se3);
        return arrres;
    }

    /*******************
     * \brief   Calculate edge aperture ratio
     *          Edge defined as:
     *          1 Origin point
     *          2 Axes
     *          3 Distance
     *          4 FunXYZT
     *
     *          return
     *          aperture ratio is number with direction
     *          *=====-----*  aperture ratio =  0.5
     *          *-----=====*  aperture ratio = -0.5
     *          *----------*  aperture ratio =  0.0
     *          *==========*  aperture ratio =  1.0 or -1.0
     *          |---------> positive direction
     ******************/
    Vt edge_aperture_ratio(
            const Vt&   xo,
            const Vt&   yo,
            const Axes& a,
            const Vt&   dis,
            const Vt&   time,
            const Vt&   th,
            FunXYZT_Value fun,
            const Vt&   tol,
            const int&  state) const{
        if(state == 2){
            return 0.0;
        }else if(state == -2){
            return 1.0;
        } else {
            std::function<Vt(Vt)> funx = [fun, &yo, &time](Vt v) {
                Vt res = fun(v, yo, 0.0, time);
                return res;
            };
            std::function<Vt(Vt)> funy = [fun, &xo, &time](Vt v) {
                Vt res = fun(xo, v, 0.0, time);
                return res;
            };
            Vt loc = 0.0;
            if (a == _X_) {
                loc = SolveDichotomy(xo, xo + dis, th, tol, funx);
                if(state == 1){
                    return (loc - xo) / dis;
                }else{
                    return -(1.0 - (loc - xo) / dis);
                }
            } else {
                loc = SolveDichotomy(yo, yo + dis, th, tol, funy);
                if (state == 1) {
                    return (loc - yo) / dis;
                } else {
                    return -(1.0 - (loc - yo) / dis);
                }
            }
        }
    }
    /********************
     * \brief  if value vm and vp > th, then return  2
     *                            < th, then return -2
     *                  vm<th  vp>th,   then return  1
     *                  vm>th  vp<th,   then return -1
     *******************/
    int _edge_aperture_state(
            const Vt&    xo,
            const Vt&    yo,
            const Axes&  a,          // _X_ or _Y_
            const Orientation& ori,  // _M_ or _P_
            const Vt&    dis,
            const Vt&    time,
            const Vt&    th,
            FunXYZT_Value fun) const{
        Vt x  = (a == _X_) ? (ori == _M_ ? xo - dis : xo + dis) : xo;
        Vt y  = (a == _Y_) ? (ori == _M_ ? yo - dis : yo + dis) : yo;
        Vt vm = fun(xo, yo, 0.0, time);
        Vt vp = fun(x,  y, 0.0, time);
        return _aperture_state(vm, vp, th);
    }

    int _aperture_state(const Vt& vm, const Vt& vp, const Vt& th) const{
        if (vm >= th && vp >= th) {
            return 2;
        }
        if (vm <= th && vp <= th) {
            return -2;
        }
        if (vm <= th && vp >= th) {
            return 1;
        }
        if (vm >= th && vp <= th) {
            return -1;
        }
        SHOULD_NOT_REACH;
        return 0;
    }




    /*****************************************************
     * Forward Problem
     *
     * Known the Line, calculate Area(color)
     *****************************************************/
    Vt cal_area(const Line& l, const Point& pmin, const Point& pmax) const{
        Vt c1 = pmax.x() - pmin.x();
        Vt c2 = pmax.y() - pmin.y();
        // pmin becomes the original point
        Vt a = l.a();
        Vt b = l.b();
        Line ln(a, b, l.alpha() - a * pmin.x() - b * pmin.y());

        return cal_area(ln, c1, c2);
    }
    Vt cal_color(const Line& l, const Point& pmin, const Point& pmax) const{
        Vt c1 = pmax.x() - pmin.x();
        Vt c2 = pmax.y() - pmin.y();
        return this->cal_area(l, pmin, pmax) / c1 / c2;
    }
    /**
     * \brief   Calculate the negative area divided by the line
     *          in a unit box (Edge length = 1)
     *
     * \param   Line l the line
     * \return  area
     */
    Vt cal_area(const Line& l) const{
        Vt m1     = l.a();
        Vt m2     = l.b();
        Vt alpha  = l.alpha();
        int cases = this->_which_case_4(m1, m2);
        switch(cases){
        case 1:
            return this->_cal_area(m1, m2, alpha);
        case 2:
            return this->_cal_area(-m1, m2, alpha - m1);
        case 3:
            return this->_cal_area(-m1, -m2, alpha - m1 - m2);
        case 4:
            return this->_cal_area(m1, -m2, alpha - m2);
        }
        SHOULD_NOT_REACH;
        return 0.0;
    }
    Vt cal_color(const Line& l) const{
        return this->cal_area(l);
    }
    /**
     * \brief   Calculate the negative area divided by the line
     *
     * \param   Line l the line
     * \param   Vt c1 box edge length on X
     * \param   Vt c1 box edge length on Y
     * \return  area
     */
    Vt cal_area(const Line& l, const Vt& c1, const Vt& c2) const{
        Vt m1     = l.a();
        Vt m2     = l.b();
        Vt alpha  = l.alpha();
        int cases = this->_which_case_4(m1, m2);
        switch(cases){
        case 1:
            return this->_cal_area(m1, m2, alpha, c1, c2);
        case 2:
            return this->_cal_area(-m1, m2, alpha - m1 * c1, c1, c2);
        case 3:
            return this->_cal_area(-m1, -m2, alpha - m1 * c1 - m2 * c2, c1, c2);
        case 4:
            return this->_cal_area(m1, -m2, alpha - m2 * c2, c1, c2);
        }
        SHOULD_NOT_REACH;
        return 0.0;
    }

    Vt cal_color(const Line& l, const Vt& c1, const Vt& c2) const{
        return this->cal_area(l, c1, c2) / c1 / c2;
    }

    /*****************************************************
     * Inverse Problem
     *
     * Known the color function and normal vector, calculate Line
     *****************************************************/
    spLine construct_interface(Vt n1, Vt n2, Vt C, Vt c1, Vt c2) const{
        Vt area   = C * c1 * c2;
        Vt alpha  = this->_cal_alpha(std::abs(n1), std::abs(n2), area, c1, c2);
        int cases = this->_which_case_4(n1,n2);
        switch (cases) {
        case 1:
            return spLine(new Line(n1, n2, alpha));
        case 2:
            return spLine(new Line(n1, n2, alpha + n1 * c1));
        case 3:
            return spLine(new Line(n1, n2, alpha + n1 * c1 + n2 * c2));
        case 4:
            return spLine(new Line(n1, n2, alpha + n2 * c2));
        }
        return nullptr;
    }

    /**
     * \brief   known the normal vector (nx, ny)
     *          and color function C
     *
     * \param   Vt nx x component of n
     * \param   Vt nx x component of n
     * \param   Vt C the color function
     * \return  spLine
     */
    spLine construct_interface(Vt n1, Vt n2, Vt C) const{
        Vt alpha  = this->_cal_alpha_in_unit_box(std::abs(n1), std::abs(n2), C);
        int cases = this->_which_case_4(n1,n2);
        switch (cases) {
        case 1:
            return spLine(new Line(n1, n2, alpha));
        case 2:
            return spLine(new Line(n1, n2, alpha + n1));
        case 3:
            return spLine(new Line(n1, n2, alpha + n1 + n2));
        case 4:
            return spLine(new Line(n1, n2, alpha + n2));
        }
        return nullptr;
    }
    /*****************************************************
     * Middle Problem
     *
     * Known the Line, calculate Segment
     *****************************************************/
    spSegment cal_segment(const Line& line, Vt c1, Vt c2) const{
        std::array<Axes, 4> ao = {_Y_, _Y_, _X_, _X_};
        std::array<Vt, 4>   av = {0.0 , c2, 0.0, c1};
        std::array<Vt, 4>   rv = {c1  , c1, c2,  c2};
        int f = 0;
        spSegment sps(new Segment());
        for(int i = 0; i < 4; i++){
            Vt cv = line.cal(ao[i], av[i]);
            if(IsInRange(0.0, cv, rv[i], _cc_)){
                sps->p(f)[_X_] = (ao[i] == _Y_) ? cv : av[i];
                sps->p(f)[_Y_] = (ao[i] == _X_) ? cv : av[i];
                f++;
            }
            if(f > 1){
                break;
            }
        }
        // orientation
        return sps;
    }
//protected:
    /**
     * \brief   known a,b in ax+by=alpha and C, calculate alpha \n
     *          no matter what a and b are, they will be change to abs(a) and abs(b)
     *          return alpha, ax+by=alpha, a>b>0;
     *          The box must be a unit box
     *
     *          Piecewise Linear Interface Calculation
     *          [Calcul d'Interface Affine par Morceaux]
     *
     * \param   Float a a in ax+by=alpha
     * \param   Float b b in ax+by=alpha
     * \param   Float C the color function
     * \return  alpha
     */
    Vt _cal_alpha_in_unit_box(Vt a, Vt b, Vt C) const{
        Vt c1, c2, alpha;
        Vt absa = std::abs(a);
        Vt absb = std::abs(b);
        Vt m, n;
        n = (absa >= absb) ? (absa) : absb;
        m = (absa <= absb) ? (absa) : absb;
        c1 = m / 2 / n;
        c2 = 1 - c1;
        if (C >= 0 && C <= c1) {
            alpha = std::sqrt(2.0 * C * m * n);
        } else if (C > c1 && C < c2) {
            alpha = (2.0 * C * n + m) / 2.0;
        } else { //(C>=c2 && C<=1)
            alpha = m + n - std::sqrt(2.0 * (1.0 - C) * m * n);
        }
        return alpha;
    }
    Vt _cal_alpha(Vt m1, Vt m2, Vt A, Vt c1, Vt c2) const{
        Vt n1, n2, d1, d2, alpha;
        if(m1 * c1 < m2 * c2){
            n2 = m2; d2 = c2;
            n1 = m1; d1 = c1;
        } else {
            n2 = m1; d2 = c1;
            n1 = m2; d1 = c2;
        }
        Vt Ac1 = 0.5 * d1 * d1 * n1 / n2;
        Vt Ac2 = d1 * d2 - Ac1;
        if (A >= 0 && A <= Ac1){
            return std::sqrt(2.0 * A * n1 * n2);
        } else if (A > Ac1 && A < Ac2){
            return (2.0 * A * n2 + n1 * d1 * d1) / 2.0 / d1;
        } else {
            return n1 * d1 + n2 * d2 - std::sqrt(2.0 * (d1 * d2 - A) * n1 * n2);
        }
        SHOULD_NOT_REACH;
        return 0;
    }
    /**
     * \brief   m1, m2 must be positive
     *
     *          Volume-of-Fluid Interface Tracking with Smoothed
     *          Surface Stress Methods for Three-Dimensional Flows
     * \param   Float a a in ax+by=alpha
     * \param   Float b b in ax+by=alpha
     * \param   Float C the color function
     * \return  alpha
     */
    Vt _cal_area(
            const Vt& m1, const Vt& m2, const Vt alpha,
            const Vt& c1, const Vt& c2) const{
        // m1, m2 and alpha must be positive
        Vt amc1  = alpha - m1 * c1;
        //trivial case
        if (alpha <= 0.0){
            return 0.0;
        }
        if (amc1 - m2 * c2 >= 0.0){
            return c1 * c2;
        }
        Vt amc2  = alpha - m2 * c2;
        Vt term1 = Heaviside(amc1) * amc1 * amc1;
        Vt term2 = Heaviside(amc2) * amc2 * amc2;
        return 0.5 * (alpha * alpha - term1 - term2) / m1 / m2;
    }
    Vt _cal_area(const Vt& m1, const Vt& m2, const Vt alpha) const{
        // m1, m2 and alpha must be positive
        Vt amc1 = alpha - m1;
        //trivial case
        if (alpha <= 0.0) {
            return 0.0;
        }
        if (amc1 - m2  >= 0.0) {
            return 1.0;
        }
        Vt amc2 = alpha - m2;
        Vt term1 = Heaviside(amc1) * amc1 * amc1;
        Vt term2 = Heaviside(amc2) * amc2 * amc2;
        return 0.5 * (alpha * alpha - term1 - term2) / m1 / m2;
    }

    /**
     * \brief   Calculate the direction of the line normal vector
     *          <pre>
     *                          ^y
     *                          |
     *                          |
     *                    2     |     1
     *                          |
     *                          |
     *          --+------+------+------+------+--> x
     *                          |
     *                          |
     *                    3     |     4
     *                          |
     *                          |
     *          </pre>
     * \param   Float a a in (a,b)
     * \param   Float b b in (a,b)
     * \return  the case
     */
    inline int _which_case_4(Vt a, Vt b) const{
        const int cVOF_whichcase4[2][2] = { { 1, 4 }, { 2, 3 } };
        return cVOF_whichcase4[a >= 0 ? 0 : 1][b >= 0 ? 0 : 1];
    }

    /**
     * \brief   Calculate the direction of the line normal vector
     *          <pre>
     *                          ^y
     *                +         |         +
     *                  +    3  |  2    +
     *                    +     |     +
     *                      +   |   +
     *                  4     + | +      1
     *          --+------+------+------+------+--> x
     *                        + | +
     *                  5   +   |   +    8
     *                    +     |     +
     *                  +     6 | 7     +
     *                +         |         +
     *          </pre>
     * \param   Float a a in ax+by=C
     * \param   Float b b in ax+by=C
     * \return  the case
     */
    inline static int _WhichCase8(Vt a, Vt b) {
        static const int cVOF_whichcase8[2][2][2] =
        { { { 2, 1 }, { 7, 8 } },
          { { 3, 4 }, { 6, 5 } } };
        return cVOF_whichcase8[a >= 0 ? 0 : 1][b >= 0 ? 0 : 1][
                std::abs(b) >= std::abs(a) ? 0 : 1];
    }
};

}

#endif
