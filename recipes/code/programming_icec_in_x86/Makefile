ICEC_SRC = /usr/src/IceC
DIRSLICE = /usr/share/slice/st/
TARGET = example_x86
SLICE = st

CC = gcc
CFLAGS += -I$(ICEC_SRC) 
CFLAGS += -Wall -Wextra -pedantic -Wno-variadic-macros -Wno-unused-parameter \
	   -Wno-unused-function



vpath %.c $(ICEC_SRC)
vpath %.c $(ICEC_SRC)/platforms/x86

all: $(SLICE).h $(TARGET)

$(SLICE).h: 
	slice2c $(DIRSLICE)$(SLICE).ice

$(TARGET): $(TARGET).c IceC.o IceUtil.o TCPEndpoint.o port.o
	$(CC) $(CFLAGS) $^ -o $@

run:
	./$(DIREXE)$(TARGET)

clean: 
	find -name "*~" -print -delete
	find -name "*.o" -print -delete
	$(RM) $(TARGET) $(SLICE).h


