CC=gcc
LIBSOCKET=-lnsl
CCFLAGS=-Wall -g
SRV=server
CLT=client
CLT2=client2
CLT3=client3

all: $(SRV) $(CLT)

$(SRV):$(SRV).c
	$(CC) $(LIBSOCKET) $(CCFLAGS) $(SRV).c -o $(SRV)

$(CLT):	$(CLT).c
	$(CC) $(LIBSOCKET) $(CCFLAGS) $(CLT).c -o $(CLT)

clean:
	rm -f *.o *~
	rm -f $(SRV) $(CLT)
