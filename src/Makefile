.PHONY: clean
all: a2q

a2q: aquery_parser_files aquery_lexer_files parser/symtable.c parser/symtable.h ast/ast.c ast/ast.h types/aquery_types.c types/aquery_types.h ast/ast_print.c ast/ast_print.h optimizer/optimizer.c optimizer/optimizer.h codegen/codegen.c codegen/codegen.h
	gcc parser/aquery.tab.c parser/lex.yy.c parser/symtable.c ast/ast.c types/aquery_types.c ast/ast_print.c optimizer/optimizer.c codegen/codegen.c -lm -o a2q

aquery_parser_files: parser/aquery.y
	bison -o parser/aquery.tab.c -d parser/aquery.y

aquery_lexer_files: parser/aquery.l 
	flex -o parser/lex.yy.c parser/aquery.l

#aquery_zip: aquery.l aquery.y Makefile ../../doc/grammar.pdf ../../doc/shiftreduce.pdf examples.a
#	zip aquery_zip aquery.l aquery.y Makefile ../../doc/grammar.pdf ../../doc/shiftreduce.pdf examples.a

clean:
	rm -rf a2q parser/lex.yy.c parser/aquery.tab.*
