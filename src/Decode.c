#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void swapSegs(char* arr,int first,int second, int size,int inlen){
	//first and second are start indices of segments to swap
	//size is the # of bytes of each segment
	//store copy of second segment
	char temp[size];
	#pragma omp parallel for
			for(int i = 0; i < size; i++){
				temp[i] = arr[(second+i)%inlen];
			}	
	//swap bytes for each segment
	#pragma omp parallel for
			for(int i = 0; i < size; i++){
				arr[(second+i)%inlen] = arr[(first+i)%inlen];
				arr[(first+i)%inlen] = temp[i];
			}
}


int main (int argc, char *argv[]) {
	//ensure all args are passed
	if(argc < 4){
		printf("ERROR: input,output,and key files must be passed as args\n");
		return 1;
	}

	//open files for reading/writing
	FILE* infile = fopen(argv[1],"rb");
	if(infile == NULL){printf("ERROR: input file not found: %s\n",argv[1]);}
	FILE* outfile = fopen(argv[2],"wb");
	FILE* keyfile = fopen(argv[3],"rb");
	if(keyfile == NULL){printf("ERROR: key file not found: %s\n",argv[3]);}

	//read the input
	int inlen;
	fseek(infile, 0, SEEK_END);          // Jump to the end of the file
	inlen = ftell(infile);             // Get the current byte offset in the file
	rewind(infile);                      // Jump back to the beginning of the file
	char* inbuff = (char*)malloc(inlen * sizeof(char)); // Enough memory for the file
	fread(inbuff, 1, inlen, infile); // Read in the entire file

	//read the key
	int keylen;
	fseek(keyfile, 0, SEEK_END);          // Jump to the end of the file
	keylen = ftell(keyfile);             // Get the current byte offset in the file
	rewind(keyfile);                      // Jump back to the beginning of the file
	char* keybuff = (char*)malloc(keylen * sizeof(char)); // Enough memory for the file
	fread(keybuff, 1, keylen, keyfile); // Read in the entire file


	//undo segment shuffling
	int numSegments = (1 << 8);
	int segmentSize = (inlen + numSegments - 1) / numSegments;
	uint8_t swapSegInd; //for swapping data segments inside loop

	//loop over each segment
	for(int i = 0; i < numSegments; i++){
		swapSegInd = (uint8_t)keybuff[keylen-1-(i % keylen)];
		swapSegs(inbuff,((numSegments-1-i)*segmentSize) % inlen, swapSegInd % inlen,segmentSize,inlen);
	}
	
	//iterate over each byte of input - XOR in place with keyfile
	for(int i = 0; i < inlen; i++){
		inbuff[i] ^= keybuff[i % keylen];
	}

	//write inbuff to outfile (it is now encoded)
	fwrite(inbuff, inlen, 1, outfile);	
	
	//close all files and free data
	free(inbuff);
	free(keybuff);
	fclose(infile);
	fclose(outfile);
	fclose(keyfile);
	return 0;
}	
