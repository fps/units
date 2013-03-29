#include <units-0/units.h>

#include <stdexcept>

namespace units
{
	host::host(std::string name) :
		m_jack_client(jack_client_open(name.c_str(), JackUseExactName, &m_jack_status))
	{
		if (NULL == m_jack_client)
		{
			throw std::runtime_error("Failed to open jack client");
		}
	}
	
	host::~host()
	{
		
	}
	
	void host::add_unit(unit_ptr unit)
	{
		
	}
	
	unsigned host::number_of_units()
	{
		
	}
	
	void host::remove_unit(unsigned index)
	{
		
	}

	int host::process(jack_nframes_t nframes)
	{
		
	}
}