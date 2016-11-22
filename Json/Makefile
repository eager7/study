default:test

SOURCE:=
test.o:test.c test.h

#OBJS := $(SOURCE:.c=.o)

OBJS+=test.o

test:$(OBJS)
	#echo $(OBJS)
	gcc $< -o $@ -ljson-c 
clean:
	rm test $(OBJS)
