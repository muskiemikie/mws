CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -I../Engine

OBJS =		main.o Sample.o

LIBS =		-L../lib -lEngine

TARGET =	Sample_1.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
