#ifndef FPS_IO_SYNTH_HH
#define FPS_IO_SYNTH_HH

#include <boost/shared_ptr.hpp>

namespace unit {

struct synth_base {
	virtual ~synth_base() {  }
};

/*
	A synth is something that has a unit and runs it
	to produce output
*/
template <class unit>
struct synth : synth_base {
	unit u;

	synth(unit u) : u(u) { } 
};

template <class unit>
synth<unit> make_synth(unit u) {
	return synth<unit>(u);
}

template<class unit>
boost::shared_ptr<synth_base> make_synth_ptr(unit u) {
	return boost::shared_ptr<synth_base>(new synth<unit>(u));
}

} // namespace

#endif
