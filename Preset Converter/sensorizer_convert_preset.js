$(document).ready(function(){

function convertPreset(preset) {
	var code = "int i = 0;\n";
	for (i in preset) {
		for (j in preset['sensorInputs']) {
			for (k in preset['sensorInputs'][j]['sensorWindows']) {
				var out = preset['sensorInputs'][j]['sensorWindows'][k];
				//sensor output
				code += 
	"\n//" + preset['sensorInputs'][j]['name'] + "\n" +
	"SensorOutput* s = new SensorOutput();\n" +
	"s->inRange.low = " + out.inRangeP.low + ";\n" +
	"s->inRange.high = " + out.inRangeP.high + ";\n" +
	"s->outRange.low = " + out.outRangeP.low + ";\n" +
	"s->outRange.high = " + out.outRangeP.high + ";\n" +
	"s->cutoffRange.low = " + out.cutoffRangeP.low + ";\n" +
	"s->cutoffRange.high = " + out.cutoffRangeP.high + ";\n" +
	"s->setCutoffType(" + out.cutoffType.value.toFixed(0) + "); //" + out.cutoffType.label + "\n" +
	"s->multiplyVal = " + out.multiplyVal + ";\n" +
	"s->addVal = " + out.addVal + ";\n" +
	"s->isInvert = " + out.isInvert + ";\n" +
	"\n" +
	"MidiMapping* m = new MidiMapping(this->midiDevice);\n" +
	"m->channel = 1;\n" +
	"m->note = 60 + i;\n" +
	"s->addMidiMapping(m);\n" +
	"\n" +
	"sensorInputs[i++] = s;\n\n";
				
			}
		}
	}
	return code;
}


$('#presets').change(function (e) {
	$.ajax({
	  url: this.value,
	  dataType: 'json',
	  success: function(data, textStatus, jqXHR) {
			$("#preset_code").text(convertPreset(data));
		},
	  error: function(jqXHR, textStatus, errorThrown) { 
		$("#preset_code").text("ERROR: could not parse JSON. You may have to replace all '-Infinity' with 'null'. (" + errorThrown + ")");
	  }
	});
});


});