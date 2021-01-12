CXX=g++
SOURCES=streamViewer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
PROGRAM=streamViewer
CFLAGS=-g

ifeq ($(GST10), 1)
	GSTMODULES=gstreamer-app-1.0 gstreamer-video-1.0
else
	GSTMODULES=gstreamer-app-0.10 gstreamer-interfaces-0.10
endif

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ -pthread `pkg-config --libs gtk+-2.0 $(GSTMODULES)` -lX11 -lm3api

.cpp.o: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) $(CFLAGS) -pthread `pkg-config --cflags gtk+-2.0 $(GSTMODULES)` -c $< -o $@

clean:
	rm -f $(PROGRAM) $(OBJECTS)

install:
	cp $(PROGRAM) ../../bin
