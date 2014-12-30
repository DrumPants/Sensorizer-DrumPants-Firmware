/***
Generates scales
***/

//var n = [0, 60, 62, 63, 65, 67];
//major pent
//var n = [0, 60, 64, 65, 67, 69];
//minor pentatonic
var // length of the final array we want.
	TOTAL_ARRAY_LEN = 12,
	scales = {
		major : [0, 2, 4, 5, 7, 9, 11, 12],
		minor : [0, 2, 3, 5, 7, 8, 10, 12],
		melodicMinor : [0, 2, 3, 5, 7, 9, 11, 12],
		gypsy : [0, 2, 3, 5, 6, 8, 10, 12],
		persian : [0, 1, 4, 5, 7, 8, 10, 12]
	},
	roots = [60 - 12 * 2, 60, 60 + 12 * 2],
	results = {
		ccode: "",
		ccodeAlts: "",
		table : "",
		tableAlt: ""
	},
	ccode = "ccode",
	ccodeAlts = "ccodeAlts",
	table = "table",
	tableAlt = 'tableAlt',
	curTableIdx = 0,
	curTableAltIdx = 0,
	printCode = function (dest, str) {
		console.log(str);
		results[dest] += str + "\n";
	},
	swapInArray = function (arr, fromIdx, toIdx) {
		var tmp = arr[toIdx];

		arr[toIdx] = arr[fromIdx];
		arr[fromIdx] = tmp;

		return arr;
	},

	capitalize = function (text) {
	    return text.charAt(0).toUpperCase() + text.slice(1).toLowerCase();
	},

	keylabel = ["B","C","C#/Db","D","D#/Eb","E","F","F#/Gb","G","G#/Ab","A","A#/Bb"],
	numToKey = function (num) {
		var idx = (num % 12);

		return keylabel[idx];
	},
	numToOctave = function (num) {
		var octave = 5;

		if (num < 60) {
			octave = 4;
		}
		else if (num > 72) {
			octave = 6;
		}

		return octave;
	};


for (var key = 0; key < 12; key++) {

	for (var keyIdx = 0; keyIdx < roots.length; keyIdx++) {
		roots[keyIdx] += 1;
	}

	printCode(ccode, '// Key ' + roots[1]);
	printCode(ccodeAlts, '// Key ' + roots[1]);

	for (var k in scales) { 
		var notes = scales[k];

		printCode(ccode, '// Scale ' + k);
		printCode(ccodeAlts, '// Alt Scale ' + k);

		for (var r = 0; r < roots.length; r++) {
			var res = [];

		    for (j in notes) {
		        res[j] = notes[j] + roots[r];
		    }
		    for (var ext = res.length; ext < TOTAL_ARRAY_LEN; ext++) {
		    	res[ext] = notes[(ext - notes.length) + 1] + (roots[r] + 12);
		    }

		    printCode(ccode, "{" + res.join(", ")  + "},");

		    // make pentatonic version (hexatonic, really)
		    res = swapInArray(res, 3, 6); // take out 4th, replace with 7th
		    printCode(ccodeAlts, "{" + res.join(", ")  + "},");

		    if (k == 'major') {
				printCode(table, '<tr><td>' + curTableIdx++ + '</td><td>' + numToKey(roots[r]) + '</td><td>' + numToOctave(roots[r]) + '</td><td>' + capitalize(k) + '</td></tr>');	
			}
			else if (roots[1] == 61) {
				printCode(tableAlt, '<tr><td>' + (39 + curTableAltIdx++) + '</td><td>' + numToKey(roots[r]) + '</td><td>' + numToOctave(roots[r]) + '</td><td>' + capitalize(k) + '</td></tr>');	
			}
		}

	}
}

console.log(results.ccode);
console.log(results.ccodeAlts);