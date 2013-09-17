$(document).ready(function(){

	var template = Mustache.compile($('#presetTemplate').text()),
		convertPreset = function convertPreset(preset) {
				var code = "int i = 0;\n" +
						"SensorOutput* s;\n" +
						"MidiMapping* m;\n";
				for (var i in preset) {
					for (var j in preset['sensorInputs']) {
						for (var k in preset['sensorInputs'][j]['sensorOutputs']) {
							var name = preset['sensorInputs'][j]['name'],
								out = preset['sensorInputs'][j]['sensorOutputs'][k];

							//sensor output
							code += template($.extend({}, {
									inputName : name,// we always use cutoffType "Nullable"
									cutoffTypeVal : 3 //out.cutoffType.value.toFixed(0)
								}, out));
							
						}
					}
				}
				return code;
			};

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

$('#pastedPreset').change(function (e) {
	$("#preset_code").text(convertPreset($.parseJSON(this.value)));
});


});