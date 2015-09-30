/**
 * Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// #ifndef OneHitEnvelope_h
// #define OneHitEnvelope_h

// #include <string>
// using namespace std;

// #include "../SensorizerServer.h"
// ////import com.odbol.sensorizer.server.SensorizerServer;

// //same as MATRIX_CELL in java version
// #define FRAME_LENGTH 20

// class OneHitEnvelope : OneHitDetector {
// 	////@Expose
// 	public: static string type;
	
// 	public: static const int LINEAR;
// 	public: static const int LINEAR_INV;
// 	public: static const int PARABOLIC;
// 	public: static const int PARABOLIC_INV;
// 	public: static const int LOG;
// 	public: static const int LOG_INV;
// 	public: static const int EXP;
// 	public: static const int EXP_INV;
// 	public: static const int SIN;
// 	public: static const int COS;

// 	private: int frameIdx;
	

// 	private: int subframeIdx;
// 	private: double subframeAmt;
	
// 	//holds the speed the envelope is played at.
// 	////@Expose
// 	public: int speed;
	
	
// 	//holds the sequence of values of the envelope
// 	////@Expose
// 	public: double frames[FRAME_LENGTH];

// 	//@Override
// 	public: void trigger();

// 	//@Override
// 	public: double value();

// 	//this assumes that it is only called once per draw frame
// 	////@Override
// 	public: void setValue(double value);
	
// 	//loads predefined functions for presets
// 	public: void loadFramesPreset(int presetIdx);

// 	//public: void resetFramesLength(int length);

// 	//loads cells retreived from a matrix into the frames buffer
// 	public: void loadFrames(int newVals[], double scale);


// 	public: OneHitEnvelope::OneHitEnvelope();
// };


// #endif
