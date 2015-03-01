void printString(char*);
char* readString(char*);
char* readSector(char* , int);
int div(int, int);
int mod (int, int);
void handleInterrupt21 (int , int , int, int);
main(){
	
	char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	while(0x1 == 0x1){
	}
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
	char c = 0x1 ;
	while (c != 0x20 || count < 80)
	{
		c= interrupt(0x16,0x0*256 ,0,0,0);
		interrupt(0x10, 0xE*256+ c, 0, 0, 0);
		
		if (c == 0x8)
		{
			i--;
		}
		else
		{
		chars [i] = c;
		i = i+1;
		count = count+1;
		}
	}
	chars [i] = 0xa;
	chars [i+1] = 0x0;
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
	else
	{
		printString("ERROR \0");
	}
}
