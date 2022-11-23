CXX ?= g++

all:
	$(CXX) main.cpp -o vmdetector
install:
	sudo install -D -m 777 vmdetector /usr/local/bin
uninstall:
	sudo rm -f /usr/local/bin/vmdetector
