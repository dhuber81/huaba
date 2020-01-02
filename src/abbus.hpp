#include "rack.hpp"

using namespace rack;

// Forward-declare the Plugin
extern Plugin *pluginInstance;

// Forward-declare each Model, defined in each module source file
extern Model *modelABBus;

// Exports 
//template <typename TSequentialSwitch = CKSSThree>
struct dh_switch3 : SVGSwitch{ //  SVGSwitch, ToggleSwitch
	dh_switch3() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Switch3_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Switch3_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Switch3_2.svg")));
	}
};

