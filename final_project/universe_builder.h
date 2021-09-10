//universe_builder file is used to build a universe from input speech signals.




void initialization_ub();		//functions declaration
void compute_ai();
void compute_ri();
void compute_ci();
void compute_observation_symbol();
void compute_raised_ci();
void dcShift();
void normalize();
void append_universe(char*);
void compute_max_sample(char*);
long double compute_energy();
void read_codebook();
void write_universe();
long double abs1(long double);
int abs(int);


int FLAG_INPUT;				////global variable declartion
long double *sample;
long double *ri;
long double **ai;
long double *ci;
long double *k;
long double *e;
long double *ais;
long double DC_SHIFT= -1.154939;
//long double **codebook;
char *filename;
char *obfilename;
long double max_sample;


FILE *universe_file;
FILE *observation_sequence;





void build_universe(int flag_input)		//This function is entry point of the universe_buildeer.h file 
{
	int i=0,j=0;
	FLAG_INPUT=flag_input;
	
	initialization_ub();

	if(flag_input==1)
		read_codebook();
	
	for(i=0;i<numWords;i++)
	{
		for(j=1;j<=numUttrs;j++)
		{
			strcpy(filename,"dataset\\train\\");
			strcat(filename, userName);
			strcat(filename, "_");
			itoa(i,filename+strlen(filename),10);
			strcat(filename,"_");
			itoa(j,filename+strlen(filename),10);
			strcat(filename,".txt");

			if(FLAG_INPUT==1)
			{
				strcpy(obfilename,"dataset\\obs\\");
				strcat(obfilename, userName);
				strcat(obfilename, "_");
				itoa(i,obfilename+strlen(obfilename),10);
				strcat(obfilename,"_");
				itoa(j,obfilename+strlen(obfilename),10);
				strcat(obfilename,".txt");

				observation_sequence=fopen(obfilename,"w");

			}

			
			append_universe(filename);
			if(FLAG_INPUT==1)
			fclose(observation_sequence);
			
		}
	}

	fclose(universe_file);
}


//********************************function definitions***************************************************************************//

void initialization_ub()				//This function initialize the variable and file openings
{
	sample=(long double*)calloc(320,sizeof(long double));			//dynamic variable memory allocation
	ci=(long double*)calloc(13,sizeof(long double));
	ri=(long double*)calloc(13,sizeof(long double));
	ai=(long double**)calloc(13,sizeof(long double));
	for(int i=0;i<=12;i++)
		ai[i]=(long double*)calloc(13,sizeof(long double));
	k=(long double*)calloc(13,sizeof(long double));
	e=(long double*)calloc(13,sizeof(long double));
	filename=(char*)calloc(50,sizeof(char));
	obfilename=(char*)calloc(50,sizeof(char));

	//codebook=(long double**)calloc(33,sizeof(long double));
//	for(int i=0;i<=32;i++)
	//	codebook[i]=(long double*)calloc(13,sizeof(long double));

	strcpy(universeName, "universe\\");
	strcat(universeName, userName);
	strcat(universeName, ".txt");
	universe_file=fopen(universeName ,"a");
	
	if(universe_file==NULL)
	{
		printf("problem in opening file  universe.txt");
		exit(0);
	}


}


void append_universe(char *filename)			// This function append the universe.txt file with ci's vectors
{
	int count=0,i=0,overlap=240,flag=0,f=0;
	long double energy=0,energy1=0,energy2=0,energy3=0;
	bool start=false,end=false;

	compute_max_sample(filename);
	
	FILE *fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("problem in opening file %s ",filename);
		exit(0);
	}

	while(true)
	{
		if(count==0)
		{
		   for(i=0;i<320;i++)
		   {
				fscanf(fp,"%lf",&sample[i]);
				if(feof(fp))
				{	
					flag=1;
					break;
				}
		   }
		}
		else
		{
			for(i=0;i<320;i++)
		   {
			   if(i<overlap)
					sample[i]=sample[i+320-overlap];
			   else
			   {
					fscanf(fp,"%lf",&sample[i]);
					if(feof(fp))
					{	
						flag=1;
						break;
					}
				}
		   }
		}
	   if(flag==1)
		   break;
	   count++;
	   dcShift();
	   normalize();
	  /* energy=energy1;
	   energy1=compute_energy();
		if(count>=2)
		{
			if(energy1>=(2.5*energy))
				start=true;
			if(!start)
				continue;
			
			if((2.5*energy1)<=energy)
				end=true;
		}
		
		if(start&&!end)
		{*/
			compute_ri();
			compute_ai();
			compute_ci();
			compute_raised_ci();
			if(FLAG_INPUT==1)
				compute_observation_symbol();
			else
				write_universe();
			f++;
			
		//}
	
	}
	printf("\n total frames    %d ",f);
	printf("\n %s  is loaded into universe \n",filename);

	fclose(fp);
}

long double abs1(long double t)				//This function compute absolute value 
{
	if(t<0)
		return -t;
	else
		return t;
}

int abs(int t)				//This function compute absolute value 
{
	if(t<0)
		return -t;
	else
		return t;
}

void compute_max_sample(char *filename)			//This function compute the maximum sample for normalization
{
	long double x=0,max=0;
	FILE *fp1=fopen(filename,"r");


	if(fp1==NULL)
	{
		printf("problem in opening file %s",filename);
		exit(0);
	}
	
	while(true)
	{
		
		fscanf(fp1,"%Lf",&x);
		x=abs1(x);
		if(feof(fp1))
			break;
		if(max<x)
			max=x;
		
	}
	max_sample=max;

	fclose(fp1);
}



long double compute_energy()		//This function is used to compute energy of a frame
{
	long double sum=0;

	for(int i=0;i<320;i++)
	{
		sum+=sample[i]*sample[i];
	}
	return sum/320;

}

void dcShift()
{					
	int i=0;							//local variable declaration and initiallization
	
	for(i=0;i<320;i++)
	{
		sample[i]=sample[i]-DC_SHIFT;
	}
}


										
void normalize()		// This function is used to compute normalized values for each frame.
{
	
	int i=0;					

	for(i=0;i<320;i++)
	{
		sample[i]=(sample[i]*5000)/max_sample;
	}
}


void compute_ri()				//This function compute the ri's for input frame
{
	long double sum=0;
	
	for(int i=0;i<=12;i++)
	{
	
		for(int m=0;m<(320-i);m++)
		{
		sum+=sample[m]*sample[m+i];
		}
		ri[i]=sum;
	
		sum=0;
	
	}
}

void compute_ai()			//This function computes ai's for input frame
{
	int i=0,j=0;
	int p=12;
	long double sum=0;

	e[0]=ri[0];
	for(i=1;i<=p;i++){
		for(j=1;j<=i-1;j++){
			sum+=ai[i-1][j]*ri[abs(i-j)];
		}
		k[i]=(ri[i]-sum)/e[i-1];
		ai[i][i]=k[i];
		for(j=1;j<i;j++){
			ai[i][j]=ai[i-1][j]-(k[i]*ai[i-1][i-j]);
		}
		e[i]=(1-(k[i]*k[i]))*e[i-1];
	sum=0;
	}
	
}



void compute_ci()				//This function compute ci's for input frame
{	
	int p=12;
	long double x=0,sum=0,y=0;
	
	for(int m=1;m<=p;m++)
	{
		for(int k=1;k<=(m-1);k++)
		{
			x=m;
			y=k;
			sum+=(y/x)*ci[k]*ai[p][m-k];
		}

		ci[m]=ai[p][m]+sum;
	
	sum=0.0;
	}

}


void compute_raised_ci()				//this function is used to apply RAISED SINE WINDOW on ci's.
{	
	int i=0;
			
	for(i=1;i<=12;i++)
	{
		ci[i]=ci[i]*sine_window[i-1];
	}						

}


void write_universe()				//This function used to update the universe with ci vectors
{

	int i=0;
			
	for(i=1;i<=12;i++)
	{
		
		fprintf(universe_file,"%Lf",ci[i]);
		if(i!=12);
			fprintf(universe_file,",");
	}

	fprintf(universe_file,"\n");							


}

void read_codebook()			//This function read the codebook from file codebook.txt
{
	FILE *fp=fopen(codebookName,"r");

	if(fp==NULL)
	{
		printf("problem in opening file codebook.txt");
		exit(0);
	}

	for(int i=1;i<=32;i++)
	{
		for(int j=1;j<=12;j++)
		{
			fscanf(fp,"%Lf",&codebook[i][j]);
		}
	}

	fclose(fp);

}

void compute_observation_symbol()			//This function used to compute observation sequence for a input speech signal
{
	int minindex=0;
	long double sum=0,min=FLT_MAX;
	
	for(int i=1;i<=32;i++)
	{
		for(int j=1;j<=12;j++)
		{
			sum+=wt[j-1]*(ci[j]-codebook[i][j])*(ci[j]-codebook[i][j]);
		}

		if(min>sum)
		{
			min=sum;
			minindex=i;
		}
		sum=0;
	}

	fprintf(observation_sequence,"%d\n",minindex);
	printf("\n state =%d",minindex);
	
	

}