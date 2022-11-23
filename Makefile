CXX ?= g++

all: compile
compile:
	$(CXX) main.cpp -o vmdetector
install: compile
	sudo install -D -m 777 vmdetector /usr/local/bin
uninstall:
	sudo rm -f /usr/local/bin/vmdetector
