OBJS = driver.o scanner.o value.o node.o stack.o queue.o bst.o convert.o calc.o
OPTS = -Wall -Wextra -g -std=c99

calculon: $(OBJS)
	gcc $(OPTS) $(OBJS) -o calculon -lm

driver.o: driver.c scanner.h value.h node.h stack.h queue.h bst.h convert.h
	gcc $(OPTS) -c driver.c

scanner.o: scanner.c scanner.h
	gcc $(OPTS) -c scanner.c

value.o: value.c value.h
	gcc $(OPTS) -c value.c

node.o: node.c node.h value.h
	gcc $(OPTS) -c node.c

stack.o: stack.c stack.h value.h node.h
	gcc $(OPTS) -c  stack.c

queue.o: queue.c queue.h value.h node.h
	gcc $(OPTS) -c queue.c

bst.o:	bst.c	bst.h	value.h	node.h
	gcc $(OPTS) -c bst.c

convert.o: convert.c convert.h value.h stack.h queue.h node.h
	gcc $(OPTS) -c convert.c

calc.o: calc.c calc.h stack.h queue.h node.h bst.h
	gcc $(OPTS) -c calc.c

test: calculon
	calculon in.txt

clean:
	rm -f calculon $(OBJS)
