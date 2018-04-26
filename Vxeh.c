//VXEH 1.0 by GCALDL
//Views file in Hex and ASCII

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>

//variables:
FILE *f; 			//file
unsigned char *fMem; //file in memory
long fLen;			//file length in bytes

//argument stuff
char *ptr; 	//end pointer
long lLen = 16;	//line length

//colors
int cHAd = 46;	//Horizontal offset Pane
int cVAd = 155;	//Vertical Address Pane
int cAI = 78;	//Hex Pane
int cHX = 15;	//ASCII Pane

HANDLE hCmd;						//console handle
CONSOLE_SCREEN_BUFFER_INFO CmdInfo;	//console information
WORD cmdDefult;						//default console settings

//function declaration
int saveDefults();
int setColor();
int loadFile();
int draw();

int main(int argc, char* argv[]){
	//check arguments
	if(!fopen(argv[1], "rb")){
		printf("Error: Please specify valid file path.");
		return 0;
	}
	if(argv[2]){
		lLen = strtol(argv[2], &ptr, 10);
	}
	//main protocol
	saveDefults();
	loadFile(argv[1]);
	draw();
	return 0;
}

int draw(){
	//top row 'offset:' & horizontal offset pane
	setColor(cVAd);
	printf("offset(h) ");
	
	setColor(cHAd);
	for (int i = 0; i < lLen; i++){
		printf("%02X ", i);
	}
	
	setColor(0);
	printf(" \n");
	
	//draw main panes
	for(int y = 0; y <= fLen/lLen; y++){
	setColor(cVAd);
	printf("%08X  ", y);
		//drawHex
		for(int x = lLen*y; x < lLen*(y+1); x++){
			setColor(cHX);
			if(x<fLen){
				printf("%02X ", fMem[x]);
			}else{
				setColor(0);
				printf("   ");
			}
		}
		printf(": ");//draw spacer
		//draw ASCII
		for(int x = lLen*y; x < lLen*(y+1); x++){
			setColor(cAI);
			if(x==fLen){break;}
			if ((fMem[x] == 10) || (fMem[x]==13) || (fMem[x]==32) || (fMem[x]==0) || (fMem[x]==9)){
					printf(".");
			}else{
					printf("%01c", fMem[x]);
			}
		}
		setColor(0);
		printf(" \n");
	}
	setColor(cmdDefult);//finish drawing reset colors
	return 0;
}

int loadFile(char* path){	
	f = fopen(path, "rb");	//open file
	fseek(f, 0, SEEK_END);		//jump to end of file
	fLen = ftell(f); 			//get file size(bytes)
	rewind(f);					//jump to start of file
	
	fMem = (char *)malloc((fLen+1)*sizeof(char));//allocate memory for file
	fread(fMem, fLen, 1, f); 					//Read in the entire file
	fclose(f);									//close file
	return 0;
}

int saveDefults(){
	hCmd = GetStdHandle(STD_OUTPUT_HANDLE);		//assign handle
	GetConsoleScreenBufferInfo(hCmd, &CmdInfo); //get console Info
    cmdDefult = CmdInfo.wAttributes; 			//save console default attributes
	return 0;
}
int setColor(int c){
	SetConsoleTextAttribute(hCmd, c); //set color
	return 0;
}