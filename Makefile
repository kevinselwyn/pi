NAME    := pi
BIN_DIR := /usr/local/bin

all: build

build: $(NAME).c
	gcc -o $(NAME) $< -lm

download:
	curl -o digits.txt https://stuff.mit.edu/afs/sipb/contrib/pi/pi-billion.txt

test: build
	./$(NAME) -s digits.txt.compress 0

install: $(NAME)
	install -m 0755 $(NAME) $(BIN_DIR)

uninstall:
	rm -f $(BIN_DIR)/$(NAME)

clean:
	rm -f $(NAME)