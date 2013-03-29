#ifndef UNITS_HH_INCLUDED
#define UNITS_HH_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <utility>

#include <boost/shared_ptr.hpp>

#include <jack/jack.h>

namespace ladspamm
{
	struct plugin_instance;
}

namespace units
{
	struct ladspa_host
	{
		/*
			Note: Might throw.
		*/
		ladspa_host
		(
			const std::string& jack_client_name,
			const std::vector<std::pair<std::string, std::string> >& plugins
		);
		
		~ladspa_host();
		
		/*
			This value will only be used if the port
			is not connected. 
			
			Note: Might throw.
		*/
		void set_input_port_value(unsigned index, float value);
		
		int process(jack_nframes_t nframes);
		
		protected:
			jack_client_t *m_jack_client;
			
			jack_status_t m_jack_status;
			
			std::vector<jack_port_t *> m_jack_ports;
			
			std::vector<float> m_input_port_values;
			
			std::vector<boost::shared_ptr<ladspamm::plugin_instance> > m_plugin_instances;
			
			void setup_plugins(const std::vector<std::pair<std::string, std::string> >& plugins);
			
			void setup_plugin(const std::pair<std::string, std::string> &plugin);
			
			void setup_ports_and_values(const boost::shared_ptr<ladspamm::plugin_instance> &instance);
			
			void process_plugin(unsigned index, jack_nframes_t nframes);
	};
}

#endif
