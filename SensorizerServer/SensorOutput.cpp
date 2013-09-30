#include "SensorOutput.h"
#include "filters/OutputFilter.h"
////import com.odbol.sensorizer.server.filters.OutputFilter;

#include "SensorizerServer.h"
	
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
		_outputValue = 0;
		
		//set current array lengths so we can fill them
		outputFiltersCurlength = 0;
		midiMappingsCurlength = 0;
		
		//manually initialize arrays (ARRG FFFFFFFFUUUUUUUU ISO C++)
		for (int i = 0; i < OUTPUT_FILTERS_LENGTH; i++) {
			outputFilters[i] = NULL;
		}
		for (int i = 0; i < MIDI_MAPPINGS_LENGTH; i++) {
			dropdownMidiMappings[i] = NULL;
		}
		
		init();
	}

	void SensorOutput::init() {		
		for (int i = 0; i < midiMappingsCurlength; i++) 
			dropdownMidiMappings[i]->init();
		
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
	double SensorOutput::scaleRange(double num, double oldMin, double oldMax, double newMin, double newMax) {

		return scaleRange(num, oldMin, oldMax, newMin, newMax, false);
	}
	
	//scales a number to fit within the new extrema.
	//if hardCutoff is true, it will not exceed the extrema
	double SensorOutput::scaleRange(double num, double oldMin, double oldMax, double newMin, double newMax, bool hardCutoff) {

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
	double SensorOutput::scaleWeightedRange(double num, double oldMin, double oldZero, double oldMax, double newMin, double newMax, bool hardCutoff) {
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
		//DEBUG_PRINT_NUM("SensorOutput::setValue ", val)
	
		//invert
		if (isInvert)
			val = 1.0 - val;

		//save cur raw val
		_inputValue = val;
		
		//calculate values!!!!!!
		//if (inRange == NULL) 
		//	return;

		_outputValue = scaleRange(val, inRange.low, inRange.high, outRange.low, outRange.high);
		
		
		if (cutoffTypeVal == CUTOFF_TYPE_VAL_HARD) {
			if (_outputValue > cutoffRange.high)
				_outputValue = 1.0;
			else if (_outputValue < cutoffRange.low)
				_outputValue = 0.0;
		}
		if (cutoffTypeVal == (CUTOFF_TYPE_VAL_NULLABLE)) {
			if (_outputValue > cutoffRange.high)
				_outputValue = SensorizerServer::SENSOR_VALUE_NULL;
			else if (_outputValue < cutoffRange.low)
				_outputValue = SensorizerServer::SENSOR_VALUE_NULL;
		}		
		else if (cutoffTypeVal == (CUTOFF_TYPE_VAL_CLIP)) {
			if (_outputValue > cutoffRange.high)
				_outputValue = cutoffRange.high;
			else if (_outputValue < cutoffRange.low)
				_outputValue = cutoffRange.low;			
		}
		
		else { //NONE
			
			
		}
		
		//DEBUG_PRINT_NUM("SensorOutput::.outputFiltersCurlength ", outputFiltersCurlength)
		//synchronized(outputFilters) { 
			for (int i = 0; i < outputFiltersCurlength; i++) {
				//if (outputFilters[i] != NULL) {		
					outputFilters[i]->setValue(_outputValue);	
					_outputValue = outputFilters[i]->value();	
				//}
			}
		//}
		
		//TODO: equaitons should go here but out meters need to accept any num

		//EQUATIONS
		if (_outputValue != SensorizerServer::SENSOR_VALUE_NULL) {
			_outputValue = _outputValue * multiplyVal + addVal;
		}
		
		//DEBUG_PRINT_NUM("DONE SensorOutput::setValue ", _outputValue)
	}
	
	//returns the current raw sensor value
	double SensorOutput::inputValue() {
		return _inputValue;
	}
	
	//returns the caluclated output value based on slider and cuttoff settings
	double SensorOutput::outputValue() {
		return _outputValue;
	}
	
	

	//sends all messages through ALL server mappings
	void SensorOutput::send() {
		//DEBUG_PRINT("SensorOutput::send")
		double outvals[1] = {outputValue()}; //only call once!
		if (outvals[0] != SensorizerServer::SENSOR_VALUE_NULL || cutoffTypeVal == CUTOFF_TYPE_VAL_NULLABLE) {
			//synchronized(dropdownMidiMappings) { 
			
				//DEBUG_PRINT_NUM("SensorOutput::midiMappingsCurlength ", midiMappingsCurlength)
				
				for (int i = 0; i < midiMappingsCurlength; i++) {
					if (dropdownMidiMappings[i] != NULL) {
						//DEBUG_PRINT_NUM("SensorOutput::dropdownMidiMappings->send()", outvals[0])
						dropdownMidiMappings[i]->send(outvals);
					}
				}
			//}
		}
		//DEBUG_PRINT("DONE SensorOutput::send")
		
		/// only tick every other time to slow it down.
		if (shouldTick) {
			for (int i = 0; i < midiMappingsCurlength; i++) {
				if (dropdownMidiMappings[i] != NULL) {
					dropdownMidiMappings[i]->tick();
				}
			}
		}
		shouldTick = !shouldTick;
	}

	void SensorOutput::addOutputFilter(/*string name, */ OutputFilter* filter) {
		if (outputFiltersCurlength < OUTPUT_FILTERS_LENGTH) {
			outputFilters[outputFiltersCurlength] = filter;
		
			outputFiltersCurlength++;
		}
		/*
		synchronized(outputFilters) { 
			//outputFilters = Arrays.copyOf(outputFilters, outputFilters.length + 1);
			OutputFilter[] si = new OutputFilter[outputFiltersCurlength + 1];
			if (outputFilters.length > 0)
				System.arraycopy(outputFilters, 0, si, 0, outputFilters.length);//Arrays.copyOf(sensorInputs, sensorInputs.length + 1);
			outputFilters = si;
			
			outputFilters[outputFilters.length - 1] = filter;
		}
		*/
	}
	
	/*
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
*/
	
	MidiMapping* SensorOutput::addMidiMapping(MidiMapping* map) {
		if (midiMappingsCurlength < MIDI_MAPPINGS_LENGTH) {
			dropdownMidiMappings[midiMappingsCurlength] = map;
		
			midiMappingsCurlength++;
		}
		
		return map;
	
	/*
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
		*/
	}

	/***
	 * assumes m has already been added.
	 * //@param m
	 */
	 /*
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
	*/
	
