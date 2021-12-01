#include <iostream>
#include <string>

const int BUFFER_SIZE = 16;
void binToASCII(unsigned char buffer[BUFFER_SIZE], int lastReadSize);
void hexdump(FILE *f);
std::string help();

int main(int argc, char *argv[]){
	FILE *f;

	if(argc==2){
		f = fopen(argv[1],"rb");
		hexdump(f);
		fclose(f);
	}

	else{
		std::cout <<  help();
	}
 
	return 0;
}

void hexdump(FILE *f){
	unsigned char prevBuffer[BUFFER_SIZE];
	unsigned char buffer[BUFFER_SIZE];
	int memCount = 0;
	int lastReadSize = 0;
	bool prevPrint = true;
	bool eof = false;

	while(!eof){
		lastReadSize = fread(buffer,sizeof(char),BUFFER_SIZE,f);
		if(lastReadSize < BUFFER_SIZE){
			eof=true;
			if(lastReadSize==0){
				break;
			}
		}

		if(std::equal(std::begin(buffer),std::end(buffer),std::begin(prevBuffer)) && !eof){
			if(prevPrint){
				printf("*\n");
			}
			prevPrint=false;
		}
		else{
			prevPrint=true;
		}

		if(prevPrint){
			printf("%08x  ", memCount);
      
			for(int i=0; i < BUFFER_SIZE/2; i++){
				if(i<lastReadSize){
					printf("%02x ", buffer[i]);
				}
				else{
					printf("   ");
				}
			}
      
			printf(" ");
      
			for(int i=BUFFER_SIZE/2; i < BUFFER_SIZE; i++){
				if(i<lastReadSize){
					printf("%02x ", buffer[i]);
				}
				else{
					printf("   ");
				}
			}
      
			printf(" |");
			binToASCII(buffer,lastReadSize);
			printf("|\n");
		}

		memCount += lastReadSize;
		for(int i=0; i < BUFFER_SIZE; i++){
			prevBuffer[i] = buffer[i];
		}
	}

	printf("%08x\n", memCount);

	return;

}

void binToASCII(unsigned char buffer[BUFFER_SIZE], int lastReadSize){
	
  for(int i = 0; i < BUFFER_SIZE && i<lastReadSize; i++){
		if(std::isprint(buffer[i])){
			printf("%c", buffer[i]);
		}
		else{
			printf(".");
		}
	}
	
  return;
}

std::string help(){
	std::string helpmsg;
	helpmsg = "Usage: hexcrap <file>\n";
	
  return helpmsg;
}
