CC := gcc
FLAGS := -Wall -std=c99

example: example.o cbuff.o
	${CC} ${FLAGS} -o $@ $^

.PHONY: clean
clean:
	@rm *.o
	@rm ./example
	@echo done