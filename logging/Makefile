default:
	gcc ./generate/datagen.c -std=c99 -lm -o ./gen
	gcc logger.c  -std=c99 -lm -o logger -Wno-format-security
wgcc:
	gcc logger.c -std=c99 -lm -o logger
wclang:
	clang logger.c -std=c99 -lm -o logger
test:
	gcc ./generate/datagen.c -std=c99 -lm -o ./gen
	gcc logger.c  -std=c99 -lm -o logger -Wno-format-security
	gcc logtest1.c -std=c99 -lm -o test1
	./gen -to > testlog.txt
	./test1
clean: 
	rm ./logger ./test1 ./log.txt ./testlog.txt ./gen
