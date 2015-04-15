void printString(char*);
char* readString(char*);
char* readSector(char* , int);
int div(int, int);
int mod (int, int);
void handleInterrupt21 (int , int , int, int);
void readFile (char*,char*);
void executeProgram(char*, int);
void terminate();

main(){

printString("hellhhfo \0");
makeInterrupt21();
interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
printString("HANG UP YA 7ayawan \0");
while(1);

}

void printString(char* chars) {	
	int x = 0;	
	while(chars[x] != 0) {
		interrupt(0x10, 0xE*256+ chars[x], 0, 0, 0);
		x = x+1;
	}
}

char* readString(char* chars) {
	int i = 0;
	int count=0;
	char c = 0x1;
	while (c != 0xd ) {
		c= interrupt(0x16,0x0*256 ,0,0,0);
		if (c!= 0xd ) {
			if (c == 0x8) {
				if(i>0) {
						interrupt(0x10, 0xE*256+ c, 0, 0, 0);
						i--;
						chars[i] = c;
						i++;
						interrupt(0x10, 0xE*256, 0, 0, 0);
						i--;
						interrupt(0x10, 0xE*256+ c, 0, 0, 0);
				}
			}
			else {
				interrupt(0x10, 0xE*256+ c, 0, 0, 0);
				chars [i] = c;
				i = i+1;
				count = count+1;
			}

		}
	}

	chars [i+1] = 0xa;
	chars [i+2] = 0x0;
	return chars;
}
char* readSector(char* buffer, int sector) {
	int relative = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);
	int track = div(sector, 36);
	interrupt(0x13, 2*256 + 1, buffer, track*256 + relative, head*256 +0);
	return buffer;
}

int div (int x, int y) {
	int i =0;
	while (x >= y) {
		x = x-y;
		i= i+1;
	}
	return i;
}

int mod (int x, int y) {
	while (x >= y) {
		x = x-y;
	}
	return x;
}

void readFile(char* fname, char* buffer){ 
	
	char array [512];
	int sectors [26];
	int r = 0;
	int buffCount =0;
	int l = 0;
	char temp[512];
	int k= 0;
    int count=0;
    int smalli = 0;
    char secnum;
    int flag = 0;
    int i =0;

	readSector(array , 2);

	while(i < 512){ 
		if (array[smalli] == fname[count]) {
	   		if (fname[count+1] == 0) {
			   	flag = 1; 
			    smalli = 6 + i;
			   	k = 0;
			    while (smalli < i + 32){ 
				    sectors [k] = array[smalli];
				    k = k+1;
				    smalli = smalli +1;
			    }
			    break;
	   		} else {
			   smalli = smalli+1;
			   count = count +1;
			}
	 	} else { 
	    i = i + 32;
	    count = 0;
	    smalli = i;
	 	}
	}

	if (flag == 0){ 
		return; 
	}

 	r = 0;
	buffCount =0;

	while (r < 26){ 
		secnum = sectors[r];
		readSector(buffer + buffCount,secnum);
		buffCount += 512;
		r = r+ 1;
	

	}

}

void executeProgram(char* name, int segment) {
	char buffer [13312]; 
	int bufferCount = 0;
	int addressCount = 0; 
	
	readFile(name, buffer);
	while (bufferCount < 13312) {
		putInMemory (segment, addressCount, buffer[bufferCount]);
		bufferCount = bufferCount+1;
		addressCount = addressCount +1;
	}
	
	launchProgram(segment);


}


void terminate(){ 
 while(1);

}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {
	if (AX == 0)
	{
		printString(BX);
	}
	else if (AX == 1)
	{
		readString(BX);
	}
	else if (AX == 2)
	{
		readSector(BX, CX);
	}
	else if (AX == 3)
	{
		readFile(BX, CX);
	}
	else if (AX == 4)
	{
		executeProgram(BX, CX);
	}
	else if (AX == 5)
	{
		terminate();
	}
	else
	{
		printString("ERROR \0");
	}
}
