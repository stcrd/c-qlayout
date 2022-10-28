precalc: optimizePrecalc.c
	gcc -o effort.o -c effort.c && gcc -o optimizePrecalc optimizePrecalc.c effort.o -lm -lbsd && rm effort.o && ./optimizePrecalc
draw: drawLayout.c
	gcc -o drawLayout drawLayout.c && ./drawLayout
limit: optimizeLimit.c
	gcc -o optimizeLimit optimizeLimit.c -lm -lbsd && ./optimizeLimit
count: countTrigrams.count
	gcc -o countTrigrams countTrigrams.c && ./countTrigrams
neighbor: optimizeNeighborPrecalc.c
	gcc -o optimizeNeighborPrecalc optimizeNeighborPrecalc.c -lm -lbsd && ./optimizeNeighborPrecalc