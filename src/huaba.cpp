#include "eq3.hpp"
#include "abbus.hpp"


Plugin *plugin;

void init(Plugin *p) {
	plugin = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);

	// Add all Models defined throughout the plugin
	p->addModel(modelEQ3);
	p->addModel(modelABBus);
}
