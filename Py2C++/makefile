CFLAGS = -std=gnu++11 -O3 -g -Wall

P1.out : Project1.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ $(CFLAGS) -o P1.out Project1.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project1.o : Project1.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ $(CFLAGS) -c Project1.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ $(CFLAGS) -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ $(CFLAGS) -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ $(CFLAGS) -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P1.out *.gch

submit : Project1.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf johnsonP1
	mkdir johnsonP1
	cp Project1.cpp johnsonP1
	cp LexicalAnalyzer.h johnsonP1
	cp LexicalAnalyzer.cpp johnsonP1
	cp SyntacticalAnalyzer.h johnsonP1
	cp SyntacticalAnalyzer.cpp johnsonP1
	cp makefile johnsonP1
	cp README.txt johnsonP1
	tar cfvz johnsonP1.tgz johnsonP1
	cp johnsonP1.tgz ~tiawatts/cs460drop
