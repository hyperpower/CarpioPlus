#ifndef _EVENT_OUTPUT_FIELD_HPP
#define _EVENT_OUTPUT_FIELD_HPP

#include <map>
#include <utility>
#include <iostream>
#include <list>
#include "type_define.hpp"
#include "event.hpp"
#include "utility/clock.hpp"
#include "utility/tinyformat.hpp"

namespace carpio{

template<St DIM, class D>
class EventOutputField_ : public Event_<DIM, D>{
public:
	typedef Event_<DIM, D> Event;
	typedef Equation_<DIM, D> Equ;
	typedef Equ* pEqu;
	typedef const Equ* const_pEqu;

	typedef typename D::IOFile IOFile;

protected:
	std::string _sn;     // scalar name
	std::string _format; // format string
	std::string _path;   // path for output file

public:
	EventOutputField_(const std::string& sname,
			int is    = -1, int ie   = -1,
	        int istep = -1, int flag = Event::AFTER) :
		 _sn(sname),
		 Event(is, ie, istep, flag) {
		_format = "%s_%d_%8.4e.txt";
		_path   = "./";
	}

	int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
		auto fn = _file_name(step, t, fob);
		if(pd->has_field(_sn)){
			IOFile::OutputField(fn, (*pd)[_sn]);
		}else{
			std::cerr<< "EventOutputField : " << _sn << " not found!" << std::endl;
		}
		return -1;
	}

	void set_format_string(const std::string& format){
		_format = format;
	}

	void set_path(const std::string& path){
		_path = path;
	}

protected:
	std::string _file_name(St step, Vt t, int fob) const{
		std::stringstream ss;
		ss << _path;
		tfm::format(ss, _format.c_str() , _sn, step, t);
		return ss.str();
	}
};


}
#endif
