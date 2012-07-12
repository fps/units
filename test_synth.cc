#include <synth.h>

using namespace unit;

struct foo { };

int main() {
	make_synth_ptr(foo());
}
