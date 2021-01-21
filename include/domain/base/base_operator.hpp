#ifndef _BASE_OPERATOR_HPP
#define _BASE_OPERATOR_HPP

#include "base_field.hpp"

namespace carpio{

template<St DIM, class VT, class GRID, class GHOST, class ORDER>
class LaplacianImplement_{
public:
    LaplacianImplement_(){
        std::cout << "Laplacian" << std::endl;
    };

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

    template<class OTHER>
    void set(const OTHER& other){}

    // void set(const int& other){std::cout << "int" << std::endl;}
        
};
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
class LaplacianImplementBase_{
public:
    LaplacianImplementBase_(){
    }
    ~LaplacianImplementBase_(){}

    void set(const int& other){
        std::cout << "LaplacianImplementBase int" << std::endl;
    }
};



template<St DIM, class VT, class GHOST, class ORDER>
class LaplacianImplement_<DIM, VT, BaseGrid_<DIM>, GHOST, ORDER> :public LaplacianImplementBase_<DIM, VT, BaseGrid_<DIM>, GHOST, ORDER>{
public:
    LaplacianImplement_(){
        std::cout << "Laplacian - partial BaseGrid" << std::endl;
    };

    template<class BDYIDX>
    void set_boundary_index(const BDYIDX& bi){};

    // template<class OTHER>
    // void set(const OTHER& other){}

    // void set(const int& other){
        // std::cout << "LaplacianImplement int" << std::endl;
    // }
        
};

template<class FIELD, class BDYIDX, class OTHER>
FIELD Laplacian(const FIELD& field, const BDYIDX bi, const OTHER& other){
    LaplacianImplement_<FIELD::Dim,
                        FIELD::ValueType,
                        typename FIELD::Grid, 
                        typename FIELD::Ghost,
                        typename FIELD::Order> imp;
    imp.set(other);
    imp.set_boundary_index(bi);

    std::cout << "Laplacian" << std::endl;
    return field;
}
template<class FIELD, class BDYIDX>
FIELD Laplacian(const FIELD& field, const BDYIDX bi){
    std::cout << "Laplacian" << std::endl;
    return field;
}

}

#endif
