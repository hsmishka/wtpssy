CXX ?= g++

CXXFLAGS += -c -Wall $(shell pkg-config --cflags opencv) -L/usr/lib/uv4l/uv4lext/armv6l -luv4lext -Wl,-rpath,'/usr/lib/uv4l/uv4lext/armv6l'
LDFLAGS += $(shell pkg-config --libs --static opencv)

all: segment_objects

segment_objects: segment_objects.o ServoCtrl.o; $(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp; $(CXX) $^ -o $@ $(CXXFLAGS)

clean: ; rm -f *.o segment_objects
