default:
	gcc ../data_generator/datagen.c -std=c99 -lm -o ./gen -I../conversion
	gcc ../logging/logger.c -std=c99 -lm -o ../logging/logger -I../conversion -Wno-format-security
	g++ datahandle.cpp -std=c++11 -lm -o datahandle -I../conversion -lpthread
