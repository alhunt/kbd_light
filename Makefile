CC=gcc
CFLAGS=-g -Wall

kbd_light: kbd_light.o
	$(CC) kbd_light.o -o kbd_light 

kbd_light.o:
	$(CC) $(CFLAGS) -c kbd_light.c

clean:
	rm -rf ./*.o kbd_light

