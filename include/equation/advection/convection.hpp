#ifndef _CONVECTION_HPP_
#define _CONVECTION_HPP_

#include "equation/equation_define.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio{

template<St DIM, class DOMAIN>
class Convection_ : public Equation_<DIM, DOMAIN>{
public:
	typedef DOMAIN Domain;
	typedef Equation_<DIM, DOMAIN>     Equation;
	typedef typename Domain::SizeType  St;
	typedef typename Domain::ValueType Vt;
	typedef typename Domain::Grid      Grid;
	typedef typename Domain::Ghost     Ghost;
	typedef typename Domain::Order     Order;
	typedef typename Domain::Scalar    Scalar;
	typedef Event_<DIM,Domain>         Event;
	typedef TimeTerm_<DIM>             TimeTerm;

	typedef typename Domain::VectorCenter VectorCenter;
	typedef typename Domain::VectorFace   VectorFace;

	typedef std::shared_ptr<Event>  spEvent;
	typedef std::shared_ptr<Scalar> spScalar;
	typedef std::shared_ptr<Grid>   spGrid;
	typedef std::shared_ptr<Ghost>  spGhost;
	typedef std::shared_ptr<Order>  spOrder;
	typedef std::shared_ptr<VectorCenter>  spVectorCenter;
	typedef std::shared_ptr<VectorFace>    spVectorFace;
	typedef std::shared_ptr<TimeTerm>      spTimeTerm;
	typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	typedef std::map<std::string, Any>                AFlags;
	typedef std::map<std::string, spScalar>           Scalars;
	typedef std::map<std::string, spBoundaryIndex>    BIs;
	typedef std::unordered_map<std::string, spEvent>  Events;
	typedef std::unordered_map<std::string, FunXYZT_Value> Functions;
	typedef std::unordered_map<std::string, Vt>       Values;

	typedef typename Domain::UdotNabla  UdotNabla;

protected:
	spVectorCenter _vc;
	spVectorFace   _vf;
public:
	Convection_(spGrid spg, spGhost spgh, spOrder spo):
		Equation(spg, spgh, spo){
		// new scalars
		_vc = spVectorCenter(new VectorCenter());
		_vf = spVectorFace(new VectorFace(spg, spgh, spo));
		_new_uvw();

		this->new_scalar("phi");
	}

	int initialize() {
		FunXYZT_Value funs[] = {this->get_funtion("initial_u"),
							    this->get_funtion("initial_v"),
				                this->get_funtion("initial_w")};
		FOR_EACH_DIM{
			(*_vc)[d].assign(funs[d], 0.0);
		}

		this->_scalars["phi"]->assign(this->get_funtion("initial_phi"));
		std::cout << "  Convection: initialize \n";
		return -1;
	}

	int finalize() {
		std::cout << "  Convection: finalize \n";
		return -1;
	}

	int run_one_step(St step) {


		std::cout << "    Convection: One Step "<< step <<" \n";
		return -1;
	}



protected:
	void _new_uvw(){
		std::vector<std::string> vname = {"u", "v", "w"};
		for(St d = 0; d< DIM; ++d){
			this->new_scalar(vname[d]);
			_vc->set(ToAxes(d), this->_scalars[vname[d]]);
		}
	}



};


}

#endif
