void printString(char*);
main(){
	printString("000000000\0");
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
