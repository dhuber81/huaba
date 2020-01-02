#include "eq3.hpp"
#include "abbus.hpp"

Plugin *pluginInstance;

void init(Plugin *p) {
	pluginInstance = p;
	p->slug = TOSTRING(SLUG);

	// Add all Models defined throughout the pluginInstance
	p->addModel(modelEQ3);
	p->addModel(modelABBus);
}
