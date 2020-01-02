#include "eq3.hpp"
#include "math.h"

struct EQ3 : Module {
	enum ParamIds {
		LOW_PARAM,
		MID_PARAM,
		HIGH_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		CV1_INPUT,
		CV2_INPUT,
		CV3_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};


	EQSTATE *eq = new EQSTATE();
	const double vsa = (1.0 / 4294967295.0);   // Denormal Fix

	EQ3() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);	
		configParam(HIGH_PARAM, 0.0f, 2.0f, 1.0f);
		configParam(LOW_PARAM, 0.0f, 2.0f, 1.0f);
		configParam(MID_PARAM, 0.0f, 2.0f, 1.0f);
		init_3band_state(eq, 880, 5000, APP->engine->getSampleRate());
	}
	void process(const ProcessArgs& args) override;

	void init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq) {
		// Clear state
		memset(es,0,sizeof(EQSTATE));

		// Set Low/Mid/High gains to unity
		es->lg = 1.0;
		es->mg = 1.0;
		es->hg = 1.0;

		// Calculate filter cutoff frequencies
		es->lf = 2 * sin(M_PI * ((double)lowfreq / (double)mixfreq));
		es->hf = 2 * sin(M_PI * ((double)highfreq / (double)mixfreq));
	}

	double do_3band(EQSTATE* es, double sample) {
		// Low / Mid / High - Sample Values
		double  l,m,h;      

		// Filter #1 (lowpass)
		es->f1p0 += (es->lf * (sample   - es->f1p0)) + vsa;
		es->f1p1 += (es->lf * (es->f1p0 - es->f1p1));
		es->f1p2 += (es->lf * (es->f1p1 - es->f1p2));
		es->f1p3 += (es->lf * (es->f1p2 - es->f1p3));
		l = es->f1p3;

		// Filter #2 (highpass)
		es->f2p0 += (es->hf * (sample   - es->f2p0)) + vsa;
		es->f2p1 += (es->hf * (es->f2p0 - es->f2p1));
		es->f2p2 += (es->hf * (es->f2p1 - es->f2p2));
		es->f2p3 += (es->hf * (es->f2p2 - es->f2p3));
		h = es->sdm3 - es->f2p3;

		// Calculate midrange (signal - (low + high))
		m = es->sdm3 - (h + l);

		// Scale, Combine and store
		l *= es->lg;
		m *= es->mg;
		h *= es->hg;

		// Shuffle history buffer
		es->sdm3   = es->sdm2;
		es->sdm2   = es->sdm1;
		es->sdm1   = sample;                

		// Return result
		return(l + m + h);
	}
};

void EQ3::process(const ProcessArgs& args) {
	
	eq->lg = clamp(params[LOW_PARAM].getValue() + inputs[CV3_INPUT].getVoltage() / 10.0f, 0.0f, 2.0f);
	eq->mg = clamp(params[MID_PARAM].getValue() + inputs[CV2_INPUT].getVoltage() / 10.0f, 0.0f, 2.0f);
	eq->hg = clamp(params[HIGH_PARAM].getValue() + inputs[CV1_INPUT].getVoltage() / 10.0f, 0.0f, 2.0f);

	if (outputs[OUT1_OUTPUT].isConnected() && inputs[IN1_INPUT].isConnected())
		outputs[OUT1_OUTPUT].setVoltage(do_3band(eq, inputs[IN1_INPUT].getVoltage()));
	if (outputs[OUT2_OUTPUT].isConnected() && inputs[IN2_INPUT].isConnected())
		outputs[OUT2_OUTPUT].setVoltage(do_3band(eq, inputs[IN2_INPUT].getVoltage()));
}

struct EQ3Widget : ModuleWidget {
	EQ3Widget(EQ3 *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/EQ3.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<RoundBlackKnob>(Vec(8, 56), module, EQ3::HIGH_PARAM));
		addInput(createInput<PJ301MPort>(Vec(10.5, 89), module, EQ3::CV1_INPUT));

		addParam(createParam<RoundBlackKnob>(Vec(8, 136), module, EQ3::MID_PARAM));
		addInput(createInput<PJ301MPort>(Vec(10.5, 169), module, EQ3::CV2_INPUT));

		addParam(createParam<RoundBlackKnob>(Vec(8, 215), module, EQ3::LOW_PARAM));
		addInput(createInput<PJ301MPort>(Vec(10.5, 248), module, EQ3::CV3_INPUT));
		
		addInput(createInput<PJ301MPort>(Vec(10.5, 280), module, EQ3::IN1_INPUT));
		addOutput(createOutput<PJ301MPort>(Vec(10.5, 320), module, EQ3::OUT1_OUTPUT));
	}
};

Model *modelEQ3 = createModel<EQ3, EQ3Widget>("EQ3");
