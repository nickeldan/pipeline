/*
Keywords

import
prod
drop
end
local
while
if
eif
else
continue
break
not
and
or
verify

*/

import os;
import math;

.source icount(start:Num, end:Num, step:Num = 1) {
	n=start
	while n <= end {
		prod n;
		n+=step;
	}
}

.predicate Posint(n) {n is Int and n > 0;}

.source factor(n:Posint) {
	x=n;
	s=n -> #math.sqrt;
	2,icount(3,s,2) -> local {
		recompute=false;
		while x%i == 0 {
			prod i;
			x/=i;
			recompute=true;
		}
		if recompute {s=x -> math.sqrt;}
	}
	if x > 1 {prod x;}
}

.sink doTakeFirst(x) {
	prod x;
	end;
}

.pipe isPrime(n:Posint) {
	factor(n) -> doTakeFirst:firstFactor;
	firstFactor == n;
}

.pipe filterPair(x, y:Bool) {
	if not y {drop;}
	x;
}

.MAIN {
	start=1 -> os.args -> #toInt;
	end=2 -> os.args -> #toInt;
	icount(start,end) -> @isPrime -> filterPair -> doPrint;
	prod 0;
}
