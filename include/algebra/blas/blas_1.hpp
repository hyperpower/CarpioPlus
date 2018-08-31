#ifndef _BLAS_1_HPP_
#define _BLAS_1_HPP_

#include "type_define.hpp"

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include "algebra/array/array_list.hpp"


namespace carpio {

//Copies vector to another vector.
template<typename ST, typename VT>
int Copy(ST n, const VT * src, VT* dst);

//Computes the sum of magnitudes of the vector elements.
template<typename ST, typename VT>
int ASum(ST n, const VT * sx, ST incx = 1);

template<typename ST, typename VT>
int ASum(ST n, const VT * sx, ST incx){
	VT asum = 0.0e0;
	if (n < 0 || incx <= 0) {
		return asum;
	}
	if (incx == 1) {
		//increase = 1
		int LN = 6;
		int m = (n - 1) % LN;
		for (int i = 0; i <= m; ++i) {
			asum = asum + std::abs(sx[i]);
		}
		if (n <= LN) {
			return asum;
		}
		int mp1 = m + 1;
		for (int i = mp1; i < n; i += LN) {  //unrolled loop
			asum = asum
					+ std::abs(sx[i])
					+ std::abs(sx[i + 1])
			        + std::abs(sx[i + 2])
					+ std::abs(sx[i + 3])
			        + std::abs(sx[i + 4])
			        + std::abs(sx[i + 5]);
		}
	} else {
		int nincx = n * incx;
		for (int i = 0; i < nincx; i += incx) {
			asum = asum + std::abs(sx[i]);
		}
	}
	return asum;
}


template<typename ST, typename VT>
int Copy(ST n, const VT * src, VT* dst) {
    //make sure: the length of a and b is equal
    //           and n>0
    ST LN = 7;
    ST m = (n - 1) % LN;
    for (ST i = 0; i <= m; ++i) {
        dst[i] = src[i];
    }
    if (n <= LN) {
        return 1;
    }
    ST mp1 = m + 1;
    for (ST i = mp1; i < n; i += LN) {
        dst[i    ] = src[i    ];
        dst[i + 1] = src[i + 1];
        dst[i + 2] = src[i + 2];
        dst[i + 3] = src[i + 3];
        dst[i + 4] = src[i + 4];
        dst[i + 5] = src[i + 5];
        dst[i + 6] = src[i + 6];
    }
    return 2;
}

/**
 * \brief   NRM2 returns the euclidean norm of a vector via the function
 *          name, so that
 *          SNRM2 := sqrt( x'*x ).
 *
 * \param   array
 *
 * \return  int
 */
template<class ST, class VT>
VT Nrm2(ST n,           //size of the array, sx.size==sy.size
		const VT* x,    //
		ST incx) {
	VT zero = 0.0e+0;
	VT norm = zero;
	if (n <= 0 || incx <= 0) {
		return norm;
	} else if (n == 1) {
		norm = std::abs(x[0]);
	} else {
		for (int i = 0; i < (n - 1) * incx; i += incx) {
			if (x[i] != zero) {
				norm += x[i] * x[i];
			}
		}
		norm = std::sqrt(norm);
	}
	return norm;
}

template<typename VT>
VT Nrm(ArrayListV<VT>& arr){

}

}

#endif
