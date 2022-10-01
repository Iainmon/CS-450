
all:
	clang++ -std=c++17 -o main main.cc -L/System/Library/Frameworks -framework GLUT -framework OpenGL  && ./main

