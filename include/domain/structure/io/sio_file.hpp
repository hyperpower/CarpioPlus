#ifndef _S_IO_FILE_HPP
#define _S_IO_FILE_HPP

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/field/sfield.hpp"
#include "io/io_define.hpp"
#include "domain/boundary/boundary_index.hpp"

namespace carpio{

template<St DIM, class GRID, class GHOST, class ORDER>
class SIOFile_ {
public:
    static const St Dim = DIM;
    typedef ArrayListT_<Vt> Arr;
    typedef Point_<Vt, Dim> Poi;
    typedef SIndex_<Dim> Index;
    typedef BoundaryIndex BI;

    typedef SField_<DIM, GRID, GHOST, ORDER> Field;
    typedef SField_<2, GRID, GHOST, ORDER>   Field2;
public:


    static void OutputField(const std::string& filename, const Field& data) {
        auto& grid = data.grid();
        auto n = grid.n();
        // Open a file
        TextFile txtf(filename);
        // format first line
        txtf.add_line(tfm::format("## SIZE : %d", data.size()));
        txtf.add_line(tfm::format("## DIM : %d", DIM));
        txtf.add_line(tfm::format("## NX : %d", n(_X_)));
        if (DIM >= 2) {
            txtf.add_line(tfm::format("## NY : %d", n(_Y_)));
        }
        if (DIM == 3) {
            txtf.add_line(tfm::format("## NZ : %d", n(_Z_)));
        }
        //
        txtf.add_line(tfm::format("## COLUMN_NAME : %s, %s, %s, %s, %s",
                                                   "ORDER", "X", "Y", "Z", "VALUE"));
        for(auto& index : data.order()){
            std::stringstream ss;
            tfm::format(ss, "%d, ", data.order().get_order(index));
            for (St d = 0; d < 3; ++d) { // always 3 d
                Vt cor = grid.c_(d, index);
                tfm::format(ss, "%10.6e, ", cor);
            }
            Vt val = data(index);
            tfm::format(ss, "%10.6e", val);
            txtf.add_line(ss.str());
        }
        txtf.write();
    }

    static void OutputFieldAxisAlignSection(const std::string& filename,
                                            const Field2& data,
                                            Axes a, Vt value) {
        auto& grid = data.grid();
        auto n = grid.n();
        // Open a file
        TextFile txtf(filename);
        // format first line
        txtf.add_line(tfm::format("## SIZE : %d", data.size()));
        txtf.add_line(tfm::format("## DIM : %d", DIM));
        txtf.add_line(tfm::format("## NX : %d", n(_X_)));
        txtf.add_line(tfm::format("## NY : %d", n(_Y_)));
        //
        txtf.add_line(
                tfm::format("## COLUMN_NAME : %s, %s, %s, %s, %s", "ORDER", "X",
                        "Y", "Z", "VALUE"));
        for (auto& index : data.order()) {
            std::stringstream ss;
            auto fpm = grid.f(a, _M_, index);
            auto fpp = grid.f(a, _P_, index);
            if (IsInRange(fpm[a], value, fpp[a], _co_)) {
                tfm::format(ss, "%d, ", data.order().get_order(index));
                for (St d = 0; d < 3; ++d) { // always 3 d
                    Vt cor = grid.c_(d, index);
                    tfm::format(ss, "%10.6e, ", cor);
                }
                Vt val = data(index);
                tfm::format(ss, "%10.6e", val);
                txtf.add_line(ss.str());
            }
        }
        txtf.write();
    }

};

}

#endif
