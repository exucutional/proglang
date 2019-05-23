CC = gcc
ASM = nasm
LD = gcc
SFML_DIR= C:/SFML
LIBDIR= $(SFML_DIR)/lib
CFLAGS =-c -g -O3 -Wall -I$(SFML_DIR)/include
AFLAGS = -f elf64	
LDFLAGS = -g -no-pie
SFML = -lsfml-graphics -lsfml-window -lsfml-system 
VPATH = ./src
OBJPATH = ./compile
SRCC = cpu/cpu_asm.c cpu/cpu_t.c tree_t.c tokenization.c parsing.c translator.c unit_test.c prog_run.c
HEAD = asprintf.h keywords.h parsing.h terminals.h tokenization.h translator.h tree_t.h
SRCASM = 
OBJC = $(SRCC:%.c=$(OBJPATH)/%.o)
OBJASM = $(SRCASM:%.S=$(OBJPATH)/%.o)
EXECUTABLE_LINUX = run.out ../comp.out
EXECUTABLE_WINDOWS = run.exe

windows: $(OBJPATH) $(SRCC) $(SRCASM) $(EXECUTABLE_WINDOWS)

linux: $(OBJPATH) $(SRCC) $(SRCASM) $(EXECUTABLE_LINUX)

$(OBJPATH):
	@mkdir $@

$(EXECUTABLE_LINUX): $(OBJC) $(OBJASM)
	@echo "LINKING:"
	$(LD) $(LDFLAGS) $(OBJC) -o  $@ $(SFML)

$(EXECUTABLE_WINDOWS): $(OBJC) $(OBJASM)
	@echo "LINKING:"
	$(LD) $(LDFLAGS) $(OBJC) -o  $@ -L$(LIBDIR) $(SFML)

$(OBJPATH)/%.o: %.c $(HEAD)
	@echo "COMPILING:"
	$(CC) $(CFLAGS) $< -o $@

$(OBJPATH)/%.o: %.S
	@echo "COMPILING:"
	$(ASM) $(AFLAGS) $< -o $@