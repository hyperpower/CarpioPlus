#ifndef _S_OPERATION_HPP
#define _S_OPERATION_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/order/xyz.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"

#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/bc_value.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "sinterpolate.hpp"
#include "svalue.hpp"

#include "soperation_define.hpp"
#include "slaplacian.hpp"
#include "slaplacian_cut.hpp"

#ifdef OPENMP
#include <omp.h>
#endif

#include <array>

namespace carpio{




}
#endif
