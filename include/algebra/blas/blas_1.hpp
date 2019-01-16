#ifndef _BLAS_1_HPP_
#define _BLAS_1_HPP_

#include "type_define.hpp"

#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <cmath>


namespace carpio {

//Forward Declaration
template <class VT> class ArrayListV_;

//Copies vector to another vector.
template<typename ST, typename VT>
int Copy(ST n, const VT * src, VT* dst);

//Computes the sum of magnitudes of the vector elements.
template<typename ST, typename VT>
int ASum(ST n, const VT * sx, ST incx = 1);

template<class VT>
int Rotg(VT& sa, VT& sb, VT& c, VT& s);

template<class VT, class ST>
VT Amax(ST n, const VT* sx, ST incx);

template<class ST, class VT>
VT Nrm2(ST n, const VT* x, ST incx);

template<class VT, class VT2, class ST>
VT Nrmp(ST n, const VT* x, VT2 p, ST incx);


// High level functions
// work with ArrayListV
template<class VT> VT Nrm1(const ArrayListV_<VT>& ax);
template<class VT> VT Nrm2(const ArrayListV_<VT>& arr);
template<class VT> VT Nrmp(const ArrayListV_<VT>& arr, float p);


// -----------------------------------------------------
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
#ifdef OPENMP
# pragma omp for
    for (ST i = 0; i < n; ++i) {
        dst[i] = src[i];
    }
#else
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
#endif
}

//construct givens plane rotation.
template<class VT>
int Rotg(VT& sa, //
		 VT& sb, //
		 VT& c,  //
		 VT& s)  //
		{
	VT r,roe,scale,z;
	roe = sb;
	if (std::abs(sa) > std::abs(sb)) {
		roe = sa;
	}
	scale = std::abs(sa) + std::abs(sb);
	if (scale == 0.0) {
		c = 1.0;
		s = 0.0;
		r = 0.0;
		z = 0.0;
	} else {
		r = scale
				* std::sqrt(
						(sa / scale) * (sa / scale)
								+ (sb / scale) * (sb / scale));
		r = sign2(roe) * r;
		c = sa / r;
		s = sb / r;
		z = 1.0;
		if (std::abs(sa) > std::abs(sb)) {
			z = s;
		}
		if (std::abs(sb) > std::abs(sa) && c != 0.0) {
			z = 1.0 / c;
		}
	}
	sa = r;
	sb = z;
	return 1;
}

/**
 * \brief   max(abs())
 *
 * \param   array
 *
 * \return  int
 */
template<class VT, class ST>
VT Amax(      ST  n,       //size of the array, sx.size
		const VT*  sx,     //
		      ST  incx) {
	VT max = 0.0e0;
	if (n < 0 || incx <= 0) {
		return max;
	}
	if (incx == 1) {
		//increase = 1
		max = std::abs(sx[0]);
		for (ST i = 1; i < n; ++i) {
			if (std::abs(sx[i]) > max) {
				max = std::abs(sx[i]);
			}
		}
	} else {
		int nincx = n * incx;
		max = std::abs(sx[0]);
		for (ST i = 0; i < nincx; i += incx) {
			if (std::abs(sx[i]) > max) {
				max = std::abs(sx[i]);
			}
		}
	}
	return max;
}

/**
 * \brief   NRM returns the p norm of a vector via the function
 *          name, so that
 *          SNRM2 := ( x ).
 *
 * \param   array
 *
 * \return  int
 */
template<class VT, class VT2, class ST>
VT Nrmp(         ST  n,    //size of the array, sx.size==sy.size
		   const VT* x,    //
		         VT2 p,    //
		         ST  incx) {
	if (p == 1.0) {
		return Asum(n, x, incx);
	}
	if (p == 2.0) {
		return Nrm2(n, x, incx);
	}
	VT zero = 0.0e+0;
	VT norm = zero;
	if (n <= 0 || incx <= 0) {
		return norm;
	} else if (n == 1) {
		norm = std::abs(x[0]);
	} else {
		for (int i = 0; i < (n - 1) * incx; i += incx) {
			if (x[i] != zero) {
				norm += std::pow(std::abs(x[i]), p);
			}
		}
		norm = std::pow(norm, 1.0 / double(p));
	}
	return norm;
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

template<class VT>
VT Nrm1(const ArrayListV_<VT>& ax) {
	typedef typename ArrayListV_<VT>::size_type St;
	return Asum(ax.size(), ax.data(), St(1));
}

template<class VT>
VT Nrm2(const ArrayListV_<VT>& arr){
	typedef typename ArrayListV_<VT>::size_type St;
	return Nrm2(arr.size(), arr.data(), St(1));
}

template<class VT>
VT Nrmp(const ArrayListV_<VT>& arr, float p){
	typedef typename ArrayListV_<VT>::size_type St;
	return Nrmp(arr.size(), arr.data(), p, St(1));
}




}

#endif
