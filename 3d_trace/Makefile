default:	trace.cpp
		g++ -o trace  trace.cpp  -lGL  -lGLU  /usr/lib64/libglut.so  -lm -I./include -Wno-write-strings
test:
		g++ -o trace_test1  trace_test2.cpp  -lGL  -lGLU  /usr/lib64/libglut.so  -lm -I./include -Wno-write-strings
		./trace_test1
clean:
		rm ./trace
