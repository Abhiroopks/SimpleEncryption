proj:	src/Encode.c src/Decode.c
		mkdir -p bin
		gcc -fopenmp src/Encode.c -o bin/Encode.o
		gcc -fopenmp src/Decode.c -o bin/Decode.o


clean:	bin/*.o
		rm bin/*.o
