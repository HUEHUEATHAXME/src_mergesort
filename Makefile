all: compile random sorted reversed plot plot_compare

random:
	./mergesort_random 262144 mergesort_random.data

sorted:
	./mergesort_sorted 262144 mergesort_sorted.data

reversed:
	./mergesort_reversed 262144 mergesort_reversed.data

compile:
	gcc -o mergesort_random testsort.c mergesort.c time.c
	gcc -o mergesort_sorted testsort_sorted.c mergesort.c time.c
	gcc -o mergesort_reversed testsort_reversed.c mergesort.c time.c

plot_compare:
	gnuplot sortperf_compare.plot
	ps2pdf sortperf_compare.ps
	xpdf sortperf_compare.pdf

plot:
	gnuplot sortperf_mergesort.plot
	ps2pdf sortperf_mergesort.ps
	xpdf sortperf_mergesort.pdf

clean:
	rm -f *.o
	rm -f *~
	rm -f ./mergesort_random ./mergesort_sorted ./mergesort_reversed
	rm -f ./mergesort_random.data ./mergesort_sorted.data ./mergesort_reversed.data
	rm -f *.ps
