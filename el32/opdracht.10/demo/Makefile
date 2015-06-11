CC 		= arm-linux-gcc		
CFLAGS 	= -O2 -Wall -Werror
LDFLAGS = -lusb-1.0
HEADERS = xboxdemo.h
SOURCES = xboxdemo.c
OBJECTS = $(SOURCES:.c=.o)
TARGET 	= xboxdemo

.PHONY: all clean

all: $(TARGET)
 
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJECTS): $(HEADERS)

install:
	@scp $(TARGET) root@10.0.0.42:/bin

clean:
	@rm -rf $(TARGETS) $(OBJECTS)

