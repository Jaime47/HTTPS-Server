
CC=gcc

# Opciones de compilacion
CFLAGS= -Wall -Wextra -g

# Opciones de enlazado
# Directorios de los ficheros

$(shell mkdir -p obj bin lib)

SRCPATH = ./src/
SRCLIBPATH = ./srclib/
INCPATH = ./includes/
OBJPATH = ./obj/
EJCPATH = ./bin/
LIBRARYPATH = ./lib/

# Ejecutables
EJS = server
OBJECTSserver =  server.o conf.o picohttpparser.o my_lock.o socket.o verbs.o libsocket libpicoparse libverbs

#============================================================
# Generacion de ejecutables
#============================================================

all: $(EJS)

server: $(OBJECTSserver) 
	$(CC) $(CFLAGS) -o $(EJCPATH)server $(OBJPATH)server.o $(OBJPATH)conf.o $(OBJPATH)picohttpparser.o $(OBJPATH)my_lock.o $(OBJPATH)socket.o $(OBJPATH)verbs.o -lconfuse -lpthread

#=============================================================
# Generación de archivos .o
#=============================================================

server.o: $(SRCPATH)server.c $(INCPATH)conf.h $(INCPATH)my_lock.h $(INCPATH)socket.h $(INCPATH)verbs.h
	$(CC) $(CFLAGS) -c $(SRCPATH)server.c -o $(OBJPATH)server.o

conf.o: $(SRCLIBPATH)conf.c $(INCPATH)conf.h
	$(CC) $(CFLAGS) -c $(SRCLIBPATH)conf.c -o $(OBJPATH)conf.o

picohttpparser.o: $(SRCLIBPATH)picohttpparser.c $(INCPATH)picohttpparser.h
	$(CC) $(CFLAGS) -c $(SRCLIBPATH)picohttpparser.c -o $(OBJPATH)picohttpparser.o

my_lock.o: $(SRCLIBPATH)my_lock.c $(INCPATH)my_lock.h $(INCPATH)picohttpparser.h
	$(CC) $(CFLAGS) -c $(SRCLIBPATH)my_lock.c -o $(OBJPATH)my_lock.o

socket.o: $(SRCLIBPATH)socket.c $(INCPATH)socket.h
	$(CC) $(CFLAGS) -c $(SRCLIBPATH)socket.c -o $(OBJPATH)socket.o

verbs.o: $(SRCLIBPATH)verbs.c $(INCPATH)verbs.h
	$(CC) $(CFLAGS) -c $(SRCLIBPATH)verbs.c -o $(OBJPATH)verbs.o

libsocket: $(OBJPATH)socket.o
	ar cr $(LIBRARYPATH)libsocket.a $(OBJPATH)socket.o

libverbs: $(OBJPATH)verbs.o
	ar cr $(LIBRARYPATH)libverbs.a $(OBJPATH)verbs.o

libpicoparse: $(OBJPATH)picohttpparser.o
	ar cr $(LIBRARYPATH)libpicoparse.a $(OBJPATH)picohttpparser.o

clean:
	rm -rf $(EJCPATH)server $(OBJPATH)*.o $(STATICPATH)*.a
	rm -r obj bin lib
