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
