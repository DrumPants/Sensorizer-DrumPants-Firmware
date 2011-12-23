#ifndef SensorOutput_h
#define SensorOutput_h

#include <string>
using namespace std;

#include "filters/OutputFilter.h"
////import com.odbol.sensorizer.server.filters.OutputFilter;

#include "MidiMapping.h"
#include "RangePreset.h"

class SensorOutput {
	
	//cutoff dropdown items
	/*
	public: static const string CUTOFF_TYPE_NONE;
	public: static const string CUTOFF_TYPE_HARD;
	public: static const string CUTOFF_TYPE_CLIP;
	public: static const string CUTOFF_TYPE_NULLABLE;
	//public: static const string CUTOFF_TYPE_ONEHIT;
	*/
	public: static const int CUTOFF_TYPE_VAL_NONE;
	public: static const int CUTOFF_TYPE_VAL_HARD;
	public: static const int CUTOFF_TYPE_VAL_CLIP;
	public: static const int CUTOFF_TYPE_VAL_NULLABLE;
	//public: static const int CUTOFF_TYPE_VAL_ONEHIT;
	
	//sensor value extrema. this should be the number range the Arduino sends in.
	//private: static const double SIGNAL_IN_MIN;
	//private: static const double SIGNAL_IN_MAX;

	//hold GSON preset values for ranges
	////@Expose
	public: RangePreset inRange;
	
	////@Expose
	public: RangePreset outRange;
	
	////@Expose
	public: RangePreset cutoffRange;
	
	
	//for equations
	////@Expose
	public: float multiplyVal;
	
	////@Expose
	public: float addVal;

	//invert signal
	////@Expose
	public: bool isInvert;
	
	//hold GSON preset values for DropdownList
	////@Expose
	//private: string cutoffType;
	private: int cutoffTypeVal;
	
	//hold GSON preset values for DropdownList
	//////@Expose
	//private: DropdownPreset filtersPreset;


	////@Expose
	public: MidiMapping[] dropdownMidiMappings;
	
	
	//current, calculated output 
	private: double outputValue;
	private: double inputValue;

	////@Expose
	public: OutputFilter[] outputFilters;
	
	public: SensorOutput();
	
	public: void init();


	
	public: int getCutoffType();
	
	public: void setCutoffType(int cutoffType);

	//scales a number to fit within the new extrema.
	public: static double scaleRange(double num, double oldMin, double oldMax, double newMin, double newMax);
	
	//scales a number to fit within the new extrema.
	//if hardCutoff is true, it will not exceed the extrema
	public: static double scaleRange(double num, double oldMin, double oldMax, double newMin, double newMax, bool hardCutoff);

	//scales a number using different scales depending if num is greater or less than the zero point oldZero.
	//if hardCutoff is true, it will not exceed the extrema
	public: static double scaleWeightedRange(double num, double oldMin, double oldZero, double oldMax, double newMin, double newMax, bool hardCutoff);
	
	
	
	

	//sets the input value, and
	//calculates the output value based on slider and cuttoff settings
	//value should ALWAYS be between 0 and 1
	public: void setValue(double val);
	
	//returns the current raw sensor value
	public: double inputValue();
	
	//returns the caluclated output value based on slider and cuttoff settings
	public: double outputValue();
	
	

	//sends all messages through ALL server mappings
	public: void send();
	
	public: void addOutputFilter(string name, OutputFilter filter);
	
	public: void removeOutputFilter(OutputFilter filter);

	
	public: MidiMapping addMidiMapping(string deviceName);

	/***
	 * assumes m has already been added.
	 * //@param m
	 */
	public: void removeMidiMapping(MidiMapping m);
	
};


#endif
