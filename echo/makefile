
objects = main.o echo.o
cc = g++

httpserver : $(objects)
	g++ -o echo $(objects) -levent
#main.o : main.cpp
#	cc -c main.cpp
	
.PHONY : clean
clean :
	rm edit $(objects)
