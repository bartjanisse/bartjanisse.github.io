#
#	Makefile for IPC32
#
#	(c) Fontys 2010, Joris Geurts
#

BINARIES = TCPEchoClient TCPEchoServer TCPEchoServer-Fork TCPEchoServer-Thread TCPChatClient TCPChatServer

CC = gcc
CFLAGS = -g3 -Wall -W -Wshadow -Wcast-qual -Wwrite-strings
LDLIBS = -lpthread
CLIENT_AUX_OBJS = Auxiliary.o CreateTCPClientSocket.o
SERVER_AUX_OBJS = Auxiliary.o CreateTCPServerSocket.o AcceptTCPConnection.o HandleTCPClient.o
CHAT_CLIENT_AUX_OBJS = Auxiliary.o CreateTCPClientSocket.o
CHAT_SERVER_AUX_OBJS = Auxiliary.o CreateTCPServerSocket.o AcceptTCPConnection.o HandleTCPChatClient.o

all:	$(BINARIES)

clean:
	rm *.o $(BINARIES)

TCPEchoClient: TCPEchoClient.o $(CLIENT_AUX_OBJS)

TCPEchoServer: TCPEchoServer.o $(SERVER_AUX_OBJS)

TCPChatClient: TCPChatClient.o $(CLIENT_AUX_OBJS)

TCPChatServer: TCPChatServer.o $(CHAT_SERVER_AUX_OBJS)

TCPEchoServer-Fork: TCPEchoServer-Fork.o $(SERVER_AUX_OBJS)

TCPEchoServer-Thread: TCPEchoServer-Thread.o $(SERVER_AUX_OBJS) $(LDLIBS)


