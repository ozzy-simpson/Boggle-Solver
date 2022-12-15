CC=gcc
CFLAGS=-Wall -Wno-unused-variable -g
LDFLAGS=-lreadline -lm
BIN=onePlayerBoggle

all: $(BIN)

onePlayerBoggle: boggle.o hashmap.o llist.o onePlayerBoggle.c
	$(CC) $(CFLAGS) -o onePlayerBoggle onePlayerBoggle.c boggle.o hashmap.o llist.o $(LDFLAGS)

llist.o: llist.h llist.c
	$(CC) $(CFLAGS) -c -o llist.o llist.c 

hashmap.o: hashmap.h hashmap.c
	$(CC) $(CFLAGS) -c -o hashmap.o hashmap.c

boggle.o: boggle.c boggle.h
	$(CC) $(CFLAGS) -c -o boggle.o boggle.c

clean:
	rm -rf $(BIN) *.o *~ *.dSym
