#ifndef _S_GNUPLOT_ACTOR_MAKER_HPP
#define _S_GNUPLOT_ACTOR_MAKER_HPP

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/operation/soperation.hpp"
#include "io/gnuplot.hpp"


namespace carpio{


using namespace GnuplotActor;

template<St DIM, class GRID, class GHOST, class ORDER>
class SGnuplotActorMaker_ {
public:
    static const St Dim = DIM;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;

    typedef SField_<Dim, Grid, Ghost, Order> Field;
    typedef SField_<1, Grid, Ghost, Order> Field1;
    typedef SField_<2, Grid, Ghost, Order> Field2;
    typedef SField_<3, Grid, Ghost, Order> Field3;


public:
    SGnuplotActorMaker_(){};
    
    spActor wire_frame(
                const Grid& grid,
                const Vt&    tik  = 0.1,
                int color_idx     = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        // short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
        if(Dim == 1){
            Vt xs, xe;
            for (St i = 0; i < grid.n(_X_); i++) {
                typename Grid::Index index(i);
                auto p = grid.f(_X_, _M_, index);
                actor->data().push_back(
                        ToString(p.value(_X_), -0.0, c, " "));
                actor->data().push_back(
                        ToString(p.value(_X_),  tik, c, " "));
                actor->data().push_back("");
                if(i == 0){
                    xs = p.value(_X_);
                }

            }
            typename Grid::Index index(grid.n(_X_) - 1);
            auto p = grid.f(_X_, _P_, index);
            actor->data().push_back(
                    ToString(p.value(_X_), -0.0, c, " "));
            actor->data().push_back(
                    ToString(p.value(_X_),  tik, c, " "));
            actor->data().push_back("");
            xe = p.value(_X_);

            actor->data().push_back(ToString(xs, 0.0, c, " "));
            actor->data().push_back(ToString(xe, 0.0, c, " "));
        }

        return actor;
    }

    spActor center_points(const Grid& grid, int color_idx = -1){
        spActor actor    = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with points lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        if(Dim == 1){
            for (St i = 0; i < grid.n(_X_); i++) {
                typename Grid::Index index(i);
                auto p = grid.c(index);
                actor->data().push_back(
                        ToString(p.value(_X_), 0.0, c, " "));
                actor->data().push_back("");
            }
        }
        return actor;
    }



    spActor lines_points(const Field1& s, int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (auto& index : s.order()) {
            auto x = s.grid().c_(_X_, index);
            auto v = s(index);
            actor->data().push_back(ToString(x, v, c, " "));
        }
        return actor;
    }









};


}

#endif