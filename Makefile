all : program1 program2 program3 program4

program1 : main.cpp
	g++ main.cpp -o program1

program2 : main2.cpp
	g++ main2.cpp -o program2

program3 : main3.cpp
	g++ main3.cpp -o program3 -lpthread

program4 : main4.cpp
	g++ main4.cpp -o program4

clean:
	rm -rf program*
