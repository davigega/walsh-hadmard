Walsh : MultiOutUGen {
	*ar {
		arg in, mul = 1.0, add = 0.0;
		^this.multiNewList(['audio']++ in.asArray)
	}
	checkInputs {
		var n = inputs.size;
		inputs.do({
			|proxy, i|
			if (proxy.rate != 'audio'){^("input at"+i+"is not at audio rate")}
		});
		if(n.log2.ceil != n.log2.floor){^"the size of the input array is not a power of 2"}
		^this.checkValidInputs
	}
	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(inputs.size, rate);
	}
}
