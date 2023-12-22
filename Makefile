# .PHONY: run

# build: clean-build
# 	MD build
# 	cd build; cmake .. -G "MinGW Makefiles"

# all: *.exe
# 	.\build\*.exe

# test:
# 	.\build\*_test

# clean-build:
# 	rd .\build\ /s /q

.PHONY: all run

all: run

run: build/OS_lab_exe.exe
	./build/OS_lab_exe.exe


