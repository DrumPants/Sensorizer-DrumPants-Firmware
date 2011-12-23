#include "SensorOutput.h"
#include "filters/OutputFilter.h"
////import com.odbol.sensorizer.server.filters.OutputFilter;

	
	//cutoff dropdown items
	/*
	static const string SensorOutput::CUTOFF_TYPE_NONE = "No Cutoff";
	static const string SensorOutput::CUTOFF_TYPE_HARD = "Hard Cutoff";
	static const string SensorOutput::CUTOFF_TYPE_CLIP = "Clip";
	static const string SensorOutput::CUTOFF_TYPE_NULLABLE = "NULLable";
	//static const string SensorOutput::CUTOFF_TYPE_ONEHIT = "One-Hit";
	*/
	const int SensorOutput::CUTOFF_TYPE_VAL_NONE = 0;
	const int SensorOutput::CUTOFF_TYPE_VAL_HARD = 1;
	const int SensorOutput::CUTOFF_TYPE_VAL_CLIP = 2;
	const int SensorOutput::CUTOFF_TYPE_VAL_NULLABLE = 3;
	//static const int SensorOutput::CUTOFF_TYPE_VAL_ONEHIT = 3;
	
	//sensor value extrema. this should be the number range the Arduino sends in.
	//static const double SensorOutput::SIGNAL_IN_MIN = 0;
	//static const double SensorOutput::SIGNAL_IN_MAX = 1023;

	SensorOutput::SensorOutput() {
		multiplyVal = 1;
		addVal = 0;
		isInvert = false;
		cutoffTypeVal = CUTOFF_TYPE_VAL_NONE;
		outputValue = 0;
		
		init();
	}

	void SensorOutput::init() {		
		for (MidiMapping o : dropdownMidiMappings) 
			o.init();
		
		//init val from string, since we don't have a nice enum
		//setCutoffType(this->cutoffType);
	}


	
	int SensorOutput::getCutoffType() {
		return cutoffTypeVal;
	}
	
	void SensorOutput::setCutoffType(int cutoffType) {
		this->cutoffTypeVal = cutoffType;
	}

	//scales a number to fit within the new extrema.
	static double SensorOutput::scaleRange(double num, double oldMin, double oldMax, double newMin, double newMax) {

		return scaleRange(num, oldMin, oldMax, newMin, newMax, false);
	}
	
	//scales a number to fit within the new extrema.
	//if hardCutoff is true, it will not exceed the extrema
	static double SensorOutput::scaleRange(double num, double oldMin, double oldMax, double newMin, double newMax, bool hardCutoff) {

	  double n =  ((num - oldMin) / ((oldMax - oldMin) / (newMax - newMin))) + newMin;
	  if (hardCutoff == true) {
	    if (n < newMin) 
	      n = newMin;
	    else if (n > newMax)
	      n = newMax;
	  }
	  return n;
	}

	//scales a number using different scales depending if num is greater or less than the zero point oldZero.
	//if hardCutoff is true, it will not exceed the extrema
	static double SensorOutput::scaleWeightedRange(double num, double oldMin, double oldZero, double oldMax, double newMin, double newMax, bool hardCutoff) {
	  double newZero = scaleRange(0.5, 0, 1, newMin, newMax);

	  if (num > oldZero) 
	    return scaleRange(num, oldZero, oldMax, newZero, newMax, hardCutoff);
	  else
	    return scaleRange(num, oldMin, oldZero, newMin, newZero, hardCutoff);
	}
	
	
	
	

	//sets the input value, and
	//calculates the output value based on slider and cuttoff settings
	//value should ALWAYS be between 0 and 1
	void SensorOutput::setValue(double val) {
		//invert
		if (isInvert)
			val = 1.0 - val;

		//save cur raw val
		inputValue = val;
		
		//calculate values!!!!!!
		if (inRange == NULL) 
			return;

		outputValue = scaleRange(val, inRange.low, inRange.high, outRange.low, outRange.high);
		
		
		if (cutoffTypeVal == CUTOFF_TYPE_VAL_HARD) {
			if (outputValue > cutoffRange.high)
				outputValue = 1.0;
			else if (outputValue < cutoffRange.low)
				outputValue = 0.0;
		}
		if (cutoffTypeVal == (CUTOFF_TYPE_VAL_NULLABLE)) {
			if (outputValue > cutoffRange.high)
				outputValue = SensorizerServer::SENSOR_VALUE_NULL;
			else if (outputValue < cutoffRange.low)
				outputValue = SensorizerServer::SENSOR_VALUE_NULL;
		}		
		else if (cutoffTypeVal == (CUTOFF_TYPE_VAL_CLIP)) {
			if (outputValue > cutoffRange.high)
				outputValue = cutoffRange.high;
			else if (outputValue < cutoffRange.low)
				outputValue = cutoffRange.low;			
		}
		
		else { //NONE
			
			
		}
		
		//synchronized(outputFilters) { 
			for (int i = 0; i < OUTPUT_FITLERS_LENGTH; i++) {
				if (outputFilters[i] != NULL) {		
					outputFilters[i].setValue(outputValue);	
					outputValue = outputFilters[i].value();	
				}
			}
		//}
		
		//TODO: equaitons should go here but out meters need to accept any num

		//EQUATIONS
		if (outputValue != SensorizerServer::SENSOR_VALUE_NULL) {
			outputValue = outputValue * multiplyVal + addVal;
		}
	}
	
	//returns the current raw sensor value
	double SensorOutput::inputValue() {
		return inputValue;
	}
	
	//returns the caluclated output value based on slider and cuttoff settings
	double SensorOutput::outputValue() {
		return outputValue;
	}
	
	

	//sends all messages through ALL server mappings
	void SensorOutput::send() {
		double outvals[1] = {outputValue()}; //only call once!
		if (outvals[0] != SensorizerServer::SENSOR_VALUE_NULL || cutoffTypeVal == CUTOFF_TYPE_VAL_NULLABLE) {
			//synchronized(dropdownMidiMappings) { 
				for (int i = 0; i < MIDI_MAPPINGS_LENGTH; i++) {
					//if (dropdownMidiMappings[i] != NULL) {
						dropdownMidiMappings[i].send(outvals);
					//}
				}
			//}
		}
	}
	
	void SensorOutput::addOutputFilter(string name, OutputFilter filter) {
		synchronized(outputFilters) { 
			//outputFilters = Arrays.copyOf(outputFilters, outputFilters.length + 1);
			OutputFilter[] si = new OutputFilter[outputFilters.length + 1];
			if (outputFilters.length > 0)
				System.arraycopy(outputFilters, 0, si, 0, outputFilters.length);//Arrays.copyOf(sensorInputs, sensorInputs.length + 1);
			outputFilters = si;
			
			outputFilters[outputFilters.length - 1] = filter;
		}
	}
	
	void SensorOutput::removeOutputFilter(OutputFilter filter) {
		if (outputFilters.length > 0) {
			synchronized(outputFilters) { 
				//outputFilters = Arrays.copyOf(outputFilters, outputFilters.length + 1);
				OutputFilter[] si = new OutputFilter[outputFilters.length - 1];
				int curIdx = 0;
				for (int i = 0; i < outputFilters.length; i++) {
					if (outputFilters[i] != filter)
						si[curIdx++] = outputFilters[i];
				}
				if (curIdx != si.length - 1) //if it didn't exist, don't remove it (i.e. don't leave empty spot!)
					outputFilters = si;
			}
		}
	}

	
	MidiMapping SensorOutput::addMidiMapping(string deviceName) {
		synchronized(dropdownMidiMappings) { 
			//dropdownMidiMappings = Arrays.copyOf(dropdownMidiMappings, dropdownMidiMappings.length + 1);
			MidiMapping[] si = new MidiMapping[dropdownMidiMappings.length + 1];
			if (dropdownMidiMappings.length > 0)
				System.arraycopy(dropdownMidiMappings, 0, si, 0, dropdownMidiMappings.length);//Arrays.copyOf(sensorInputs, sensorInputs.length + 1);
			dropdownMidiMappings = si;
			
			MidiMapping d = dropdownMidiMappings[dropdownMidiMappings.length - 1] = new MidiMapping();
			
			d.setOutputDevice(deviceName);
			d.init();
	
			return d;
		}
	}

	/***
	 * assumes m has already been added.
	 * //@param m
	 */
	void SensorOutput::removeMidiMapping(MidiMapping m) {
		if (dropdownMidiMappings.length > 0) {
			synchronized(dropdownMidiMappings) { 
				//MidiMappings = Arrays.copyOf(MidiMappings, MidiMappings.length + 1);
				MidiMapping[] si = new MidiMapping[dropdownMidiMappings.length - 1];
				int curIdx = 0;
				for (int i = 0; i < dropdownMidiMappings.length; i++) {
					if (dropdownMidiMappings[i] != m)
						si[curIdx++] = dropdownMidiMappings[i];
				}
				if (curIdx != si.length + 1) //if it didn't exist, don't remove it (i.e. don't leave empty spot!)
					dropdownMidiMappings = si;
			}
		}
	}
	
