var lookup = {HighQ:27,
Slap:28,
ScratchPush:29,
ScratchPull:30,
Sticks:31,
SquareClick:32,
MetronomeClick:33,
MetronomeBell:34,
AcousticBassDrum:35,
BassDrum1:36,
SideStick:37,
AcousticSnare:38,
HandClap:39,
ElectricSnare:40,
LowFloorTom:41,
ClosedHi_hat:42,
HighFloorTom:43,
PedalHi_hat:44,
LowTom:45,
OpenHi_hat:46,
Low_MidTom:47,
HighMidTom:48,
CrashCymbal1:49,
HighTom:50,
RideCymbal1:51,
ChineseCymbal:52,
RideBell:53,
Tambourine:54,
SplashCymbal:55,
Cowbell:56,
CrashCymbal2:57,
Vibra_slap:58,
RideCymbal2:59,
HighBongo:60,
LowBongo:61,
MuteHiConga:62,
OpenHiConga:63,
LowConga:64,
HighTimbale:65,
LowTimbale:66,
HighAgogo:67,
LowAgogo:68,
Cabasa:69,
Maracas:70,
ShortWhistle:71,
LongWhistle:72,
ShortGuiro:73,
LongGuiro:74,
Claves:75,
HiWoodBlock:76,
LowWoodBlock:77,
MuteCuica:78,
OpenCuica:79,
MuteTriangle:80,
OpenTriangle:81,
Shaker:82,
Jinglebell:83,
Belltree:84,
Castanets:85,
MuteSurdo:86,
OpenSurdo:87},




str = 
"	{\n" + 
"    ScratchPull,\n" + 
"    CrashCymbal2,\n" + 
"    ElectricSnare,\n" + 
"    HighMidTom, \n" + 
"    44, \n" + 
"    41},\n" + 
"	{\n" + 
"    ScratchPull,\n" + 
"    49,\n" + 
"    39,\n" + 
"    60, \n" + 
"    61, \n" + 
"    43},\n" + 
"	{\n" + 
"    ScratchPull,\n" + 
"    52,\n" + 
"    39,\n" + 
"    67, \n" + 
"    68, \n" + 
"    43},\n" + 
"	{\n" + 
"    ScratchPull,\n" + 
"    53,\n" + 
"    58,\n" + 
"    76, \n" + 
"    77, \n" + 
"    54}\n" + 
"#else\n" + 
"  {\n" + 
"    ScratchPull,\n" + 
"    CrashCymbal2,\n" + 
"    ElectricSnare,\n" + 
"    HighMidTom, \n" + 
"    44, \n" + 
"    41, 53, 58, 80},\n" + 
"  {\n" + 
"    ScratchPull,\n" + 
"    49,\n" + 
"    39,\n" + 
"    60, \n" + 
"    61, \n" + 
"    43, 53, 58, 80},\n" + 
"  {\n" + 
"    ScratchPull,\n" + 
"    52,\n" + 
"    39,\n" + 
"    67, \n" + 
"    68, \n" + 
"    43, 53, 58, 80},\n" + 
"  {\n" + 
"    ScratchPull,\n" + 
"    53,\n" + 
"    58,\n" + 
"    76, \n" + 
"    77, \n" + 
"    54, 67, 68, 80}\n",
 
result = str;


for (var i in lookup) {
	result = result.replace(new RegExp(lookup[i], 'g'), i);
}

