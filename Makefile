default:
	gcc logging/generate/datagen.c -std=c99 -lm -o logging/gen
	gcc logging/logger.c  -std=c99 -lm -o logging/logger -Wno-format-security
wgcc:
	gcc logging/logger.c -std=c99 -lm -o logging/logger
wclang:
	clang logging/logger.c -std=c99 -lm -o logging/logger
test:
	gcc ./logging//generate/datagen.c -std=c99 -lm -o ./gen
	gcc ./logging/logger.c  -std=c99 -lm -o ./logger -Wno-format-security
	gcc ./logging/logtest1.c -std=c99 -lm -o ./test1
	rm -f ./testlog.txt
	./gen -to > testlog.txt
	./test1
clean: 
	rm ./logger ./test1 ./log.txt ./testlog.txt ./gen
