void checkCommand (char*); 
main() { 
		char command[80];
		
	while(1){ 
		
		interrupt (0x21, 0, "SHELL> \0", 0, 0);
		interrupt(0x21,1,command,0,0);
		checkCommand(command);
	}
}

void checkCommand (char* command) { 
	 int i = 0;
		int viewFlag = 0;
		int executeFlag = 0;
		char exec [7];
		char view [4];

		view [0] = 'v';
		view [1] = 'i';
		view [2] = 'e';
		view [3] = 'w';

		exec [0] = 'e';
		exec [1] = 'x';
		exec [2] = 'e';
		exec [3] = 'c';
		exec [4] = 'u';
		exec [5] = 't';
		exec [6] = 'e';
		
		while (i < 7){

    	    if (command [i] == view [i])
    	    { 
   				if(i == 3) { 
   						if(command[i+1] == 0x20) { 
   							viewFlag = viewFlag +2;
   							break;
   						}
   				}
   				
   				viewFlag = viewFlag +1;

            } 
             if (command [i] == exec[i]) 
            { 
            	if(i == 6) { 
   						if(command[i+1] == 0x20) { 
   							executeFlag = executeFlag +2;
   							break;
   						}
   				}
   				
   				executeFlag = executeFlag +1;

            }

            else { 

              break;
            }

            i = i+1;
		}
 

     if (viewFlag == 5) { 
        	
    char buffer[13312]; /*this is the maximum size of a file*/
	interrupt(0x21, 3, command + 5, buffer, 0); /*read the file into buffer*/
	interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
	

     } else if (executeFlag == 8) { 
 
	interrupt(0x21, 4, command + 8, 0x2000, 0); 
     } 


     else { 
      
      interrupt (0x21, 0, "Bad Command \0", 0, 0);
		interrupt(0x10, 0xE*256+ 0xa, 0, 0, 0);
		interrupt(0x10, 0xE*256+ 0xd, 0, 0, 0);

     }

		
	
}