//-----------------------------------proj3.c--------------------------------------
/*
    Name: Chaolun Wang
    FSUID: cw13f
    GRADUATE VERSION
    This is the simulation of catch with different archetecture
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lg(int input); //calculate log 2 
void updateuwt(int arr[], int m, int n, char access, int address, char id, int *ref, int *hit, int *miss, int *mem); //update write through
void updateuwb(int arr[], int m, int n, char access, int address, char id, int *ref, int *hit, int *miss, int *mem); //update write back

int main(int argc, char *argv[])
{
	int blockSize=0, setNum=0, associate=0, offset, indexNum, tag, setNumS, indexNumS, tagS, i, address;
	char input[100], access, id;

	//read flags value
	for(i=1; i<7; i+=2)
	{
		if(argv[i][1]=='b')
			blockSize=atoi(argv[i+1]);
		else if(argv[i][1]=='s')
			setNum=atoi(argv[i+1]);
		else if(argv[i][1]=='n')
			associate=atoi(argv[i+1]);
		else
		{
			printf("Error: Improper command line arguments\n");
			exit(0);
		}
	}
	//calculate offset and tag
	offset=lg(blockSize);
	indexNum=lg(setNum);
	tag=32-offset-indexNum;
	setNumS=setNum>>1;
	indexNumS=lg(setNumS);
	tagS=32-offset-indexNumS;
	//initialize statistical counters
	int n=associate*2+1, ref=0, hit=0, miss=0, mem=0;
	int n2=associate*3+1, ref2=0, hit2=0, miss2=0, mem2=0;
	int ref3=0, hit3=0, miss3=0, mem3=0;
	int ref4=0, hit4=0, miss4=0, mem4=0;
	int ref5=0, hit5=0, miss5=0, mem5=0;

	//initialize caches
	int *uwt= (int *)calloc(setNum * n,  sizeof(int));
	int *uwb= (int *)calloc(setNum * n2,  sizeof(int));	
	int *uwt2= (int *)calloc(setNumS * n,  sizeof(int));
	int *uwt3= (int *)calloc(setNumS * n,  sizeof(int));
	int *uwb2= (int *)calloc(setNumS * n2,  sizeof(int));

	//loop when sampeling
	while(fgets(input, 100, stdin) != 0 ){
		sscanf(input, " %c %d %c", &access, &address, &id);
		updateuwt(uwt, setNum, n, access, address>>offset, id, &ref, &hit, &miss, &mem);
		updateuwb(uwb, setNum, n2, access, address>>offset, id, &ref2, &hit2, &miss2, &mem2);
		if(id=='I')
			updateuwt(uwt2, setNumS, n, access, address>>offset, id, &ref3, &hit3, &miss3, &mem3);
		else
		{
			updateuwt(uwt3, setNumS, n, access, address>>offset, id, &ref4, &hit4, &miss4, &mem4);
			updateuwb(uwb2, setNumS, n2, access, address>>offset, id, &ref5, &hit5, &miss5, &mem5);			
		}
	}

	//free pointers
	free(uwt);
	free(uwt2);
	free(uwt3);
	free(uwb);
	free(uwb2);

	//print statistics
	printf("================ Unified ================\n");
	printf("Block size: %d\n", blockSize);
	printf("Number of sets: %d\n", setNum);
	printf("Associativity: %d\n", associate);
	printf("Number of offset bits: %d\n", offset);
	printf("Number of index bits: %d\n", indexNum);
	printf("Number of tag bits: %d\n", tag);

	printf("\n");
	printf("****************************************\n");
	printf("Write-through with No Write Allocate\n");
	printf("****************************************\n");
	printf("Total number of references: %d\n", ref);
	printf("Hits: %d\n", hit);
	printf("Misses: %d\n", miss);
	printf("Memory References: %d\n", mem);

	printf("\n");
	printf("****************************************\n");
	printf("Write-back with Write Allocate\n");
	printf("****************************************\n");
	printf("Total number of references: %d\n", ref2);
	printf("Hits: %d\n", hit2);
	printf("Misses: %d\n", miss2);
	printf("Memory References: %d\n", mem2);

	printf("\n============= Split I vs. D =============\n");
	printf("Block size: %d\n", blockSize);
	printf("Number of sets: %d\n", setNumS);
	printf("Associativity: %d\n", associate);
	printf("Number of offset bits: %d\n", offset);
	printf("Number of index bits: %d\n", indexNumS);
	printf("Number of tag bits: %d\n", tagS);

	printf("\n");
	printf("****************************************\n");
	printf("Instructions: \n");
	printf("****************************************\n");
	printf("Total number of references: %d\n", ref3);
	printf("Hits: %d\n", hit3);
	printf("Misses: %d\n", miss3);
	printf("Memory References: %d\n", mem3);

	printf("\n");
	printf("****************************************\n");
	printf("Data: Write-through with No Write Allocate\n");
	printf("****************************************\n");
	printf("Total number of references: %d\n", ref4);
	printf("Hits: %d\n", hit4);
	printf("Misses: %d\n", miss4);
	printf("Memory References: %d\n", mem4);

	printf("\n");
	printf("****************************************\n");
	printf("Data: Write-back with Write Allocate\n");
	printf("****************************************\n");
	printf("Total number of references: %d\n", ref5);
	printf("Hits: %d\n", hit5);
	printf("Misses: %d\n", miss5);
	printf("Memory References: %d\n", mem5);
}

int lg(int input)
{
	int result=0;
	while(input>1)
	{
		input=input>>1;
		result++;
	}
	return result;
}

void updateuwt(int arr[], int m, int n, char access, int address, char id, int *ref, int *hit, int *miss, int *mem)
{
	++(*ref);
	int ifhit=0, k=address%m, i;
	int num=arr[k*n], max=(n-1)/2;
	for(i=0; i<num; ++i)
	{
		if(arr[k*n+i*2+1]==address)
		{
			ifhit=1;
			break;
		}
	}

	if(ifhit) //if hit
	{
		(*hit)++;
		arr[k*n+i*2+2]=(*ref);
		if(access=='W')
			(*mem)++;
	}
	else
	{
		(*miss)++;
		(*mem)++;
		if(access=='R')
		{
			if(num<max)
			{
				arr[k*n+num*2+1]=address;
				arr[k*n+num*2+2]=(*ref);
				arr[k*n]++;
			}
			else
			{
				int min=1000, index=0;
				for(i=0; i<max; ++i)
				{
					if(min>arr[k*n+i*2+2])
					{
						min=arr[k*n+i*2+2];
						index=i;
					}
				}
				arr[k*n+index*2+1]=address;
				arr[k*n+index*2+2]=(*ref);	
			}
		}			
	}
	
}

void updateuwb(int arr[], int m, int n, char access, int address, char id, int *ref, int *hit, int *miss, int *mem)
{
	++(*ref);
	int ifhit=0, k=address%m, i;
	int num=arr[k*n], max=(n-1)/3;
	for(i=0; i<num; ++i)
	{
		if(arr[k*n+i*3+1]==address)
		{
			ifhit=1;
			break;
		}
	}

	if(ifhit) //if hit
	{
		(*hit)++;
		arr[k*n+i*3+2]=(*ref);
		if(access=='W')
			arr[k*n+i*3+3]=1;
	}
	else
	{
		(*miss)++;
		(*mem)++;
		if(num<max)
		{
			arr[k*n+num*3+1]=address;
			arr[k*n+num*3+2]=(*ref);
			arr[k*n]++;
			if(access=='W')
				arr[k*n+num*3+3]=1;
		}
		else
		{
			int min=1000, index=0;
			for(i=0; i<max; ++i)
			{
				if(min>arr[k*n+i*3+2])
				{
					min=arr[k*n+i*3+2];
					index=i;
				}
			}
			if(arr[k*n+index*3+3]==1)
				(*mem)++;
			arr[k*n+index*3+1]=address;
			arr[k*n+index*3+2]=(*ref);
			if(access=='R')
				arr[k*n+index*3+3]=0;
			else
				arr[k*n+index*3+3]=1;	
		}
					
	}
	
}

