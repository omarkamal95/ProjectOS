void printString(char*);
char* readString(char*);
char* readSector(char* , int);
int div(int, int);
int mod (int, int);
void readFile (char*,char*);
void handleInterrupt21 (int , int , int, int);

main(){
	
char buffer[13312]; /*this is the maximum size of a file*/
makeInterrupt21();
interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
/*printString("hello \0");
*/while(true){
}/*hang up*/


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
	while (c != 0xd )
	{
		c= interrupt(0x16,0x0*256 ,0,0,0);

		if (c!= 0xd ) {
			if (c == 0x8) {
				if(i>0)
					{
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
/*	int sectors [26];*/

int j=0;
while (fname[j]!= 0){ 
j = j+1; 
}

readSector(array , 2);

int i = 0;
int count=0;
int flag = 0;

while(i< 512){ 
 if (array[i] == fname[count]) {
    flag = 1; 
   if (count == j-1) {
    count = 6;
    int k= 0;
    while (array[count] != 0x00 && count < 32){ 
     sectors [k] = array[count];
     k = k+1;
     count = count +1;
    }
    break;
   } else {
   i = i+1;
   count = count +1;
}
 } else { 
    i = i+31;
    count = 0;
 }
}

if (flag == 0){ 
return; }


int r = 0;
int buffCount =0;
while (r <26 && sectors[r] != 0x00){ 
char temp[512];

readSector(temp, sectors[r]);
buffer[buffCount] = 512;
buffCount = buffCount+1;

int l = 0;
while (l<512){ 
buffer[buffCount] = temp [l];
l = l+1;
buffCount = buffCount +1;
}



}





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
	else
	{
		printString("ERROR \0");
	}
}
