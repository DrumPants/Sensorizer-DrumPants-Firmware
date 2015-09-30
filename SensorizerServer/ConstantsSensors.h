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

#ifndef SensorizerConstantsSensors_h
#define SensorizerConstantsSensors_h


	#define DEFAULT_FOOT_PEDAL_SENSITIVITY_THRESHOLD 0.07
	#define FOOT_PEDAL_CUTOFF_LOW 0.23
	#define FOOT_PEDAL_HIGHPASS_FILTER_CONST 0.81
	#define FOOT_PEDAL_INRANGE_HIGH 0.75

	// always bump foot pedals up to maximum velocity.
	#define FOOT_PEDAL_OUTRANGE_LOW 0.85
	#define FOOT_PEDAL_OUTRANGE_HIGH 1.0

	#define PIEZO_OUTRANGE_LOW 0.25

	#if PRESET >= PRESET_PREPRODUCTION	

		#define BARE_PIEZOS 0

		#if BARE_PIEZOS
			// handle sensitive-ass bare peizos
			#define PIEZO_INRANGE_HIGH 1.0;
			#define PIEZO_CUTOFFRANGE_LOW 0.6
		#elif PRESET >= PRESET_PREPRODUCTION_CHINA
			// handle plastic, 24mm peizos
			#define PIEZO_INRANGE_HIGH 0.45;
			#define PIEZO_CUTOFFRANGE_LOW 0.1
		#else
			// handle plastic peizos
			#define PIEZO_INRANGE_HIGH 0.5909090909090908;
			#define PIEZO_CUTOFFRANGE_LOW 0.1
		#endif
	
		#define DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD DEFAULT_RETRIGGER_THRESHOLD_FOOT_PEDAL
	#elif PRESET == PRESET_BETA_RYAN
		// handle larger peizo type
		#define PIEZO_INRANGE_HIGH 0.56353861	
		#define PIEZO_CUTOFFRANGE_LOW 0.1

		#define DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD (DEFAULT_RETRIGGER_THRESHOLD_FOOT_PEDAL * 6)
	#else
		#define PIEZO_INRANGE_HIGH 0.16353861
		#define PIEZO_CUTOFFRANGE_LOW 0.1

		#define DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD (DEFAULT_RETRIGGER_THRESHOLD * 6)
	#endif

#endif