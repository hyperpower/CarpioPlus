/************************
 //  \file   ArrayT.h
 //  \brief
 //
 //  \author zhou
 //  \date   23 janv. 2014
 ***********************/
#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

#include "type_define.hpp"
#include "algebra/blas/blas_0.hpp"
#include "algebra/blas/blas_1.hpp"


#include <stddef.h>
#include <stdio.h>
#include <iostream>

namespace carpio {

/**
 * @class Array
 *
 * @brief A class designed for dynamic array.
 *        It's the basic data structure class.
 */
template<typename T>
class ArrayListT {
public:
    // type definitions
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef St size_type;
    typedef St difference_type;

protected:
    St m_Len;
    T *m_p;    //!< The heap for the real data.
public:
    //constructor==================================
    ArrayListT();
    ArrayListT(const ArrayListT<T>& a);
    ArrayListT(size_type Len);
    void reconstruct(size_type Len);
    ArrayListT(size_type Len, const T& nd);
    ArrayListT(T *nd, size_type Len);
    //=============================================
    virtual ~ArrayListT();
    //=============================================
    T* data() {
        return m_p;
    }
    const T* data() const {
        return m_p;
    }
    //=============================================
    const_reference operator[](size_type index) const;  //overload []
    reference operator[](size_type index);
    const_reference operator()(size_type index) const;
    reference operator()(size_type index);
    const_reference at(size_type index) const;
    reference at(size_type index);
    //operator=====================================
    ArrayListT<T>& operator=(const ArrayListT<T> &a);
    // iterator support============================
    iterator begin() {
        return m_p;
    }
    const_iterator begin() const {
        return m_p;
    }
    iterator end() {
        return m_p + m_Len;
    }
    const_iterator end() const {
        return m_p + m_Len;
    }

    size_type size() const;  //!< get length of the array
    T get(size_type i) const;
    void set(size_type i, const T& value);
    void assign(const T& nd);

    // front() and back()
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void swap(size_type i1, size_type i2);
    void reverse();
    void push_back(const T& nd);
    void pop_back();
    void erase(size_type i);
    virtual void resize(const size_type& new_size);

    bool empty() const;
    bool non_empty() const;
    inline bool check_idx(size_type) const;

    bool has(const T& nd) const;              //overload ==
    size_type find(const T& nd) const;        //overload ==
    size_type count_equal(const T& nd) const; //overload ==
};

template<typename T>
ArrayListT<T>::ArrayListT() {
    m_Len = 0;
    m_p = nullptr;
}

template<typename T>
ArrayListT<T>::ArrayListT(const ArrayListT<T>& a) {
    m_Len = a.size();
    m_p = new T[m_Len];
    //unrolled loop
    Copy(m_Len, a.m_p, this->m_p);
}

template<typename T>
ArrayListT<T>::ArrayListT(size_type Len) {
    m_Len = Len;
    m_p = new T[m_Len];
}

template<typename T>
void ArrayListT<T>::reconstruct(size_type Len) {
    if (Len == 0 && m_Len == 0) {
        m_p = nullptr;
        return;
    }
    if (Len == 0 && nullptr != m_p) {
        delete[] m_p;
        m_p = nullptr;
        return;
    }
    ASSERT(Len > 0);
    if (nullptr != m_p)
        delete[] m_p;
    m_Len = Len;
    m_p = new T[m_Len];
}

template<typename T>
ArrayListT<T>::ArrayListT(size_type Len,
                          const T& nd) {
    m_Len = Len;
    m_p = new T[Len];
    for (size_type i = 0; i < m_Len; i++) {
        m_p[i] = nd;
    }
}

template<typename T>
ArrayListT<T>::ArrayListT(T *nd,
                          size_type Len) {
    m_Len = Len;
    m_p = new T[Len];
    for (size_type i = 0; i < m_Len; i++) {
        m_p[i] = nd[i];
    }
}

template<typename T>
ArrayListT<T>::~ArrayListT() {
    delete[] m_p;
}

template<typename T>
const T& ArrayListT<T>::operator[](size_type index) const {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
T& ArrayListT<T>::operator[](size_type index) {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
const T& ArrayListT<T>::operator()(size_type index) const {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
T& ArrayListT<T>::operator()(size_type index) {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}
template<typename T>
const T& ArrayListT<T>::at(size_type index) const {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}
template<typename T>
T& ArrayListT<T>::at(size_type index) {
    ASSERT(index >= 0 && index < m_Len);
    return m_p[index];
}

template<typename T>
ArrayListT<T>& ArrayListT<T>::operator=(const ArrayListT<T> &a) {
    if (this == &a) {
        return *this;
    }
    if (a.m_Len <= 0) {
        this->resize(0);
        return *this;
    }
    if (m_Len == a.size()) {
        //unrolled loop
        Copy(m_Len, a.m_p, m_p);
    } else {
        delete[] this->m_p;
        m_Len = a.size();
        m_p = new T[m_Len];
        Copy(m_Len, a.m_p, m_p);
    }
    return *this;
}

template<typename T>
void ArrayListT<T>::set(size_type i, const T& value) {
    ASSERT(i >= 0 && i < m_Len);
    m_p[i] = value;
}

template<typename T>
void ArrayListT<T>::assign(const T& nd) {
    for (size_type i = 0; i < m_Len; i++) {
        m_p[i] = nd;
    }
}
template<typename T>
St ArrayListT<T>::size() const {
    return m_Len;
}
template<typename T>
T& ArrayListT<T>::front() {
    ASSERT(m_Len > 0);
    return m_p[0];
}
template<typename T>
const T& ArrayListT<T>::front() const {
    ASSERT(m_Len > 0);
    return m_p[0];
}
template<typename T>
T& ArrayListT<T>::back() {
    ASSERT(m_Len > 0);
    return m_p[m_Len - 1];
}
template<typename T>
const T& ArrayListT<T>::back() const {
    ASSERT(m_Len > 0);
    return m_p[m_Len - 1];
}

template<typename T>
T ArrayListT<T>::get(size_type i) const {
    ASSERT(i >= 0 && i < m_Len);
    return m_p[i];
}

template<typename T>
void ArrayListT<T>::swap(size_type i1, size_type i2) {
    ASSERT(i1 >= 0 && i1 < m_Len && i2 >= 0 && i2 < m_Len);
    if (i1 == i2) {
        return;
    }
    T tmp;
    tmp = m_p[i1];
    m_p[i1] = m_p[i2];
    m_p[i2] = tmp;
}

template<typename T>
void ArrayListT<T>::reverse() {
    if (empty()) {
        return;
    }
    T tmp;
    for (int i1 = 0,i2 = size() - 1; i1 < i2; i1++, i2--) {
        tmp = m_p[i1];
        m_p[i1] = m_p[i2];
        m_p[i2] = tmp;
    }
}

template<typename T>
void ArrayListT<T>::push_back(const T& nd) {
    if (m_p == nullptr) {
        m_Len = 1;
        m_p = new T[1];
        m_p[0] = nd;
    } else {
        T *tmp = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len += 1;
        m_p = new T[m_Len];
        for (size_type i = 0; i < m_Len - 1; i++) {
            m_p[i] = tmp[i];
        }
        m_p[m_Len - 1] = nd;
        delete[] tmp;
    }
}

template<typename T>
void ArrayListT<T>::pop_back() {
    if (m_p == NULL) {
        return;
    } else if (m_Len == 1) {
        m_Len = 0;
        delete[] m_p;
        m_p = NULL;
    } else {
        T *tmp = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len -= 1;
        m_p = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            m_p[i] = tmp[i];
        }
        delete[] tmp;
    }
}

template<typename T>
void ArrayListT<T>::erase(size_type idx) {
    ASSERT(idx >= 0 && idx < m_Len);
    if (m_Len == 1) {
        m_Len = 0;
        delete[] m_p;
        m_p = NULL;
    } else {
        T *tmp = new T[m_Len];
        for (size_type i = 0; i < m_Len; i++) {
            if (i != idx)
                tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len--;
        m_p = new T[m_Len];
        for (size_type i = 0; i < idx; i++) {
            m_p[i] = tmp[i];
        }
        for (size_type i = idx; i < m_Len; i++) {
            m_p[i] = tmp[i + 1];
        }
        delete[] tmp;
    }
}

template<typename T>
void ArrayListT<T>::resize(const size_type& new_size) {
    ASSERT(new_size >= 0);
    if (new_size == m_Len) {
        return;
    } else if (empty()) {
        reconstruct(new_size);
    } else {
        T *tmp = new T[new_size];
        for (size_type i = 0; i < m_Len && i < new_size; i++) {
            tmp[i] = m_p[i];
        }
        delete[] m_p;
        m_Len = new_size;
        m_p = tmp;
    }
}

template<typename T>
bool ArrayListT<T>::has(const T& nd) const {
    if (m_p == NULL) {
        return false;
    } else {
        for (size_type i = 0; i < m_Len; i++) {
            if (nd == m_p[i]) {
                return true;
            }
        }
        return false;
    }
}

template<typename T>
St ArrayListT<T>::find(const T& nd) const {
    if (m_p == NULL) {
        return -1;
    } else {
        for (size_type i = 0; i < m_Len; i++) {
            if (nd == m_p[i]) {
                return i;
            }
        }
        return -1;
    }
}
template<typename T>
St ArrayListT<T>::count_equal(const T& nd) const { //overload ==
    size_type res = 0;
    if (m_p == NULL) {
        return 0;
    } else {
        for (size_type i = 0; i < m_Len; i++) {
            if (nd == m_p[i]) {
                res++;
            }
        }
    }
    return res;
}
template<typename T>
bool ArrayListT<T>::empty() const {
    if (m_p == NULL && m_Len == 0) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool ArrayListT<T>::non_empty() const {
    return !empty();
}
template<typename T>
inline bool ArrayListT<T>::check_idx(ArrayListT<T>::size_type idx) const {
    if (idx >= 0 && idx < m_Len) {
        return true;
    } else {
        return false;
    }

}
//end of Class arrayListT
//=========================================================
//=========================================================
//Function out of class====================================

//=========================================================

template<typename V>
class ArrayListV: public ArrayListT<V> {
public:
    // type definitions
    typedef V value_type;
    typedef V* pointer;
    typedef const V* const_pointer;
    typedef V& reference;
    typedef const V& const_reference;
    typedef St size_type;
    typedef St difference_type;
    typedef ArrayListV<V>        Self;
    typedef ArrayListV<V>&       ref_Self;
    typedef const ArrayListV<V>& const_ref_Self;
    //constructor==================================
    ArrayListV();
    ArrayListV(size_type Len);
    ArrayListV(size_type Len, const V& nd);
    void reconstruct(size_type Len);
    //arrayListV(V *nd, size_type Len);
    //opertator====================================
    ref_Self operator+=(const V& a);
    ref_Self operator-=(const V& a);
    ref_Self operator*=(const V& a);
    ref_Self operator/=(const V& a);
    ref_Self operator+=(const Self& a);
    ref_Self operator-=(const Self& a);
    ref_Self operator*=(const Self& a);
    ref_Self operator/=(const Self& a);

        Self operator-() const;
        Self operator*(const Self &a);
        Self operator*(const V &a);
        Self operator/(const Self &a);
        Self operator/(const V &a);

    //other functions==============================
    V sum() const;
    V min() const;
    V max() const;
    size_type findMinIdx() const;
    size_type findMaxIdx() const;
    //fill ----------------------------------------
    void assign_forward(const V& be, const V& step);
    void assign_backward(const V& be, const V& step);
    void ones();
    void zeros();
    //count ---------------------------------------
    size_type count_equal(const V &a) const;

    void show() const;
};
template<typename V>
ArrayListV<V> operator+(ArrayListV<V> x, const ArrayListV<V> &y);
template<typename V>
ArrayListV<V> operator+(ArrayListV<V> x, const V &a);
template<typename V>
ArrayListV<V> operator+(const V &a, ArrayListV<V> x);

template<typename V>
ArrayListV<V> operator-(ArrayListV<V> x, const ArrayListV<V> &y);
template<typename V>
ArrayListV<V> operator-(ArrayListV<V> x, const V &a);
template<typename V>
ArrayListV<V> operator-(const V &a, const ArrayListV<V>& x);

template<typename V>
ArrayListV<V> operator*(const V &a, const ArrayListV<V> &x);
template<typename V>
ArrayListV<V> operator*(const ArrayListV<V> &x, const V &a);
template<typename V>
ArrayListV<V> operator-(const ArrayListV<V> &a, const ArrayListV<V> &b);
//=================================================
/*
 * Return evenly spaced numbers over a specified interval.
 * Returns num evenly spaced samples, calculated over the interval [start, stop].
 */
template<typename V>
ArrayListV<V> Linspace(V start, V stop, St n) {
    if (n == 0) {
        return ArrayListV<V>();
    }
    ArrayListV<V> arr(n + 1);
    V d = (stop - start) / n;
    for (St i = 0; i < n + 1; ++i) {
        arr[i] = start + d * i;
    }
    return arr;
}

template<typename V>
ArrayListV<V>::ArrayListV() :
    ArrayListT<V>() {
}
template<typename V>
ArrayListV<V>::ArrayListV(size_type Len) :
    ArrayListT<V>(Len) {
    this->assign(0);
}
template<typename V>
ArrayListV<V>::ArrayListV(size_type Len, const V& nd) :
    ArrayListT<V>(Len, nd) {
}

template<typename V>
void ArrayListV<V>::reconstruct(size_type Len) {
    if (Len == 0 && this->m_Len == 0) {
        return;
    }
    if (Len == 0 && NULL != this->m_p) {
        delete[] this->m_p;
        return;
    }
    ASSERT(Len > 0);
    if (nullptr != this->m_p)
        delete[] this->m_p;
    this->m_Len = Len;
    this->m_p = new V[this->m_Len];
    this->assign(0);
}

template<typename V>
ArrayListV<V> operator+(ArrayListV<V> x, const ArrayListV<V> &y){
	ASSERT(x.size() == y.size());
	x += y;
	return x;
}
template<typename V>
ArrayListV<V> operator+(ArrayListV<V> x, const V &a){
	x += a;
	return x;
}
template<typename V>
ArrayListV<V> operator+(const V &a, ArrayListV<V> x){
	x += a;
	return x;
}

template<typename V>
ArrayListV<V> operator-(ArrayListV<V> x, const ArrayListV<V> &y){
	ASSERT(x.size() == y.size());
	x -= y;
	return x;
}
template<typename V>
ArrayListV<V> operator-(ArrayListV<V> x, const V &a){
	x -= a;
	return x;
}
template<typename V>
ArrayListV<V> operator-(const V &a, const ArrayListV<V>& x){
	ArrayListV<V> res(x.size());
	res.assign(a);
	res -= x;
	return res;
}
//template<typename V>
//ArrayListV<V> ArrayListV<V>::operator+(const ArrayListV<V> &a) {
//    ASSERT(a.size() == this->size());
//    ArrayListV<V> res(this->m_Len);
//    Copy_(a.size(), this->m_p, res.m_p);
//    AddEqual(a.size(), a.m_p, res.m_p);
//    return res;
//}
template<typename V>
ArrayListV<V> ArrayListV<V>::operator-() const{
	ArrayListV<V> tmp(this->m_Len);
	for (size_type i = 0; i < this->m_Len; i++) {
		tmp[i] = -(this->m_p[i]);
	}
	return tmp;
}

template<typename V>
ArrayListV<V> ArrayListV<V>::operator*(const ArrayListV<V> &a) {
    ASSERT(a.size() == this->size());
    ArrayListV<V> tmp(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        tmp[i] = this->m_p[i] * a[i];
    }
    return tmp;
}
template<typename V>
ArrayListV<V> ArrayListV<V>::operator/(const ArrayListV<V> &a) {
    ASSERT(a.size() == this->size());
    ArrayListV<V> tmp(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        tmp[i] = this->m_p[i] / a[i];
    }
    return tmp;
}
template<typename V>
ArrayListV<V> ArrayListV<V>::operator*(const V &a) {
    ArrayListV<V> sum(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        sum[i] = this->m_p[i] * a;
    }
    return sum;
}
template<typename V>
ArrayListV<V> ArrayListV<V>::operator/(const V &a) {
    ArrayListV<V> sum(this->m_Len);
    for (size_type i = 0; i < this->m_Len; i++) {
        sum[i] = this->m_p[i] / a;
    }
    return sum;
}
template<typename V>
V ArrayListV<V>::sum() const {
    V sum = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        sum += this->m_p[i];
    }
    return sum;
}
template<typename V>
V ArrayListV<V>::min() const {
    ASSERT(this->m_p!=NULL);
    V min = this->m_p[0];
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] < min) {
            min = this->m_p[i];
        }
    }
    return min;
}
template<typename V>
V ArrayListV<V>::max() const {
    ASSERT(this->m_p!=NULL);
    V max = this->m_p[0];
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] > max) {
            max = this->m_p[i];
        }
    }
    return max;
}
template<typename V>
St ArrayListV<V>::findMinIdx() const {
    ASSERT(this->m_p!=NULL);
    V min = this->m_p[0];
    size_type idx = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] < min) {
            min = this->m_p[i];
            idx = i;
        }
    }
    return idx;
}
template<typename V>
St ArrayListV<V>::findMaxIdx() const {
    ASSERT(this->m_p!=NULL);
    V max = this->m_p[0];
    size_type idx = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] > max) {
            max = this->m_p[i];
            idx = i;
        }
    }
    return idx;
}
template<typename V>
void ArrayListV<V>::assign_forward(const V& be, const V& step) {
    for (size_type i = 0; i < this->m_Len; i++) {
        this->m_p[i] = be + i * step;
    }
}
template<typename V>
void ArrayListV<V>::assign_backward(const V& be, const V& step) {
    for (size_type i = this->m_Len - 1; i != 0; --i) {
        this->m_p[i] = be + (this->m_Len - 1 - i) * step;
    }
    this->m_p[0] = be + (this->m_Len - 1) * step;
}

template<typename V>
void ArrayListV<V>::ones() {
    this->assign_forward(1.0, 0.0);
}

template<typename V>
void ArrayListV<V>::zeros() {
    this->assign_forward(V(0.0), V(0.0));
}

template<typename V>
St ArrayListV<V>::count_equal(const V &a) const {
    if (this->m_p != NULL) {
        return 0;
    }
    size_type count = 0;
    for (size_type i = 0; i < this->m_Len; i++) {
        if (this->m_p[i] == a) {
            count++;
        }
    }
    return count;
}
template<typename V>
void ArrayListV<V>::show() const {
    std::cout << "size = " << this->m_Len << "\n";
    for (int i = 0; i < this->m_Len; i++) {
        std::cout.width(5);
        std::cout << i << " ";
        std::cout.width(12);
        std::cout.precision(8);
        std::cout << this->m_p[i] << "\n";
    }
}
//=========================================================
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator+=(const V &a) {
    AddEqual(this->size(), this->m_p, a);
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator-=(const V &a) {
    for (St i = 0; i < this->size(); ++i) {
        this->at(i) -= a;
    }
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator*=(const V &a) {
    for (St i = 0; i < this->size(); ++i) {
        this->at(i) *= a;
    }
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator/=(const V &a) {
    for (St i = 0; i < this->size(); ++i) {
        this->at(i) /= a;
    }
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator+=(const ArrayListV<V> &a) {
	ASSERT(this->size() == a.size());
	AddEqual(this->size(), this->m_p, a.m_p);
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator-=(const ArrayListV<V> &a) {
	ASSERT(this->size() == a.size());
	MinusEqual(this->size(), this->m_p, a.m_p);
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator*=(const ArrayListV<V> &a) {
	ASSERT(this->size() == a.size());
	MultiplyEqual(this->size(), this->m_p, a.m_p);
    return *this;
}
template<typename V>
ArrayListV<V>& ArrayListV<V>::operator/=(const ArrayListV<V> &a) {
	ASSERT(this->size() == a.size());
	DivideEqual(this->size(), this->m_p, a.m_p);
    return *this;
}


template<typename V>
ArrayListV<V> operator*(const V& a, const ArrayListV<V>& x) {
    ArrayListV<V> res(x);
    res *= a;
    return res;
}
template<typename V>
ArrayListV<V> operator*(const ArrayListV<V>& x, const V&a) {
    ArrayListV<V> res(x);
    res *= a;
    return res;
}

template<typename V>
ArrayListV<V> operator-(const ArrayListV<V> &a, const ArrayListV<V> &b) {
    ArrayListV<V> res(a);
    res -= b;
    return res;
}

typedef ArrayListV<Float> arrayList;
typedef ArrayListV<St> arrayList_st;
typedef ArrayListV<int> arrayList_int;
typedef ArrayListT<bool> arrayList_bool;
typedef ArrayListT<unsigned long int> arrayList_ul;

}

#endif /* ARRAYT_H_ */
