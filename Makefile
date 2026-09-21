CC = gcc
CFLAGS = -O2 -Wall -pthread

.PHONY: all parte1 parte2 clean

all: parte1 parte2

parte1:
	$(CC) $(CFLAGS) parte1/contacorrente_seq.c -o parte1/contacorrente_seq
	$(CC) $(CFLAGS) parte1/contacorrente_f1.c -o parte1/contacorrente_f1
	$(CC) $(CFLAGS) parte1/contacorrente_f2.c -o parte1/contacorrente_f2

parte2:
	$(CC) $(CFLAGS) parte2/primos.c -o parte2/primos
	$(CC) $(CFLAGS) parte2/monte_carlo_pi.c -o parte2/monte_carlo_pi
	$(CC) $(CFLAGS) parte2/soma_vetores.c -o parte2/soma_vetores

clean:
	rm -f parte1/contacorrente_seq
	rm -f parte1/contacorrente_f1
	rm -f parte1/contacorrente_f2
	rm -f parte2/primos
	rm -f parte2/monte_carlo_pi
	rm -f parte2/soma_vetores