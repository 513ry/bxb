# Application
object   = bxb64
source   = ../src/[a-z]*.c

# Meta
log      = make.log

# Compilation
CC       = gcc
LIBS     = -lrt
CCFLAGS  = -std=c17 -D_GNU_SOURCE -pedantic

OPTIM    = -O3
DEBUG    = $(CCFLAGS) -Og -Wall -g

# Set default task
all: development

# --- TASKS ---
production: $(source)
	echo "Building production ELF.." > $(log)
	$(CC) -o $(object) $^ $(CCFLAGS) $(OPTIOM)

development: $(source)
	echo "Building development ELF.." > $(log)
	$(CC) -o $(object) $^ $(DEBUG)

.PHONY: clean
clean: ;rm -f $(object)
	rm -f $(log)
