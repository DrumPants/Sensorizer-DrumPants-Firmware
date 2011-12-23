#ifndef OutputMapping_h
#define OutputMapping_h
class OutputMapping {
	public: virtual void send(double values[]);
	
	//associates an output device with given name to the mapping
	public: virtual void setOutputDevice(string name);
	
	//inits and connects to output device
	public: virtual void init();
};
#endif