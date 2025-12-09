CC = g++
BISON = bison
FLEX = flex

TARGET = compilador
SOURCES = parser.tab.cpp lex.yy.c ast/ASTPrinter.cpp semantic/SemanticChecker.cpp ast/CodeGenerator.cpp

all: $(TARGET)

parser.tab.cpp parser.tab.hh: parser.y
	$(BISON) -d parser.y -o parser.tab.cpp

lex.yy.c: scanner.l
	$(FLEX) -o lex.yy.c scanner.l

$(TARGET): $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES) -lfl

clean:
	rm -f $(TARGET) parser.tab.cpp parser.tab.hpp lex.yy.c *.mepa

.PHONY: all clean
