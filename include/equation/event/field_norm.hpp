#ifndef _EVENT_FIELD_NORM_HPP
#define _EVENT_FIELD_NORM_HPP

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
class EventNormPrevious_ : public Event_<DIM, D>{
public:
    typedef Event_<DIM, D> Event;
    typedef Equation_<DIM, D> Equ;
    typedef Equ* pEqu;
    typedef const Equ* const_pEqu;

    typedef typename D::IOFile IOFile;
    typedef typename D::Field  Field;

    typedef typename D::spGrid  spGrid;
    typedef typename D::spGhost spGhost;
    typedef typename D::spOrder spOrder;

    typedef std::list<std::array<Vt, 3> > List;
protected:
    std::string _fn;     //field name
    Field       _fp;     //field previous
    int         _stepp;  //step previous
    int         _count;  //count

    List        _ln1;    //list of norm 1
    List        _ln2;    //list of norm 1
    List        _lninf;  //list of norm 1
public:
    EventNormPrevious_(
        const std::string& fname,
        spGrid spg, spGhost spgh, spOrder spor,
        int is    = -1, int ie   = -1,
        int istep = -1, int flag = Event::AFTER) :
            _fn(fname),
            _count(0),
            _stepp(is),
            _fp(spg, spgh, spor),
            Event(is, ie, istep, flag) {
    }
    virtual ~EventNormPrevious_(){
    }

    virtual int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
        if (pd->has_field(_fn)) {
            auto& fc = (*pd)[_fn];
            if(_count == 0){
                _fp    = fc;  //store previous field
                _stepp = step;
                _count++;
                return _SUCCESS;
            }else{
                // norm1
                auto e = fc - _fp;
                _ln1.push_back({Vt(_stepp), Vt(step), e.norm1()});
                _ln2.push_back({Vt(_stepp), Vt(step), e.norm2()});
                _lninf.push_back({Vt(_stepp), Vt(step), e.norminf()});
                _fp    = fc;
                _stepp = step;
                _count++;
            }
        } else {
            std::cerr << "EventOutputField : " << _fn << " not found!"
                    << std::endl;
        }
        return -1;
    }

    List& get_norm1_list(){
        return _ln1;
    }
    const List& get_norm1_list() const{
        return _ln1;
    }

    List& get_norm2_list() {
        return _ln2;
    }
    const List& get_norm2_list() const {
        return _ln2;
    }

    List& get_norminf_list() {
        return _lninf;
    }
    const List& get_norminf_list() const {
        return _lninf;
    }

    void show() const{
        tfm::format(std::cout, "%13s  %8s  %10s\n",
                "Previous Step", "Step", "Norm1");
        for(auto& e: _ln1){
            tfm::format(std::cout, "%10d  %8d  %10.4e\n", e[0], e[1], e[2]);
        }
    }
};

template<St DIM, class D>
class EventConditionNormPrevious_ : public EventCondition_<DIM, D>{
public:
    typedef Event_<DIM, D> Event;
    typedef EventCondition_<DIM, D> Base;
    typedef Equation_<DIM, D> Equ;
    typedef Equ* pEqu;
    typedef const Equ* const_pEqu;

    typedef typename D::IOFile IOFile;
    typedef typename D::Field  Field;

    typedef typename D::spGrid  spGrid;
    typedef typename D::spGhost spGhost;
    typedef typename D::spOrder spOrder;

    typedef std::list<std::array<Vt, 3> > List;
protected:
    Vt          _cv1;     // critical value,
                          //  a small value > 0.
                          //  if it's less than 0,
                          //  the critical value will not be checked
    Vt          _cv2;     // critical value
    Vt          _cvinf;   // critical value

    std::string _fn;     //field name
    Field       _fp;     //field previous
    int         _stepp;  //step previous
    int         _count;  //count

    List        _ln1;    //list of norm 1
    List        _ln2;    //list of norm 2
    List        _lninf;  //list of norm infinite

    std::string _relation; // "or" or "and"
public:
    EventConditionNormPrevious_(
            const Vt& cv1, const Vt& cv2, const Vt& cvinf,
            const std::string& fname,
            spGrid spg, spGhost spgh, spOrder spor,
            int is    = -1, int ie   = -1,
            int istep = -1, int flag = Event::AFTER) :
                _cv1(cv1), _cv2(cv2), _cvinf(cvinf),
                _fn(fname),
                _count(0),
                _stepp(is),
                _fp(spg, spgh, spor),
                Base(is, ie, istep, flag){
        _relation = "and";
    }

    int execute(St step, Vt t, int fob, pEqu pd = nullptr) {
        if (pd->has_field(this->_fn)) {
            auto& fc = (*pd)[this->_fn];
            if (this->_count == 0) {
                this->_fp = fc;                   // store previous field
                this->_stepp = step;              // previous step
                this->_count++;
                return _SUCCESS;
            } else {
                // norm1
                auto e = fc - this->_fp;
                if(_cv1 > 0){
                    this->_ln1.push_back(   { Vt(this->_stepp), Vt(step), e.norm1() });
                }
                if(_cv2 > 0){
                    this->_ln2.push_back(   { Vt(this->_stepp), Vt(step), e.norm2() });
                }
                if(_cvinf > 0){
                    this->_lninf.push_back( { Vt(this->_stepp), Vt(step), e.norminf() });
                }
                _check_satisfied(this->_relation);

                this->_fp    = fc;
                this->_stepp = step;
                this->_count++;
            }
        } else {
            std::cerr << "EventOutputField : " << this->_fn << " not found!"
                      << std::endl;
        }
        return -1;
    }

    bool is_satisfied() const {
        return this->_is_satisfied;
    }

    List& get_norm1_list() {
        return _ln1;
    }
    const List& get_norm1_list() const {
        return _ln1;
    }

    List& get_norm2_list() {
        return _ln2;
    }
    const List& get_norm2_list() const {
        return _ln2;
    }

    List& get_norminf_list() {
        return _lninf;
    }
    const List& get_norminf_list() const {
        return _lninf;
    }

    void set_relation_and(){
        this->_relation = "and";
    }
    void set_relation_or(){
        this->_relation = "or";
    }

    void show() const {
        tfm::format(std::cout,
                "%13s  %8s  %10s\n",
                "Previous Step", "Step",
                "Norm1");
        for (auto& e : _ln1) {
            tfm::format(std::cout, "%10d  %8d  %10.4e\n", e[0], e[1], e[2]);
        }
    }
protected:
    void _check_satisfied(const std::string& relation = "and") {  // default relation is and
        std::array<int, 3> arrs = {_check_satisfied_norm1(),
                                   _check_satisfied_norm2(),
                                   _check_satisfied_norminf()};
        if(relation == "and"){
            for(auto& state : arrs){
                if(state == 0){
                    this->_is_satisfied = false;
                    return;
                }
            }
            this->_is_satisfied = true;
        }else{ // relation == "or"
            for (auto& state : arrs) {
                if (state == 1) {
                    this->_is_satisfied = true;
                    return;
                }
            }
            this->_is_satisfied = false;
        }
    }

    int _check_satisfied_norm1() const {
        if (_cv1 > 0) {
            return _ln1.back()[2] < _cv1 ? 1 : 0;
        }
        return -1;
    }
    bool _check_satisfied_norm2() const {
        if (_cv2 > 0) {
            return _ln2.back()[2] < _cv2 ? 1 : 0;
        }
        return -1;
    }
    bool _check_satisfied_norminf() const {
        if (_cvinf > 0) {
            return _lninf.back()[2] < _cvinf ? 1 : 0;
        }
        return -1;
    }
};

void OutputNormList(
        const std::string& filename,
        const std::string& normname,
        const std::list<std::array<Vt, 3> >& list){
    // Open a file
    TextFile txtf(filename);
    // format first line
    txtf.add_line(tfm::format("## SIZE : %d", list.size()));
    txtf.add_line(tfm::format("## NAME : %d", normname));
    //
    txtf.add_line(
            tfm::format("## COLUMN_NAME : %s, %s, %s",
                    "PREVIOUS STEP", "STEP", "VALUE"));
    for (auto& row : list) {
        std::stringstream ss;
        tfm::format(ss, "%d, %d, %.8e", row[0], row[1], row[2]);
        txtf.add_line(ss.str());
    }
    txtf.write();
}



}

#endif
