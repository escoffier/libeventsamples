
objects = main.o echo.o
cc = g++

httpserver : $(objects)
	g++ -o echo -levent $(objects)
#main.o : main.cpp
#	cc -c main.cpp
	
.PHONY : clean
clean :
	rm edit $(objects)
