#ifndef _LINEAR_POLYNOMIAL_H_
#define _LINEAR_POLYNOMIAL_H_

#include <utility>
#include <map>

namespace carpio {

template<class T>
struct IsZero_: std::unary_function<const T&, bool> {
	bool operator()(const T& number) const {
		return (number == 0);
	}
};

template<class COE, class TERM,
         class COMPARE_TERM = std::less<TERM> >                                 //
class LinearPolynomial_ : public std::map<TERM, COE, COMPARE_TERM>{
public:
	typedef std::map<TERM, COE, COMPARE_TERM> Base;
	typedef LinearPolynomial_<COE, TERM, COMPARE_TERM> Self;
	typedef Self& ref_Self;

	typedef COE Vt;
	typedef TERM Term;
protected:
	COE _num;
public:
	LinearPolynomial_() : Base(), _num(0) {
	}

	LinearPolynomial_(const Self& other) : Base(other), _num(other._num) {
	}

	Vt value() const{
		return _num;
	}

	Self& operator=(const Self& other){
		this->Base::operator =(other);
		_num = other._num;
		return *this;
	}

	ref_Self operator+=(const Vt& a){
		this->_num += a;
		return *this;
	}
	ref_Self operator-=(const Vt& a){
		this->_num -= a;
		return *this;
	}
	ref_Self operator*=(const Vt& a){
		for(auto iter = this->begin(); iter!=this->end(); ++iter){
			iter->second *=a;
		}
		this->_num *=a;
		return *this;
	}
	ref_Self operator/=(const Vt& a){
		// a is not zero;
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			iter->second /= a;
		}
		this->_num /= a;
		return *this;
	}
	ref_Self operator+=(const Self& a);
	ref_Self operator-=(const Self& a);


};

template<class COE, class TERM,
         class COMPARE_TERM = std::less<TERM> >
std::ostream& operator<<(
		std::ostream& stream,
		const LinearPolynomial_<COE, TERM, COMPARE_TERM>& p) {
	for (auto& t : p) {
		stream << t.second << " : ";
		stream << t.first  << "\n";
	}
	stream << p.value();
	return stream;
}



}

#endif
