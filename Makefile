#
# Makefile for C++ version  of statistics (dtrace & kstat) server.
#

# Libraries
LDFLAGS=-ldtrace -lzmq -lkstat -lprotobuf
INCLUDE=-I/opt/local/include
LDFLAGS_LCL=$(LDFLAGS) -L/opt/local/lib/amd64 -L/opt/local/lib

HEADERS=tm_kstat.h tm_scripts.h tm_dtrace.h tm_util.h tm_zone.h
SOURCES=tm_dtrace.cc tm_kstat.cc pckt.pb.cc
PROTOBUF= pckt.pb.h pckt.pb.cc
OBJECTS:= $(SOURCES:.cc=.o)
SRVOBJ = tm_server.o

.PHONY: rel

all: tachyon-agent

tachyon-agent: libtachyon.a $(SRVOBJ)
	$(CXX) $(SRVOBJ) $(LDFLAGS_LCL) -L. -ltachyon -o tachyon-agent

libtachyon.a: $(OBJECTS)
	$(AR) $(ARFLAGS) libtachyon.a $(OBJECTS)

tm_dtrace.o: tm_dtrace.cc $(HEADERS) $(PROTOBUF)
tm_kstat.o: tm_kstat.cc $(HEADERS) $(PROTOBUF)
tm_server.o: tm_server.cc $(HEADERS) $(PROTOBUF)

pckt.pb.h pckt.pb.cc: pckt.proto
	protoc -I=. --cpp_out=. ./pckt.proto

rel: generator
	cp tachyon-agent meter/bin

clean:
	-rm -f pckt.pb.cc
	-rm -f pckt.pb.h
	-rm -f tachyon-agent
	-rm -f meter/bin/tachyon-agent
	-rm -f *.a *.o *~

