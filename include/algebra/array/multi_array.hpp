/************************
 //  \file   multi_array.hpp
 //  \brief
 //
 //  \author czhou
 //  \date   20 janv. 2018
 ***********************/
#ifndef MULTI_ARRAY_H_
#define MULTI_ARRAY_H_

#include <iostream>
#include <assert.h>

#include "algebra/algebra_define.hpp"
#include <array>
#include "array_list.hpp"

namespace carpio {

template<typename T, St DIM>
class MultiArray_ {
  public:
    static const St Dim = DIM;
    // type definitions===================
    typedef T value_t;
    typedef MultiArray_<value_t, Dim> Self;
    typedef MultiArray_<value_t, Dim>* pSelf;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    //typedef St difference_type;

    typedef typename ArrayListT<value_t>::iterator iterator;
    typedef typename ArrayListT<value_t>::const_iterator const_iterator;
    //static const int DIM = Dim;
  private:
    std::array<size_type, Dim> m_len;
    ArrayListT<value_t> m_mp;
  public:
    //constructor==========================
    MultiArray_(){
    	m_len.fill(0);
    }
    MultiArray_(const Self& a){
    	this->m_len = a.m_len;
    	this->m_mp = a.m_mp;
    }
    MultiArray_(St iLen, St jLen= 0, St kLen= 0){
		this->m_len[0] = iLen;
		if (Dim >= 2) {
			ASSERT(iLen > 0 && jLen > 0);
			this->m_len[1] = jLen;
			this->m_mp.reconstruct(iLen * jLen);
		}
		if (Dim >= 3) {
			ASSERT(iLen > 0 && jLen > 0 && kLen > 0);
			this->m_len[2] = kLen;
			this->m_mp.reconstruct(iLen * jLen * kLen);
		}
    }

    void reconstruct(St iLen, St jLen = 0, St kLen= 0){
		St Len = 0;
		this->m_len[0] = iLen;
		if (Dim == 1) {
			Len = iLen;
		}
		if (Dim >= 2) {
			ASSERT(iLen > 0 && jLen > 0);
			this->m_len[1] = jLen;
			Len = iLen * jLen;
		}
		if (Dim >= 3) {
			ASSERT(iLen > 0 && jLen > 0 && kLen > 0);
			this->m_len[2] = kLen;
			Len = iLen * jLen * kLen;
		}
		this->m_mp.reconstruct(Len);
    }
    //=============================================
    MultiArray_<T, DIM>& operator=(const MultiArray_<T, DIM>& a){
		if (this == &a) {
			return *this;
		}
		this->m_len = a.m_len;
		this->m_mp = a.m_mp;
		return *this;
    }
    //=============================================
    ~MultiArray_() {
    }

    //Capacity=====================================
    St size() const {
        return m_mp.size();
    }
    St size_i() const {
        return m_len[0];
    }
    St size_j() const {
        return Dim >= 2 ? m_len[1] : 0;
    }
    St size_k() const {
        return Dim >= 3 ? m_len[2] : 0;
    }
    bool empty() const {
        return m_mp.empty();
    }
    /*
     *  iterator
     */
    iterator begin() {
        return m_mp.begin();
    }
    const_iterator begin() const {
        return m_mp.begin();
    }
    iterator end() {
        return m_mp.end();
    }
    const_iterator end() const {
        return m_mp.end();
    }
    //Element access===============================
    St to_1d_idx(St i, St j = 0, St k = 0) const{
		ASSERT(i < this->m_len[0]);
		if (Dim >= 2)
			ASSERT(j < this->m_len[1]);
		if (Dim >= 3)
			ASSERT(k < this->m_len[2]);
		std::array<size_type, Dim> inp;
		inp[0] = i;
		if (Dim >= 2) {
			inp[1] = j;
		}
		if (Dim >= 3) {
			inp[2] = k;
		}
		size_type idx = 0;
		for (size_type ii = 0; ii < Dim; ii++) {
			size_type b = 1;
			for (size_type jj = ii + 1; jj < Dim; jj++) {
				b *= m_len[jj];
			}
			idx += b * inp[ii];
		}
		return idx;
    }



    reference       at(St i, St j= 0, St k= 0){
    	 St idx = to_1d_idx(i, j, k);
    	 return m_mp[idx];
    }
    const_reference at(St i, St j = 0, St k = 0) const{
    	St idx = to_1d_idx(i, j, k);
    	return m_mp[idx];
    }

    reference       operator()(St i, St j = 0, St k = 0){
       	return at(i, j, k);
    }
    const_reference operator()(St i, St j = 0, St k = 0) const{
      	return at(i, j, k);
    }

    reference       at_1d(St i){        return m_mp[i];}
    const_reference at_1d(St i) const{  return m_mp[i];}

    T    get(St i, St j = 0,  St k = 0){
    	return at(i, j, k);
    }
    void set(const T& value, St i, St j = 0, St k = 0){
    	this->at(i, j, k) = value;
    }

    void assign(const T& value){
    	 m_mp.assign(value);
    }
    //element access===============================

    T* getpValue(St i, St = 0, St = 0);  //not good

    inline bool check_idx(St dim, St idx) const {
        ASSERT(dim < Dim);
        if (idx >= 0 && idx < m_len[dim]) {
            return true;
        } else {
            return false;
        }
    }
    inline bool check_idx_ijk(St i, St j, St k) const {
        return check_idx(0, i) && ((Dim >= 2) ? check_idx(1, j) : true)
               && ((Dim >= 3) ? check_idx(2, k) : true);
    }

    inline St count_equal(const T& nd) const { //overload ==
        return m_mp.count_equal(nd);
    }
};


}

#endif /* MULTI_ARRAY_HPP */
