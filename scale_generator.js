/***
Generates scales
***/

//var n = [0, 60, 62, 63, 65, 67];
//major pent
//var n = [0, 60, 64, 65, 67, 69];
//minor pentatonic
var scales = {
		major : [0, 2, 4, 5, 7, 9, 11, 12],
		minor : [0, 2, 3, 5, 7, 8, 10, 12],
		melodicMinor : [0, 2, 3, 5, 7, 9, 11, 12],
		gypsy : [0, 2, 3, 5, 6, 8, 10, 12],
		persian : [0, 1, 4, 5, 7, 8, 10, 12]
	},
	roots = [60 - 12 * 2, 60, 60 + 12 * 2],
	ccode = "",
	printCode = function (str) {
		console.log(str);
		ccode += str + "\n";
	};



for (var k in scales) { 
	var n = scales[k];

	printCode('// Scale ' + k);

	for (var r = 0; r < roots.length; r++) {
		var res = [];

	    for (j in n) {
	        res[j] = n[j] + roots[r];
	    }
	    console.log(res);

	    printCode("{" + res.join(",")  + "},");
	}

}

console.log(ccode);