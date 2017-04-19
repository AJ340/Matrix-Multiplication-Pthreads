GCC      = gcc

SRCS       = AQ-UNIX-HW10.c
OBJS       = $(patsubst %.c,%.o,$(SRCS))
EXECS      = AQ-UNIX-HW10

CFLAGS     = -Wall 


%.o : %.c
	$(GCC) -c $< -o $@ $(CFLAGS) 

all:
	make $(EXECS)

AQ-UNIX-HW10: $(OBJS)
	$(GCC) -o $@ $(OBJS) $(CFLAGS) 


clean:
	-rm -f *.o


cleanall:
	-rm -f $(OBJS) $(EXECS) 
