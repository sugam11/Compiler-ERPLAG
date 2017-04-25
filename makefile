CC = gcc
CFLAGS = -Wall
DEPS = queue.h hashtable.h hashDef.h queueDef.h lexer.h parseTable.h parseTableDef.h parseTree.h parseTreeDef.h ast.h createParseTree.h symbolTable.h symbolTableDef.h
OBJ = driver.c lexer.c queue.c hashtable.c parseTable.c parseTree.c ast.c createParseTree.c symbolTable.c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

compiler: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ -g