//This file test the model on pre-recorded input files.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>

long double test_input();			//functions declaration
void load_file(int,int);
void load_model(int);
void alpha_computation();

long double a[6][6];			//global variable declaration and initialization
long double b[6][33];
int o[150];
long double pie[6];
long double alpha[150][6];
int n=5,m=32,T;
long double prob;
char obfilename[50];
char modelfilename[50];



void test_model()			//This function is entry point of the testing_module.h file and it call other function of this file.
{
	int result =0,accuracy=0,avg=0;
	long double x=0;

	for(int k=0;k<=9;k++)
	{

		//load_model(k);
		accuracy=0;
		for(int i=21;i<=30;i++)
		{
			strcpy(obfilename,"dataset\\obs\\204101011_");
			itoa(k,obfilename+strlen(obfilename),10);
			strcat(obfilename,"_");
			itoa(i,obfilename+strlen(obfilename),10);
			strcat(obfilename,".txt");


			load_file(k,i);
			prob=0;
			x=0;
			
			for(int j=0;j<=9;j++)
			{
				load_model(j);
			//	printf("\nTesting....\n");
				x=test_input();
				if(x>prob)
				{
					prob=x;
					result=j;
				}

			}
			if(result==k)
			{	accuracy++;
				avg++;
			}

			printf("digit %d  is recognized as : %d \n",k,result);
		}
		printf("-------------------------------\n");
		printf("        Accuracy of digit %d : %d%%\n",k,accuracy*10);
		printf("-------------------------------\n");
		
	}

	printf("\n AVERGE ACCURACY OF DIGITS RECOGNIZER IS  : %d%%",avg);
	 
}

void load_model(int digit)		 //This function is used to load the trained model to test input on it.
{
	int i=1,j=1;

	strcpy(modelfilename,"model\\iteration_1\\204101011_");
	itoa(digit,modelfilename+strlen(modelfilename),10);
	strcat(modelfilename,"_it_1.txt");
	

	FILE *fp=fopen(modelfilename,"r");
	

	if(fp==NULL)
	{
		printf("problem in opening file %s", modelfilename);
		exit(0);
	}

	for(i=1;i<=n;i++)
		fscanf(fp,"%Lf",&pie[i]);       
		
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
				fscanf(fp,"%Lf",&a[i][j]);
	
	
	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
			fscanf(fp,"%Lf",&b[i][j]);

	printf("digit %d model loaded.... \n",digit);

	fclose(fp);
}


void load_file(int digit,int ut)			//This function used to load recording file for testing
{
	FILE *fp=fopen(obfilename,"r");
	T=0;
	while(true)
	{
		fscanf(fp,"%d",&o[++T]);
		if(feof(fp))
			break;
	}
	T--;

	printf("digit %d utterance %d file loaded....\n",digit,ut);

	fclose(fp);
}


long double test_input()			//This function apply testing on given input.
{
	int k=0;
	long double sum=0;
		
		alpha_computation();
	/*	beta_computation();
		gamma_computation();
		delta_computation();
		xi_computation();
		update_model();
		if(k==20)
			break;*/
	
	for(int i=1;i<=n;i++)
		sum+=alpha[T][i];

	return sum;

}


void alpha_computation()			//This function is used to compute parameter alpha
{

	int i=1,j=1,t=1;
	long double sum=0;


	for(i=1;i<=n;i++)					//Initializing alpha at t=1
	{
		alpha[1][i]=pie[i]*b[i][o[1]];
	}


	for(t=1;t<T;t++)					//Induction step
	{
		for(j=1;j<=n;j++)
		{
			for(i=1;i<=n;i++)
			{
				sum+=alpha[t][i]*a[i][j];
			}
			alpha[t+1][j]=sum*b[j][o[t+1]];
			sum=0;
		}
	}

	
}
