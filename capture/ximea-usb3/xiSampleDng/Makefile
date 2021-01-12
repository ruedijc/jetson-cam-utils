CXX=g++
SOURCES=xiSampleDng.cpp
OBJECTS=$(SOURCES:.cpp=.o)
PROGRAM=xiSampleDng

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -g $(OBJECTS) -o $@ -lm3api -L/opt/XIMEA/lib -Wl,-rpath=/opt/XIMEA/lib -lxiapi_dng_store

.cpp.o: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) -g -c $< -o $@

clean:
	rm -f $(PROGRAM) $(OBJECTS)

install:
	cp $(PROGRAM) ../../bin
