/usr/local/opt/bison/bin/bison -d -v --output-file=src/parser/parser.cpp src/parser/impossible.ypp
/usr/local/opt/flex/bin/flex -c++ --outfile=src/parser/lexer.cpp src/parser/impossible.l