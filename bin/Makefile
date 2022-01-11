# Application
object   := bx64
source   := ../src/[a-z]*.c

# Meta
log      := make.log

# Compilation
CC       := gcc
CCFLAGS  := -std=c17 -D_GNU_SOURCE -pedantic

# TODO: 

LIBS     := -lrt

OPTIM    := -O3
DEBUG    := $(CCFLAGS) -g -Og -W -Wall -Wextra

.PHONY: all production development cpp clean clean_all

all: clean development

# --- TASKS ---
production: $(source)
	echo "Building production ELF" $(shell date --iso-8601=seconds) >> $(log)
	$(CC) -o $(object) $^ $(CCFLAGS) $(OPTIOM)
	echo "DONE" >> $(log)

development: $(source)
	echo "Building development ELF $(shell date --iso-8601=seconds)" >> $(log)
	$(CC) -o $(object) $^ $(DEBUG) 2>> $(log)
	echo "DONE" >> $(log)

clean: ;rm -rf $(object) *.exe *.o
	echo "Clean build the directory" >> $(log)

# Those methods will need root access
clean_assets:
	rm -rf *.log ../assets/*crowimage* /bin/$(object)
