CC=gcc
COMPILE=-g -c -std=c99 -Wall -pedantic
LINK= -g


wordpairs: main.o crc64.o ht.o getWord.o
	$(CC) $(LINK) -o wordpairs main.o crc64.o ht.o getWord.o

main.o:	main.c ht.h getWord.h
	$(CC) $(COMPILE) main.c

getWord.o: getWord.h getWord.c
	$(CC) $(COMPILE) getWord.c

crc64.o: crc64.c crc64.h 
	$(CC) $(COMPILE) crc64.c

ht.o: ht.c ht.h crc64.h
	$(CC) $(COMPILE) ht.c

clean:
	rm *.o wordpairs