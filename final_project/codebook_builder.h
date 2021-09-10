//codebook_builder.h file used to build the code book of size 32 from universe given as input.




void lbg_algo();			//functions declaration
void displayCodebook();
void initialization_cb();
void spliting();
void classification();
void code_vector_updating();
int compute_distance();
void compute_distortion();
long double abs(long double);
void splitvector(int,int);
void classifycluster(int,int);
void check_empty_cell();


int iteration;					//global variable declartion
int codebook_size;
long double spliting_factor=0.03;
long double distortion[200];

long double vector_cluster[32][12];
long double codevector[12];
int cluster_size[32];
long double universe[40000][12];
int vector_map[40000];
//long double wt[]={1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
int universe_size;
long double distortion_threshold=0.1;


int main1()
{
	lbg_algo();
	
	displayCodebook();
	//printf("done");

	return 0;
}



void lbg_algo()				//This function is implementation of LBG algorithm.
{
	initialization_cb();

	while(codebook_size<32)
	{
		
		spliting();

		do
		{
			iteration++;
			classification();
			check_empty_cell();
			code_vector_updating();
			compute_distortion();

			printf("codebook size =%d iteration= %d dis= %Lf\n\n\n",codebook_size,iteration,distortion[iteration]);

			/*	for(int i=0;i<codebook_size;i++)
			printf("cluster %d size =%d\n",i,cluster_size[i]);*/
	
		}while(abs(distortion[iteration-1]-distortion[iteration])>distortion_threshold);

		iteration=0;
		printf("\n");
	
		printf("\n\n\n");
	}

}



void initialization_cb()		//This function is used to initialize code book and initial distortion
{
	int t=0,count=0;
	long double x=0;
	long double *vector_sum=(long double*)calloc(12,sizeof(long double));
	FILE *fp=fopen(universeName,"r");

	if(fp==NULL)
		printf("problem in opening universe file ");

	
	while(true)
	{
		if(feof(fp))
			break;
		for(int i=0;i<12;i++)
		{
			fscanf(fp,"%Lf,",&x);
			universe[count][i]=x;
			vector_sum[i]+=x;
		}
		fscanf(fp,"\n");
		count++;
		
		
		
	}
	universe_size=count;

	for(int i=0;i<12;i++)
		codebook[0][i]=vector_sum[i]/universe_size;
	codebook_size=1;
	cluster_size[0]=universe_size;
	for(int i=0;i<universe_size;i++)
		vector_map[0]=0;
	compute_distortion();
	printf("codebook size =%d iteration= %d dis= %Lf  \n\n",codebook_size,iteration,distortion[iteration]);
	fclose(fp);
}


void spliting()				//This function is used to split the code book into two parts
{
	for(int i=0;i<codebook_size;i++)
	{
		for(int j=0;j<12;j++)
		{
			codebook[i+codebook_size][j]=codebook[i][j]*(1+spliting_factor);
			codebook[i][j]=codebook[i][j]*(1-spliting_factor);
		}
	}
	codebook_size=2*codebook_size;

}


void classification()				//This function is used to classify the universe into clusters.
{
	int count=0;
	long double x=0;
	

	
	
	for(int i=0;i<codebook_size;i++)
		cluster_size[i]=0;
	

	for(int j=0;j<universe_size;j++)
	{
		vector_map[j]=-1;
		for(int i=0;i<12;i++)
		{

			
			codevector[i]=universe[j][i];
			

		}
		
		vector_map[j]=compute_distance();
	}
	
	
		
	
}


void check_empty_cell()
{

	for(int i=0;i<codebook_size;i++)		//here the empty cell problem is handled
	{
		if(cluster_size[i]==0)
		{
			while(cluster_size[i]==0)
			{
				if(i>(codebook_size/2-1))
				{	
					splitvector(i-codebook_size/2,i);
					classification();
					//classifycluster(i,i-codebook_size/2);
				}
			
				else
					{
						splitvector(i+codebook_size/2,i);
						classification();
						//classifycluster(i,i+codebook_size/2);
					}

			}

		}
	}

}



void splitvector(int source,int destination)				//This function is used to split the vector when empty cell problem is occured
{
	for(int j=0;j<12;j++)
		{
			codebook[destination][j]=codebook[source][j]*(1+spliting_factor);
			codebook[source][j]=codebook[source][j]*(1-spliting_factor);
		}

}



void classifycluster(int index,int index1)		//This function is used to classify speacific cluster when empty cell problem is occured 
{	
	long double sum=0,min=FLT_MAX,sum1=0;
	int minindex=0;
	cluster_size[index1]=0;
	cluster_size[index]=0;
	for(int j=0;j<universe_size;j++)
	{
		if(vector_map[j]==index1)
		{
			for(int i=0;i<12;i++)
			{
				sum+=wt[i]*(universe[j][i]-codebook[index][i])*(universe[j][i]-codebook[index][i]);
				sum1+=wt[i]*(universe[j][i]-codebook[index1][i])*(universe[j][i]-codebook[index1][i]);
			}
			if(sum1<sum)
			{	
				vector_map[j]=index1;
				cluster_size[index1]++;
				
			}
			else
				cluster_size[index]++;
			sum=0;
			sum1=0;
		}
	}

	


}




int compute_distance()			//This function is used to compute distance so to apply Nearest neighbour rule
{
	long double min=FLT_MAX,sum=0;
	int minindex=0;

	
		for(int i=0;i<codebook_size;i++)
		{
			for(int j=0;j<12;j++)
			{
				sum+=wt[j]*(codevector[j]-codebook[i][j])*(codevector[j]-codebook[i][j]);
			}

			if(min>sum)
			{
				min=sum;
				minindex=i;
			}
			sum=0;
		}
		
		/*for(int i=0;i<12;i++)					//computing the sum of all code vectors that used while centroid computation
		{
			vector_cluster[minindex][i]+=codevector[i];
		
		}*/
		
		cluster_size[minindex]++;
	
	
	
	
	return minindex;
}





void code_vector_updating()				//This function is used to upadte code vectors based on the centroid condition
{
	long double sum[12]={0};
	int count=0;

	for(int i=0;i<codebook_size;i++)
	{
		for(int k=0;k<universe_size;k++)
		{
			if(vector_map[k]==i)
			{
				for(int j=0;j<12;j++)
				{	
					sum[j]+=universe[k][j];
					//codebook[i][j]=vector_cluster[i][j]/cluster_size[i];
				}
				count++;
			}

		}
		
		for(int j=0;j<12;j++)
		{	
 			codebook[i][j]=sum[j]/count;
			sum[j]=0;
		}
		count=0;
	}
	
}




void compute_distortion()			//This function is used to compute the overall distortion
{
	long double x=0,sum=0,sum1=0,min=FLT_MAX,dis=0;
		
	for(int i=0;i<codebook_size;i++)
			{	
				for(int j=0;j<universe_size;j++)
				{
					if(vector_map[j]==i)
					{
						for(int k=0;k<12;k++)
						{
					
							sum+=wt[k]*(universe[j][k]-codebook[i][k])*(universe[j][k]-codebook[i][k]);
					
						}
						
							
					}
				}
				dis+=sum;
				
				sum=0;
			}

	distortion[iteration]=dis/universe_size;
	dis=0;
	

}

void displayCodebook()						//This function is used to display codebook as output
{
	FILE *fp=fopen(codebookName,"w");

	if(fp==NULL)
	{
		printf("problem in opening file codebook.txt");
		exit(0);
	}
	for(int i=0;i<codebook_size;i++)
	{
		for(int j=0;j<12;j++)
		{
			printf("%Lf    ",codebook[i][j]);
			fprintf(fp,"%Lf\t",codebook[i][j]);
		}

		fprintf(fp,"\n");
		printf("\n");
	}
	fclose(fp);
}
/*
long double abs(long double x)			//This function is used to find absolute value of the input variable
{
	if(x<0)
		return -x;
	else
		return x;
}*/