CXX=g++
SOURCES=xiApiPlusOcvExample.cpp
SOURCES+=xiApiPlusOcv.cpp
OBJECTS=$(SOURCES:.cpp=.o)
PROGRAM=xiApiPlusOcvExample


all: $(PROGRAM)

$(PROGRAM): xiApiPlusOcv.o xiApiPlusOcvExample.o
	g++ $(OBJECTS) -o xiApiPlusOcvExample -lm3api -lopencv_core -lopencv_highgui -lopencv_imgproc -L/usr/local/lib 

xiApiPlusOcv.o: xiApiPlusOcv.cpp
	g++ -c xiApiPlusOcv.cpp -I . -I /usr/local/include/ -g3 -Wall -c -fmessage-length=0

xiApiPlusOcvExample.o: xiApiPlusOcvExample.cpp
	g++ -c xiApiPlusOcvExample.cpp -I .  -I /usr/local/include/ -g3 -Wall -c -fmessage-length=0

clean:
	rm -f *.o *~ $(PROGRAM) $(OBJECTS)






