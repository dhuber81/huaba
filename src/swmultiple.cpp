#include "swmultiple.hpp"
#include "math.h"

struct SwMultiple : Module {
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

	SwMultiple() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

	void step() override;
};

void SwMultiple::step() {
	float outa=0.0;
	float outb=0.0;

	for(int i=0; i<8; i++) {
		if(params[SW1_PARAM+i].value == 2.0)
			outa += inputs[IN1_INPUT+i].normalize(0.0);
		if(params[SW1_PARAM+i].value == 0.0)			
			outb += inputs[IN1_INPUT+i].normalize(0.0);	
	}
	
	outputs[OUTA_OUTPUT].value = outa;
	outputs[OUTB_OUTPUT].value = outb;
}

struct SwMultipleWidget : ModuleWidget {
	SwMultipleWidget(SwMultiple *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/SwMultiple.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		const float offset_y = 40, delta_y = 26.5+3, offset_x=2;

		for( int i=0; i<8; i++) {
			addInput(Port::create<PJ301MPort>(Vec(offset_x, offset_y + i*delta_y), Port::INPUT, module, SwMultiple::IN1_INPUT+i));			
			addParam(ParamWidget::create<dh_switch3>(Vec(offset_x+27, offset_y+6 + i*delta_y), module, SwMultiple::SW1_PARAM+i, 0.0, 2.0, 1.0));		
		}
		addOutput(Port::create<PJ301MPort>(Vec(offset_x+1.5, 320), Port::OUTPUT, module, SwMultiple::OUTA_OUTPUT));
		addOutput(Port::create<PJ301MPort>(Vec(offset_x+29, 320), Port::OUTPUT, module, SwMultiple::OUTB_OUTPUT));
	}
};

Model *modelSwMultiple = Model::create<SwMultiple, SwMultipleWidget>("huaba", "Switched Multiple", "Switched Multiple", UTILITY_TAG, MULTIPLE_TAG);
