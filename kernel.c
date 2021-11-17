void readSector(char* buffer, int sector);
void printChar(char c);
void printString(char* s);
void readString(char* s);
void handleInterrupt21(int ax, int bx, int cx, int dx);

void main(){
/*
	//test printString
	printString("Hello World");
*/

/*
	//test readString
	char line[80];
	printString("Enter a line: ");
	readString(line);
	printString(line);
*/


/*	//testing read sector
	char buffer[512];
	readSector(buffer,30);
	printString(buffer);
	printString("HI");
*/

/*	//testing interrupt
	char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
*/

	while(1);


}

void printString(char* s){
	int i;
	for(i=0; s[i]!='\0'; i++){ 
		printChar(s[i]);
	}

	//call princtchar over and over again until done
	
}

void printChar(char c){


	interrupt(0x10,0x0e*0x100+c,0,0,0);
	//interrupt to print char
	
}

void readString (char* s)
{

	int i = 0;
	char c = 0;
	while (1)
	{
		c = interrupt (0x16, 0, 0, 0, 0);
		//if input is enter key then stop input
		if (c == 0xd)
			break;
		//if input is backspace check if array length is bigger than 0
		//and if it is then interrupt that stroke and decrease array size
		if (c == 0x8){
		if (i > 0){
			i--;
			interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
			interrupt(0x10, 0xe * 256 + ' ', 0, 0, 0);
			interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
		}
		else
		continue;
		}
		//else if neither of those keys are inputs then inc through array
		//and keep reading through the array
		else
		{
		s[i++] = c;
		interrupt (0x10, 0x0e * 256 + c, 0, 0, 0);	
		}
	}
	//print on next line	
	interrupt(0x10, 0xe * 256 + '\n', 0, 0, 0);

	//makes sure the array size is 80 and then also sets the last two elements
	//of the array as the last two elements should be 0xa(line feed) and
	//0x0 (end of string)
	if (i > 80 - 2)
		i = 80 - 2;
	s[i++] = 0xa;
	s[i] = 0x0;
	return;
}

void readSector (char* buffer, int sector){

	//declaring values in readSector
	int ah = 2;
	int al = 1;
	int bx = buffer;
	int ch = 0;
	int cl = sector + 1;
	int dh = 0;
	int dl = 0x80;
	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx = dh*256+dl;

	interrupt (0x13, ax, bx, cx, dx);

	buffer[512 - 1] = '\0';
}


void handleInterrupt21(int ax, int bx, int cx, int dx){
	//if ax = 0 then run print string
	//if ax = 1 then read string
	//if ax = 2 then read sector
	//and then if ax = 3 print an error
	if (ax == 0)
		printString(bx);
	else if (ax == 1)
		readString(bx);
	else if (ax == 2)
		readSector(bx, cx);
	else
		printString("Error!");
}
