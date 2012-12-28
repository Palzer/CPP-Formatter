CC=g++
CFLAGS= -g -Wall

FORMAT_SRCS=format.cpp
FORMAT_OBJS= ${FORMAT_SRCS:.cpp=.o}

format: $(FORMAT_OBJS) 
	${CC} -o format $(FORMAT_OBJS)	

clean:
	rm -r format $(FORMAT_OBJS)
	
#$(OBJS): serverdata.h deck.h clientdata.h
