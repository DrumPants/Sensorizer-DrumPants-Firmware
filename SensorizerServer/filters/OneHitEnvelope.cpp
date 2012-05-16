#include "OneHitEnvelope.h"
#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;

#include <math.h>

	////@Expose
	string OneHitEnvelope::type = "OneHitEnvelope";
	
	const int OneHitEnvelope::LINEAR = 0;
	const int OneHitEnvelope::LINEAR_INV = 1;
	const int OneHitEnvelope::PARABOLIC = 2;
	const int OneHitEnvelope::PARABOLIC_INV = 3;
	const int OneHitEnvelope::LOG = 4;
	const int OneHitEnvelope::LOG_INV = 5;
	const int OneHitEnvelope::EXP = 6;
	const int OneHitEnvelope::EXP_INV = 7;
	const int OneHitEnvelope::SIN = 8;
	const int OneHitEnvelope::COS = 9;

	OneHitEnvelope::OneHitEnvelope() {
		frameIdx = -1;
		subframeIdx = 0;
		subframeAmt = 0;
		speed = 10;
		//frames = new double[1];

		//manually initialize arrays (ARRG FFFFFFFFUUUUUUUU ISO C++)
		for (int i = 0; i < FRAME_LENGTH; i++) {
			frames[i] = 0.0;
		}
	}
	
	//@Override
	void OneHitEnvelope::trigger() {
		OneHitDetector::trigger();
		
		frameIdx = 0;
	}

	//@Override
	double OneHitEnvelope::value() {
		if (frameIdx < 0)
			return SensorizerServer::SENSOR_VALUE_NULL; //TODO: need to send null values!
		else {
			
			if (frames[frameIdx] == SensorizerServer::SENSOR_VALUE_NULL) //TODO: interpolate from null???
				return SensorizerServer::SENSOR_VALUE_NULL;
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
				if (++frameIdx >= FRAME_LENGTH) {
					frameIdx = -1; //stop envelope
				}
				else if (frameIdx < FRAME_LENGTH - 1) {  //can't interpolate past last value!
					//recalculate the amount to interpolate per subframe
					subframeAmt = (frames[frameIdx + 1] - frames[frameIdx]) / speed;
				}
			}
		
		OneHitDetector::setValue(value);
	}
	
	//loads predefined functions for presets
	void OneHitEnvelope::loadFramesPreset(int presetIdx) {
		double scale;
		
		switch (presetIdx) {
		case LINEAR: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = x;
			}
			break;
		case LINEAR_INV: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = 1.0 - x;
			}
			break;
		case PARABOLIC: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = 4 * pow(x - 0.5, 2);
			}
			break;
		case PARABOLIC_INV: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = 1.0 - (4 * pow(x - 0.5, 2));
			}
			break;		
		case LOG: 
			scale = log(FRAME_LENGTH);
			for (int i = 0; i < FRAME_LENGTH; i++) {
				frames[i] = log(i) / scale;
			}
			break;
		case LOG_INV: 
			scale = log(FRAME_LENGTH);
			for (int i = 0; i < FRAME_LENGTH; i++) {
				frames[i] = 1.0 - (log(i) / scale);
			}
			break;	
		case SIN: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = 0.5 * sin(x *6) + 0.5;
			}
			break;
		case COS: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = 0.5 * cos(x *6) + 0.5;
			}
			break;			
		case EXP: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = pow(x, 2);
			}
			break;
		case EXP_INV: 
			for (int i = 0; i < FRAME_LENGTH; i++) {
				double x = (double)i / FRAME_LENGTH;
				frames[i] = 1.0 - pow(x, 2);
			}
			break;		
		}
	}

/*
	void OneHitEnvelope::resetFramesLength(int length) {
		frames = new double[length];
		frameIdx = -1; //stop playing
	}
*/
	//loads cells retreived from a matrix into the frames buffer
	void OneHitEnvelope::loadFrames(int newVals[], double scale) {
		//resetFramesLength(newVals.length);
		for (int i = 0; i < FRAME_LENGTH; i++) {
			if (newVals[i] == null) {
				frames[i] = SensorizerServer::SENSOR_VALUE_NULL;
			}
			else {
				//the Matrix is IN REVERSE, yo!
				frames[i] = 1.0 - (double)newVals[i] / scale;
			}
		}
	}

