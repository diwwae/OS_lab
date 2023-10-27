all: directories parent child

directories: 
	mkdir -p build

parent: src/parent.c
	gcc src/parent.c -g -o build/parent

child: src/child.c
	gcc src/child.c -g -o build/child


run: task 

task: build/parent 
	build/parent

# open:
# 	./parent