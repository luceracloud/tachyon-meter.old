# Makefile for C++ version
# of statistics (dtrace &
# kstat) server.
#

# Compiler
CC=g++
# Libraries
LDFLAGS=-ldtrace -lzmq -lkstat -lprotobuf
LDFLAGS_LCL=$(LDFLAGS) -I/opt/local/include -L/opt/local/lib/amd64 -L/opt/local/lib

.PHONY: rel

# Instructions
all: generator

generator: server.cpp scripts.hpp pckt.pb.h pckt.pb.c
	$(CC) server.cpp pckt.pb.cc $(LDFLAGS_LCL) -O2 -o tachyon-agent

pckt.pb.h pckt.pb.c:
	protoc -I=. --cpp_out=. ./pckt.proto

rel: generator
	cp tachyon-agent tachyon-agent/bin

clean:
	-rm pckt.pb.cc
	-rm pckt.pb.h
	-rm tachyon-agent
	-rm meter/bin/tachyon-agent

