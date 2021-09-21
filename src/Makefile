BIN = pascal

LEX = flex
YACC = bison
CC = g++ -g
LLVM_CONFIG = llvm-config

CPPFLAGS = `$(LLVM_CONFIG) --cppflags` -std=c++11 $(NO_WARNING) -c
LDFLAGS = `$(LLVM_CONFIG) --ldflags --libs --system-libs`

SRCS = token.cpp parser.cpp main.cpp AST.cpp CodeGenContext.cpp errorHandle.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
OBJ_DIR = build

NO_WARNING = -Wno-write-strings

all: $(BIN)
	cp $(OBJ_DIR)/$^ test/$(BIN)

$(BIN): $(OBJS)
	cd $(OBJ_DIR) && $(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $(OBJ_DIR)/$@ $^ $(CPPFLAGS)

parser.cpp parser.h: pascals.y
	bison --defines=parser.h -o parser.cpp $^ -Wno-other

token.cpp: pascals.l parser.h
	flex -o $@ pascals.l

test_compiler: test/pascal test/test19.pas
	-cd test && ./pascal -I test19.pas

test_compile_code_gen_context: CodeGenContext.cpp
	$(CC) -o $(OBJ_DIR)/CodeGenContext CodeGenContext.cpp $(CPPFLAGS)

test_compile_ast: AST.cpp
	$(CC) -o $(OBJ_DIR)/AST AST.cpp $(CPPFLAGS)

clean_test_folder:
	-cd test && rm `ls | grep -v ".pas"`
	cp $(OBJ_DIR)/$(BIN) test/$(BIN)

.PHONY: clean
clean:
	-rm token.cpp parser.cpp parser.h build/*
	-cd test && rm `ls | grep -v ".pas"`
