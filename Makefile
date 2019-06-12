all:
	gcc -Wall -pedantic -std=c99 -Og main.c -o uicp -pthread -lpthread -lraylib -lGL -lm -ldl -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
compile:
