flex --header-file=lexer.hpp -o lexer.cpp lexer.l
bison -d -o parser.cpp parser.y
