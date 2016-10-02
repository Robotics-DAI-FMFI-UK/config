all: config_example

config_example: *.c *.h
	gcc -o config_example config_example.c config.c -std=c11 -D_BSD_SOURCE

clean:
	rm config_example
