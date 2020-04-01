SOURCES_ASM=asm.cpp
HEADERS_ASM=classAssembler.hpp commands.hpp

SOURCES_CPU=cpu.cpp
HEADERS_CPU=commands.hpp

all: assembler processor

assembler: $(HEADERS_ASM) $(SOURCES_ASM)
	g++ $(SOURCES_ASM) $(HEADERS_ASM) -o asm

processor: $(HEADERS_CPU) $(SOURCES_CPU)
	g++ $(SOURCES_CPU) $(HEADERS_CPU) -o cpu
