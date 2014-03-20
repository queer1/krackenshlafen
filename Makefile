CC=gcc
LIBWPS_DIR=libwps
INC=-I$(LIBWPS_DIR)
#prefix=/usr/local
Dexec_prefix=${prefix}
#CONFDIR=${prefix}/etc/reaver
#CFLAGS=-DCONF_DIR='"$(CONFDIR)"' -DREAVER_DATABASE='"$(CONFDIR)/reaver.db"'  -g 
LDFLAGS=$(LIBWPS_DIR)/*.o wps/*.o tls/bignum.o tls/libtls.a utils/libutils.a crypto/libcrypto.a lwe/libiw.a -ldl -lm -lpcap -lsqlite3 

all: krakenmapen

krakenmapen: libiw argsparser.o ap_list.o globule.o init.o sigint.o sigalrm.o misc.o 80211.o iface.o send.o session.o 
	$(CC) $(CFLAGS) $(INC) krakenmapen.c *.o $(LDFLAGS) -o krakenmapen

libwps.o:
	(cd $(LIBWPS_DIR) && make)

wps: libutils libcrypto
	(cd wps && make)

#libutils:
#	(cd utils && make)

#libcrypto: libtls
#	(cd crypto && make)

#libtls:
#	(cd tls && make)

libiw:
	(cd lwe && make BUILD_STATIC=y libiw.a)

init.o:
	$(CC) $(CFLAGS) init.c -c

sql.o:
	$(CC) $(CFLAGS) $(INC) sql.c -c

crc.o:
	$(CC) $(CFLAGS) crc.c -c

keys.o:
	$(CC) $(CFLAGS) keys.c -c

argsparser.o: globule.o
	$(CC) $(CFLAGS) $(INC) argsparser.c -c

sigint.o: globule.o
	$(CC) $(CFLAGS) $(INC) sigint.c -c

exchange.o: globule.o send.o sigalrm.o 80211.o
	$(CC) $(CFLAGS) $(INC) exchange.c -c

send.o: globule.o builder.o sigalrm.o
	$(CC) $(CFLAGS) send.c -c

session.o: globule.o
	$(CC) $(CFLAGS) $(INC) session.c -c

80211.o: globule.o sql.o builder.o crc.o
	$(CC) $(CFLAGS) $(INC) 80211.c -c

iface.o: globule.o
	$(CC) $(CFLAGS) iface.c -c

sigalrm.o: globule.o
	$(CC) $(CFLAGS) sigalrm.c -c

misc.o: globule.o 
	$(CC) $(CFLAGS) misc.c -c

builder.o: globule.o
	$(CC) $(CFLAGS) builder.c -c

pins.o: globule.o keys.o
	$(CC) $(CFLAGS) pins.c -c

cracker.o: globule.o init.o pins.o iface.o exchange.o session.o 80211.o
	$(CC) $(CFLAGS) $(INC) cracker.c -c

globule.o:
	$(CC) $(CFLAGS) globule.c -c 

ap_list.o:
	$(CC) $(CFLAGS) ap_list.c -c 

install: cleanbin
	if [ ! -d $(CONFDIR) ]; then mkdir -p $(CONFDIR); fi
	if [ ! -e $(CONFDIR)/reaver.db ]; then cp reaver.db $(CONFDIR)/reaver.db && chmod -R a+rw $(CONFDIR); fi
	if [ -e wash ]; then cp wash ${exec_prefix}/bin/wash; fi
	if [ -e reaver ]; then cp reaver ${exec_prefix}/bin/reaver; fi

clean:
	rm -f *~ *.o krakenshalfen
	rm -rf *.cache config.status config.log
	(cd utils && make clean)
	(cd tls && make clean)
	(cd crypto && make clean)
	(cd wps && make clean)
	(cd lwe && make realclean)
	(cd $(LIBWPS_DIR) && make clean)
#	rm -f Makefile config.h

cleanbin: 
	rm -f ${exec_prefix}/bin/reaver ${exec_prefix}/bin/wash ${exec_prefix}/bin/walsh

distclean: cleanbin clean
	rm -rf $(CONFDIR)
