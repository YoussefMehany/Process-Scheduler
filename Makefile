build:
	gcc process_generator.c process_funcs.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c priority_queue.c -o scheduler.out
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out

test:
	./test_generator.out

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out

c:
	ipcrm -a
	clear