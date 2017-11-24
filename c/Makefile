CC = cc
CFLAGS = -std=c99 -Wall -g
LFLAGS =
FILES = khalang
PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	LFLAGS += -ledit -lm
	FILES += prompt
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	LFLAGS += -ledit -lm
	FILES += prompt
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	FILES += prompt
endif

all: $(FILES)

%: %.c mpc.c
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@
