default:
<<<<<<< HEAD
	gcc ./generate/datagen.c -std=c99 -lm -o ./gen
=======
	gcc ../data_generator/datagen.c -std=c99 -lm -o ./gen
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
	gcc logger.c  -std=c99 -lm -o logger -Wno-format-security
wgcc:
	gcc logger.c -std=c99 -lm -o logger
wclang:
	clang logger.c -std=c99 -lm -o logger
test:
<<<<<<< HEAD
	gcc ./generate/datagen.c -std=c99 -lm -o ./gen
	gcc logger.c  -std=c99 -lm -o logger -Wno-format-security
	gcc logtest1.c -std=c99 -lm -o test1
	./gen -to > testlog.txt
=======
	gcc ../data_generator/datagen.c -std=c99 -lm -o ./gen
	gcc logger.c  -std=c99 -lm -o logger -Wno-format-security
	gcc logtest1.c -std=c99 -lm -o test1
	./gen -test -to testlog.txt 
	./gen -test -pipe ./logger
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
	./test1
clean: 
	rm ./logger ./test1 ./log.txt ./testlog.txt ./gen
