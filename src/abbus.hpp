#include "rack.hpp"

using namespace rack;

// Forward-declare the Plugin
extern Plugin *plugin;

// Forward-declare each Model, defined in each module source file
extern Model *modelABBus;

// Exports 
struct dh_switch3 : SVGSwitch, ToggleSwitch {
	dh_switch3() {
		addFrame(SVG::load(assetPlugin(plugin,"res/Switch3_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin,"res/Switch3_1.svg")));
        addFrame(SVG::load(assetPlugin(plugin,"res/Switch3_2.svg")));
	}
};