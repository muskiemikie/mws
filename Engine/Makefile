CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -I../include

OBJS =		Engine.o Common.o VulkanCore.o

TARGET =	../lib/libEngine.a

$(TARGET):	$(OBJS)
	ar -r $@ $(OBJS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
