TARGET = syn_flood

REBUILDABLE = $(TARGET) *.o

all: $(TARGET)

$(TARGET): $(TARGET).o utility.o
	cc -g -o $@ $^

%.o: %.c
	cc -g -Wall -o $@ -c $^

clean:
	rm -f $(REBUILDABLE)