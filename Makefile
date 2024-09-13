CC=cc
CFLAGS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

INPUT_FILE=$(in)
OUTPUT_FILE=$(INPUT_FILE:.c=.out)

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(INPUT_FILE)
	$(CC) $(INPUT_FILE) $(CFLAGS) -o $(OUTPUT_FILE)

mygame: mygame.c
	$(CC) $(CFLAGS) mygame.c -o mygame.out

clean:
	rm -f *.out
