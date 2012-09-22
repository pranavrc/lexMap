CFLAGS = -Wall -g
all:
	gcc keyMapper.c $(CFLAGS) -o keyMapper
clean:
	rm -f keyMapper
