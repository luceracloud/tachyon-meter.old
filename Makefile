#
# Makefile for C++ version  of statistics (dtrace & kstat) server.
#

# Libraries
#CXXFLAGS= -v -g -O2
CXXFLAGS= -g -std=gnu++0x
INCLUDE=-I/opt/local/include
LDFLAGS= -Bstatic -ldtrace -lcurl -lkstat -lprotobuf -lglog
LDFLAGS_LCL=-L/opt/local/lib/amd64 -L/opt/local/lib -L/usr/lib/amd64 -L/usr/lib $(LDFLAGS)  

HEADERS=tm_curl.h tm_kstat.h tm_kstat_data.h tm_scripts.h tm_util.h tm_zone.h
SOURCES=tm_curl.cc tm_kstat.cc tm_kstat_data.cc pckt.pb.cc
PROTOBUF= pckt.pb.h pckt.pb.cc
OBJECTS:= $(SOURCES:.cc=.o)
SRVOBJ = tm_server.o

.PHONY: rel

all: tachyon-agent

force-link:
	rm -f tachyon-agent

tachyon-agent: libtachyon.a $(SRVOBJ)
	$(CXX) -v $(LDFLAGS_LCL) -L. $(SRVOBJ) -ltachyon -o tachyon-agent

libtachyon.a: $(OBJECTS)
	$(AR) $(ARFLAGS) libtachyon.a $(OBJECTS)

kstat_server: libtachyon.a kstat_server.o
	$(CXX) -v $(LDFLAGS_LCL) -L. kstat_server.o -ltachyon -o kstat_server

kstat_demo: kstat_demo.c

kstat_server.o: kstat_server.cc $(HEADERS)
tm_curl.o: tm_curl.cc $(HEADERS) $(PROTOBUF)
tm_kstat.o: tm_kstat.cc $(HEADERS) $(PROTOBUF)
tm_server.o: tm_server.cc $(HEADERS) $(PROTOBUF)
tm_kstat_data.o: tm_kstat_data.cc $(HEADERS) $(PROTOBUF)
# tm_dtrace.o: tm_dtrace.cc $(HEADERS) $(PROTOBUF)

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

