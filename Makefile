all: directories parent child1 

directories: 
	mkdir -p build

parent: src/parent.c
	g++ src/parent.c -g -o build/parent

child1: src/child1.c
	g++ src/child1.c -g -o build/child1


run: task 

task: build/parent 
	build/parent

# open:
# 	./parent