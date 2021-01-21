#ifndef _BASE_FIELD_HPP
#define _BASE_FIELD_HPP

#include <iostream>

#include "type_define.hpp"


namespace carpio{


struct BaseFieldTag{};


template<St DIM>
class BaseGrid_{
public:
    static const St Dim = DIM;
public:
    BaseGrid_(){}

    virtual ~BaseGrid_(){}
};
template<St DIM>
class BaseGhost_{
public:
    static const St Dim = DIM;
public:
    BaseGhost_(){};

    virtual ~BaseGhost_(){};
};
template<St DIM>
class BaseOrder_{
public:
    static const St Dim = DIM;
public:
    BaseOrder_(){};

    virtual ~BaseOrder_(){};
};
template<St DIM, class VT, class GRID, class GHOST, class ORDER>
class BaseField_ {
public:
    static const St Dim = DIM;

    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;
    typedef VT    ValueType;

    typedef BaseFieldTag Tag;
public:
    BaseField_(){}

    virtual ~BaseField_(){}

    virtual Grid&  grid(){return Grid();};
    virtual Ghost& ghost(){return Ghost();};
    virtual Order& order(){return Order();};

};


}

#endif
