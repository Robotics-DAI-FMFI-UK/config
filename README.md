# config

A very simple config in C language so that I do not have 
to depend on external libraries that change and break every two weeks.

It reads all file into memory first, then parses it using a state machine,
and finally realeases it.

See config.h for API and config_example.c for a simple example.
