CC = gcc
CFLAGS = -std=c99 -Wall -Werror -g

SOURCES = main.c database.c 
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = proj4

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean