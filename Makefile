CC = gcc
TDIR = target
MY_FILES := $(wildcard my-own/*.c)
TLPI_FILES := $(wildcard tlpi/*.c)
MY_PROGS = $(patsubst %.c,%,$(MY_FILES))
TLPI_PROGS = $(patsubst %.c,%,$(TLPI_FILES))

default: target mine tlpi

target:
	mkdir -p $@/my-own
	mkdir -p $@/tlpi

mine: $(MY_PROGS)

tlpi: $(TLPI_PROGS)

%: %.c
	$(CC) -pthread -w -o $(TDIR)/$@ $<

clean:
	rm -rf target/
