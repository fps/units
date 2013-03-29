#ifndef UNITS_HH_INCLUDED
#define UNITS_HH_INCLUDED

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <ladspamm-0/plugin.h>
#include <ladspamm-0/plugin_instance.h>
#include <jack/jack.h>

namespace units
{
	struct port
	{
		std::string name;
		
		enum { input, output } direction;
	};
	
	struct unit
	{
		virtual unsigned number_of_ports() = 0;

		virtual struct port port(unsigned index) = 0;
		
		virtual void process(jack_nframes_t nframes) = 0;
		
		virtual ~unit()
		{
			
		}
	};
	
	typedef boost::shared_ptr<unit> unit_ptr;
	
	struct ladspa_unit
	{
		ladspa_unit(const ladspamm::plugin_ptr& plugin);
		
		protected:
			ladspamm::plugin_instance_ptr m_instance;
	};
	
	extern "C"
	{
		int jack_process(jack_nframes_t nframes, void *arg);
	}
	
	struct host
	{
		host(std::string name, bool allow_different_name = true);
		
		~host();
		
		void add_unit(unit_ptr unit);
		
		unsigned number_of_units();
		
		void remove_unit(unsigned index);

		int process(jack_nframes_t nframes);
		
		protected:
			std::vector<unit_ptr> m_units;
	};
	
	extern "C"
	{
		int jack_process(jack_nframes_t nframes, void* arg)
		{
			return ((host*)arg)->process(nframes);
		}
	}
}

#endif
