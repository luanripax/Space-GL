all:
	g++ -c *.cpp
	g++ *.o -o trabalhocg -lGL -lGLU -lglut

clean:
	rm -rf *.o