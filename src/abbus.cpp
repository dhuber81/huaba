#include "abbus.hpp"
#include "math.h"

struct ABBus : Module {
	enum ParamIds {
		SW1_PARAM,
		SW2_PARAM,
		SW3_PARAM,
		SW4_PARAM,
		SW5_PARAM,
		SW6_PARAM,
		SW7_PARAM,
		SW8_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		IN5_INPUT,
		IN6_INPUT,
		IN7_INPUT,
		IN8_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTA_OUTPUT,
		OUTB_OUTPUT,
		NUM_OUTPUTS
	};

	ABBus() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
		for(int i=0; i<NUM_PARAMS; i++) {
			configParam(SW1_PARAM+i, 0.0f, 2.0f, 1.0f);
		}
	}

	void process(const ProcessArgs& args) override;
};

void ABBus::process(const ProcessArgs& args) {
	float outa=0.0;
	float outb=0.0;

	for(int i=0; i<NUM_PARAMS; i++) {
		if(params[SW1_PARAM+i].getValue() == 2.0)
			outa += inputs[IN1_INPUT+i].getNormalVoltage(0.0);
		if(params[SW1_PARAM+i].getValue() == 0.0)			
			outb += inputs[IN1_INPUT+i].getNormalVoltage(0.0);	
	}
	
	outputs[OUTA_OUTPUT].setVoltage(outa);
	outputs[OUTB_OUTPUT].setVoltage(outb);
}

struct ABBusWidget : ModuleWidget {
	ABBusWidget(ABBus *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ABBus.svg")));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH+8, 0)));		
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH+8, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));		
		const float offset_y = 40, delta_y = 26.5+3, offset_x=2;
		for( int i=0; i<ABBus::NUM_PARAMS; i++) {
			addInput(createInput<PJ301MPort>(Vec(offset_x, offset_y + i*delta_y), module, ABBus::IN1_INPUT+i));			
			addChild(createParam<dh_switch3>(Vec(offset_x+27,  offset_y+6 + i*delta_y), module, ABBus::SW1_PARAM+i));
		}
		addOutput(createOutput<PJ301MPort>(Vec(offset_x+1.5, 320), module, ABBus::OUTA_OUTPUT));
		addOutput(createOutput<PJ301MPort>(Vec(offset_x+29, 320), module, ABBus::OUTB_OUTPUT));
	}
};

Model *modelABBus = createModel<ABBus, ABBusWidget>("ABBus");
