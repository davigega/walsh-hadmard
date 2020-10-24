Walsh : MultiOutUGen {
	*ar {
		arg in, mul = 1.0, add = 0.0;
		^this.multiNewList(['audio']++ in.asArray)
	}
	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(inputs.size, rate);
	}
/*	*kr {
		arg in, mul = 1.0, add = 0.0;
		^this.multiNewList('control', in).madd(mul, add)
	}*/
}

// Pitch