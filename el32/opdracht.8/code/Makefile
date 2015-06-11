#
#	(c) 2015, A.W. Janisse
# 
# 	make file for buidling teh Hue application
#  
BINARIES = hue

CC = gcc
CFLAGS = -g3 -Wall -W -Wshadow -Wcast-qual -Wwrite-strings
LDLIBS = -lcurl -ljson
HUE_AUX_OBJS = hue_aux.o hue.o

all:	$(BINARIES)

clean:
	rm *.o $(BINARIES)

hue: hue.o $(HUE_AUX_OBJS)
