

main: main.cpp FTree.h
	g++ -g -o main.o main.cpp FTree.h CLIParser/CLIParser.h CLIParser/CLIParser.cpp -L/usr/lib -lX11 
