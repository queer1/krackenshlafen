CC=gcc
LDFLAGS= -lpcap  
CFLAGS = -g


krakenmapen: krakenmapen.c sniffer.o
	$(CC) $(CFLAGS) $(LDFLAGS)  krakenmapen.c ap_list.c sniffer.o -o krakenmapen


krakenshlafen: krakenshlafen.c sniffer.o
	$(CC) $(CFLAGS) $(LDFLAGS)  krakenshlafen.c sniffer.o -o krakenshlafen

sniffer.o: sniffer.c
	$(CC) $(CFLAGS) $(LDFLAGS) sniffer.c -c 
