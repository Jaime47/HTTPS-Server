CC = gcc
CFLAGS = -g -Wall -Iincludes
EXE = server client

all : $(EXE)

server: server.o socket.a conf.a verbs.a
	$(CC) $(CFLAGS) -pthread -o server obj/server.o lib/socket.a lib/conf.a lib/verbs.a

server.o : src/server.c
	$(CC) $(CFLAGS) -c src/server.c -o obj/server.o

socket.o : srclib/socket.c
	$(CC) $(CFLAGS) -c srclib/socket.c -o obj/socket.o

socket.a: socket.o
	ar -rcs lib/socket.a obj/socket.o

conf.o : srclib/conf.c
	$(CC) $(CFLAGS) -c srclib/conf.c -o obj/conf.o

conf.a: conf.o
	ar -rcs lib/conf.a obj/conf.o

verbs.o : srclib/verbs.c
	$(CC) $(CFLAGS) -c srclib/verbs.c -o obj/verbs.o

verbs.a: verbs.o
	ar -rcs lib/verbs.a obj/verbs.o

picohttpparser.o : srclib/picohttpparser.c
	$(CC) $(CFLAGS) -c srclib/picohttpparser.c -o obj/picohttpparser.o

picohttpparser.a: picohttpparser.o
	ar -rcs lib/picohttpparser.a obj/picohttpparser.o

clean:
	rm -rvf obj/*.o lib/*.a *.tgz $(EXE) *.txt

dist : clean
	tar zcvf nombre.tgz makefile *.*
