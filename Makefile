build:
	gcc process_generator.c process_funcs.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -lm memory.c priority_queue.c circularQueue.c -o scheduler.out
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out

garab:
	gcc memory.c test.c -o test.out

test:
	./test_generator.out

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out

runv:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./process_generator.out 3 "processes.txt"
c:
	ipcrm -a
	clear
	
GUI: 
	# gcc -o GUI.out GUI.c $(pkg-config --cflags --libs gtk+-3.0)
	./GUI.out

js:
	electron main.js

youssef:
	git config --global core.autocrlf input