#include <units-0/units.h>
#include <unistd.h>

int main()
{
	std::vector<std::pair<std::string, std::string> > plugins;
	
	plugins.push_back(std::make_pair("/usr/lib/ladspa/inv_filter.so", "invada_lp_mono_filter_module_0_1"));
	
	units::ladspa_host h("foobar", plugins);
	
	usleep(5000000);
}

