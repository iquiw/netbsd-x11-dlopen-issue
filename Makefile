all: xim-dl

xim-dl: xim-dl.c
	gcc -I/usr/X11R7/include -o xim-dl xim-dl.c
