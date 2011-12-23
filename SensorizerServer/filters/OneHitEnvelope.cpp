#include "OneHitEnvelope.h"
#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;


	////@Expose
	static string OneHitEnvelope::type = "OneHitEnvelope";
	
	static const int OneHitEnvelope::LINEAR = 0;
	static const int OneHitEnvelope::LINEAR_INV = 1;
	static const int OneHitEnvelope::PARABOLIC = 2;
	static const int OneHitEnvelope::PARABOLIC_INV = 3;
	static const int OneHitEnvelope::LOG = 4;
	static const int OneHitEnvelope::LOG_INV = 5;
	static const int OneHitEnvelope::EXP = 6;
	static const int OneHitEnvelope::EXP_INV = 7;
	static const int OneHitEnvelope::SIN = 8;
	static const int OneHitEnvelope::COS = 9;

	int OneHitEnvelope::frameIdx = -1;
	

	int OneHitEnvelope::subframeIdx = 0;
	double OneHitEnvelope::subframeAmt = 0;
	
	//holds the speed the envelope is played at.
	////@Expose
	int OneHitEnvelope::speed = 10;
	
	
	//holds the sequence of values of the envelope
	////@Expose
	double[] OneHitEnvelope::frames = new double[1];

	//@Override
	void OneHitEnvelope::trigger() {
		super.trigger();
		
		frameIdx = 0;
	}

	//@Override
	double OneHitEnvelope::value() {
		if (frameIdx < 0)
			return SensorizerServer.SENSOR_VALUE_NULL; //TODO: need to send null values!
		else {
			
			if (frames[frameIdx] == SensorizerServer.SENSOR_VALUE_NULL) //TODO: interpolate from null???
				return SensorizerServer.SENSOR_VALUE_NULL;
			else //interpolate frames
				return frames[frameIdx] + (subframeIdx * subframeAmt);
		}
	}

	//this assumes that it is only called once per draw frame
	////@Override
	void OneHitEnvelope::setValue(double value) {
		if (frameIdx >= 0) //only increment frames if envelope is activated
			if (++subframeIdx >= speed) {
				subframeIdx = 0;
				subframeAmt = 0;
				if (++frameIdx >= frames.length) {
					frameIdx = -1; //stop envelope
				}
				else if (frameIdx < frames.length - 1) {  //can't interpolate past last value!
					//recalculate the amount to interpolate per subframe
					subframeAmt = (frames[frameIdx + 1] - frames[frameIdx]) / speed;
				}
			}
		
		super.setValue(value);
	}
	
	//loads predefined functions for presets
	void OneHitEnvelope::loadFramesPreset(int presetIdx) {
		double scale;
		
		switch (presetIdx) {
		case LINEAR: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = x;
			}
			break;
		case LINEAR_INV: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = 1.0 - x;
			}
			break;
		case PARABOLIC: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = 4 * Math.pow(x - 0.5, 2);
			}
			break;
		case PARABOLIC_INV: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = 1.0 - (4 * Math.pow(x - 0.5, 2));
			}
			break;		
		case LOG: 
			scale = Math.log(frames.length);
			for (int i = 0; i < frames.length; i++) {
				frames[i] = Math.log(i) / scale;
			}
			break;
		case LOG_INV: 
			scale = Math.log(frames.length);
			for (int i = 0; i < frames.length; i++) {
				frames[i] = 1.0 - (Math.log(i) / scale);
			}
			break;	
		case SIN: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = 0.5 * Math.sin(x *6) + 0.5;
			}
			break;
		case COS: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = 0.5 * Math.cos(x *6) + 0.5;
			}
			break;			
		case EXP: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = Math.pow(x, 2);
			}
			break;
		case EXP_INV: 
			for (int i = 0; i < frames.length; i++) {
				double x = (double)i / frames.length;
				frames[i] = 1.0 - Math.pow(x, 2);
			}
			break;		
		}
	}

	void OneHitEnvelope::resetFramesLength(int length) {
		frames = new double[length];
		frameIdx = -1; //stop playing
	}

	//loads cells retreived from a matrix into the frames buffer
	void OneHitEnvelope::loadFrames(Integer[] newVals, double scale) {
		resetFramesLength(newVals.length);
		for (int i = 0; i < newVals.length; i++) {
			if (newVals[i] == null) {
				frames[i] = SensorizerServer.SENSOR_VALUE_NULL;
			}
			else {
				//the Matrix is IN REVERSE, yo!
				frames[i] = 1.0 - (double)newVals[i] / scale;
			}
		}
	}

