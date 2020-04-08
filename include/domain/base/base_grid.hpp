#ifndef _BASE_GRID_HPP
#define _BASE_GRID_HPP

#include "domain/domain_define.hpp"
#include <array>

namespace carpio{

template<St DIM, class INDEX>
class BaseGrid_ {
public:
    static const St Dim = DIM;
public:
    BaseGrid_(){}

    virtual ~BaseGrid_(){}

    virtual std::string type_name() const{
        return "BaseGrid";
    };

};



}

#endif
