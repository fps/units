#include <units-0/units.h>

#include <stdexcept>

#include <ladspamm-0/library.h>
#include <ladspamm-0/plugin.h>
#include <ladspamm-0/plugin_instance.h>

namespace units
{
	extern "C"
	{
		int jack_process(jack_nframes_t nframes, void* arg)
		{
			return ((ladspa_host*)arg)->process(nframes);
		}
	}

	ladspa_host::ladspa_host
	(
			const std::string& jack_client_name,
			const std::vector<std::pair<std::string, std::string> >& plugins
	) :
		m_jack_client(jack_client_open(jack_client_name.c_str(), JackUseExactName, &m_jack_status))
	{
		if (NULL == m_jack_client)
		{
			throw std::runtime_error("Failed to open jack client");
		}
		
		if (0 != jack_set_process_callback(m_jack_client, jack_process, this))
		{
			throw std::runtime_error("Failed to set jack process callback");
		}
		
		try
		{
			setup_plugins(plugins);
		}
		catch(std::runtime_error &e)
		{
			jack_client_close(m_jack_client);
			throw;
		}
		
		if (0 != jack_activate(m_jack_client))
		{
			throw std::runtime_error("Failed to activate jack client");
		}
	}
	
	void ladspa_host::setup_plugins(const std::vector<std::pair<std::string, std::string> >& plugins)
	{
		for (unsigned index = 0; index < plugins.size(); ++index)
		{
			setup_plugin(plugins[index]);
		}
	}
	
	void ladspa_host::setup_plugin(const std::pair<std::string, std::string> &plugin)
	{
		ladspamm::library_ptr lib(new ladspamm::library(plugin.first));
		
		for (unsigned index = 0; index < lib->plugins.size(); ++index)
		{
			if (lib->plugins[index]->label() == plugin.second)
			{
				ladspamm::plugin_instance_ptr instance
				(
					new ladspamm::plugin_instance
					(
						lib->plugins[index], 
						jack_get_sample_rate(m_jack_client)
					)
				);
				
				m_plugin_instances.push_back(instance);
			
				setup_ports_and_values(instance);
				
				return;
			}
		}
		
		throw std::runtime_error("Plugin not found with label " + plugin.second);
	}
	
	void ladspa_host::setup_ports_and_values(const boost::shared_ptr<ladspamm::plugin_instance> &instance)
	{
		ladspamm::plugin_ptr plugin = instance->the_plugin;
		
		for (unsigned index = 0; index < plugin->port_count(); ++index)
		{
			unsigned long port_flags = 0;
			m_input_port_values.push_back(instance->port_default_guessed(index));

			if (plugin->port_is_input(index))
			{
				port_flags |= JackPortIsInput;
			}
			
			if (plugin->port_is_output(index))
			{
				port_flags |= JackPortIsOutput;
			}
			
			std::string port_name = plugin->label() + "-" + plugin->port_name(index);
			
			jack_port_t *port = jack_port_register
			(
				m_jack_client,
				port_name.c_str(),
				JACK_DEFAULT_AUDIO_TYPE,
				port_flags,
				0
			);
			
			if (NULL == port)
			{
				throw std::runtime_error("Failed to register port");
			}
			
			m_jack_ports.push_back(port);
		}
	}


	ladspa_host::~ladspa_host()
	{
		jack_deactivate(m_jack_client);
		
		jack_client_close(m_jack_client);
	}
	
	int ladspa_host::process(jack_nframes_t nframes)
	{
		for (unsigned plugin_index = 0, port_start_index = 0; plugin_index < m_plugin_instances.size(); ++plugin_index)
		{
			process_plugin
			(
				plugin_index, 
				port_start_index, 
				port_start_index + m_plugin_instances[plugin_index]->the_plugin->port_count(),
				nframes
			);
			
			port_start_index += m_plugin_instances[plugin_index]->the_plugin->port_count();
		}
		
		return 0;
	}
	
	void ladspa_host::process_plugin
	(
		unsigned plugin_index, 
		unsigned port_start_index, 
		unsigned port_end_index, 
		jack_nframes_t nframes
	)
	{
		ladspamm::plugin_instance_ptr instance = m_plugin_instances[plugin_index];
		
		for (unsigned frame_index = 0; frame_index < nframes; ++frame_index)
		{
			for 
			(
				unsigned port_index = port_start_index, max_port_index = port_end_index; 
				port_index < max_port_index; 
				++port_index
			)
			{
				const unsigned plugin_port_index = port_index - port_start_index;
				
				if (0 == jack_port_connected(m_jack_ports[port_index]))
				{
					instance->connect_port(plugin_port_index, &m_input_port_values[port_index]);
				}
				else
				{
					instance->connect_port
					(
						plugin_port_index, 
						((float*)jack_port_get_buffer(m_jack_ports[port_index], nframes)) + frame_index
					);
				}
				
				instance->run(1);
			}
		}
	}
	
	void ladspa_host::set_input_port_value(unsigned index, float value)
	{
		
	}
}