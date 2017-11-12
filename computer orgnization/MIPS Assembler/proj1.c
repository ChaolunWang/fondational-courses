//------------------------------proj1.c--------------------------------
/*In this project, an assembler was build which can be used to translate the mips assembly language to binary machine code
	programmed by: Chaolun Wang
		   at: 01/31/2017
*/
#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 100


//the structure to store the information of instruction data format: address  lable   form(r i j d x) name  rd  rs  rt  immed/shamt 
struct line{
	int address;
	char lable[20];
	char form;
	char name[5];
	char rd[5];
	char rs[5];
	char rt[5];	
	char immed[20];
};

//the structure to store output of the assembler
struct data{
	int address;
	int operation;
};


int extractLable(char buffer[], char lable[]);                              //extract the lable from buffer
int process(int address, struct line list[], char buffer[], int * n);       //processing the instructions, store the results in a table
//void print(struct line list[], int n);                                    //test purpose only
int findAddress(struct line list[], char lable[], int n);		    //find the address corresponding to the lable
int translate(struct line list[], struct data result[], int n);             //translate the data to binary code
int id(char lable[]);                                                       //find the number corresponding to opcode/registers
void printRes(struct data result[], int i);                                 //print result
int toint( char data[]);                                                    //change char array to int

int main (int argc, char *argv[])
{
	char buffer[BUFFERSIZE];
	int i=0, n=0;

	struct line list[100];
	struct data result[100];

	while(fgets(buffer, BUFFERSIZE , stdin)!=NULL)
	{
		i+=process(i, list, buffer, &n);
	}
	i=translate(list, result, n);
	printRes(result, i);
	return 0;
}








int translate(struct line list[], struct data result[], int n)
{
	int i=0, k=0;
	while(list[k].form!='d')
	{
		struct data code;
		int temp=0;
		code.address=list[k].address;
		if (strcmp(list[k].name, "la")==0)
		{
			temp=id("lui")<<26;
			temp+=1<<16;
			temp+=findAddress(list, list[k].immed, n)>>16;
			code.operation=temp;
			result[i]=code;

			i++;
			code.address=list[k].address+4;
			temp=id("ori")<<26;
			temp+=1<<21;
			temp+=id(list[k].rt)<<16;
			temp+=findAddress(list, list[k].immed, n)&65535;
			code.operation=temp;
			result[i]=code;
		}
		else if (strcmp(list[k].name, "bne")==0)
		{
			temp=id("bne")<<26;
			temp+=id(list[k].rs)<<21;
			temp+=id(list[k].rt)<<16;
			temp+=((findAddress(list, list[k].immed, n)-list[k].address)/4)&65535;
			code.operation=temp;
			result[i]=code;
		}
		else if (strcmp(list[k].name, "j")==0)
		{
			temp=id("j")<<26;
			temp+=(findAddress(list, list[k].immed, n)>>2);
			code.operation=temp;
			result[i]=code;
		}
		else if (strcmp(list[k].name, "sll")==0)
		{
			temp=id(list[k].rs)<<21;
			temp+=id(list[k].rt)<<16;
			temp+=id(list[k].rd)<<11;
			temp+=id(list[k].name);
			temp+=(toint(list[k].immed)&31)<<6;
			code.operation=temp;
			result[i]=code;
		}
		else if (list[k].form=='r')
		{
			temp=id(list[k].rs)<<21;
			temp+=id(list[k].rt)<<16;
			temp+=id(list[k].rd)<<11;
			temp+=id(list[k].name);
			code.operation=temp;
			result[i]=code;
		}
		else if (list[k].form=='i')
		{
			temp=id(list[k].name)<<26;
			temp+=id(list[k].rs)<<21;
			temp+=id(list[k].rt)<<16;
			temp+=(toint(list[k].immed)&65535);
			code.operation=temp;
			result[i]=code;
		}		
		i++;
		k++;
		
	}
	return i;
}


int extractLable(char buffer[], char lable[])
{
	int i=0;
	while(buffer[i]!=9 && buffer[i]!=32)
	{
		lable[i]=buffer[i];
		buffer[i]=' ';
		i++;
	}
	lable[i-1]='\0';
	return i;
}

int findAddress(struct line list[], char lable[], int n)
{
	int i=0;
	for(i=0; i<n; ++i)
	{
		if(strcmp(list[i].lable, lable)==0)
			return list[i].address;
	}
	return -1;
}

int process(int address, struct line list[], char buffer[], int * n)
{
	struct line code;
	code.lable[0]='\0';
	code.name[0]='\0';
	code.rd[0]='\0';
	code.rs[0]='\0';
	code.rt[0]='\0';
	code.immed[0]='\0';
	char tempname[5];
	int result=4;

	code.address=address;

	if(buffer[0]!=9)
		extractLable(buffer, code.lable);

	sscanf (buffer,"%s", tempname);
 	if (strcmp(tempname, "add")==0)
	{
		sscanf (buffer,"%s %[^,], %[^,],%s",code.name, code.rd, code.rs, code.rt);
		code.form='r';
		list[*n]=code;
	}
	else if(strcmp(tempname, "addi")==0)
	{
		sscanf (buffer,"%s %[^,], %[^,],%s",code.name, code.rt, code.rs, code.immed);
		code.form='i';
		list[*n]=code;
	}
	else if(strcmp(tempname, "nor")==0)
	{
		sscanf (buffer,"%s %[^,], %[^,],%s",code.name, code.rd, code.rs, code.rt);
		code.form='r';
		list[*n]=code;
	}
	else if(strcmp(tempname, "ori")==0)
	{
		sscanf (buffer,"%s %[^,], %[^,],%s",code.name, code.rt, code.rs, code.immed);
		code.form='i';
		list[*n]=code;
	}
	else if(strcmp(tempname, "sll")==0)
	{
		sscanf (buffer,"%s %[^,], %[^,],%s",code.name, code.rd, code.rt, code.immed);
		code.form='r';
		list[*n]=code;
	}
	else if(strcmp(tempname, "lui")==0)
	{
		sscanf (buffer,"%s %[^,], %s",code.name, code.rt, code.immed);
		code.form='i';
		list[*n]=code;
	}	
	else if(strcmp(tempname, "sw")==0)
	{
		sscanf (buffer,"%s %[^,], %[^'(']( %[^')'])",code.name, code.rt, code.immed, code.rs);
		code.form='i';
		list[*n]=code;
	}
	else if(strcmp(tempname, "lw")==0)
	{
		sscanf (buffer,"%s %[^,], %[^'(']( %[^')'])",code.name, code.rt, code.immed, code.rs);
		code.form='i';
		list[*n]=code;
	}
	else if(strcmp(tempname, "bne")==0)
	{
		sscanf (buffer,"%s %[^,], %[^,],%s",code.name, code.rs, code.rt, code.immed);
		code.form='i';
		list[*n]=code;
	}
	else if(strcmp(tempname, "j")==0)
	{
		sscanf (buffer,"%s %s",code.name, code.immed);
		code.form='j';
		list[*n]=code;
	}
	else if(strcmp(tempname, "la")==0)
	{
		sscanf (buffer,"%s %[^,], %s",code.name, code.rt, code.immed); 
		code.form='x';
		list[*n]=code;
		result=8;
	}
	else if(strcmp(tempname, ".space")==0)
	{
		sscanf (buffer,"%s %d",code.name, &result);
		code.form='d';
		list[*n]=code;
	}
 	else if(strcmp(tempname, ".word")==0)
	{
		sscanf (buffer,"%s",code.name);
		code.form='d';
		list[*n]=code;
	}
	else
	{
		result=0;
		*n=*n-1;
 	}
	*n=*n+1;
	return result;
}


/*void print(struct line list[], int n)
{
	int i;
	for(i=0; i<n; i++)
		printf("\naddress: %d lable: %s form: %c name: %s rd: %s rs: %s rt: %s immed: %s \n", list[i].address, list[i].lable, list[i].form, list[i].name, list[i].rd,list[i].rs,list[i].rt,list[i].immed);
}*/ //test purpose only

int id(char lable[])
{
	if(strcmp(lable, "add")==0)
		return 32;
	if(strcmp(lable, "addi")==0)
		return 8;
	if(strcmp(lable, "nor")==0)
		return 39;
	if(strcmp(lable, "ori")==0)
		return 13;
	if(strcmp(lable, "sll")==0)
		return 0;
	if(strcmp(lable, "lui")==0)
		return 15;
	if(strcmp(lable, "sw")==0)
		return 43;
	if(strcmp(lable, "lw")==0)
		return 35;
	if(strcmp(lable, "bne")==0)
		return 5;
	if(strcmp(lable, "j")==0)
		return 2;
	if(strcmp(lable, "$t0")==0)
		return 8;
	if(strcmp(lable, "$t1")==0)
		return 9;
	if(strcmp(lable, "$t2")==0)
		return 10;
	if(strcmp(lable, "$t3")==0)
		return 11;
	if(strcmp(lable, "$t4")==0)
		return 12;
	if(strcmp(lable, "$t5")==0)
		return 13;
	if(strcmp(lable, "$t6")==0)
		return 14;
	if(strcmp(lable, "$t7")==0)
		return 15;
	if(strcmp(lable, "$s0")==0)
		return 16;
	if(strcmp(lable, "$s1")==0)
		return 17;
	if(strcmp(lable, "$s2")==0)
		return 18;
	if(strcmp(lable, "$s3")==0)
		return 19;
	if(strcmp(lable, "$s4")==0)
		return 20;
	if(strcmp(lable, "$s5")==0)
		return 21;
	if(strcmp(lable, "$s6")==0)
		return 22;
	if(strcmp(lable, "$s7")==0)
		return 23;
	if(strcmp(lable, "$0")==0)
		return 0;
	return 0;
}


void printRes(struct data result[], int i)
{
	int k=0;
	for(k=0; k<i; ++k)
	{
		printf("0x%08X: 0x%08X\n", result[k].address, result[k].operation);
	}
}

int toint( char data[])
{
	int result;
	sscanf (data,"%d",&result);
	return result;
}





