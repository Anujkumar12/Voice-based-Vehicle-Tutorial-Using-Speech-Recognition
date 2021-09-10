
//model_builder.h file used to build/train the model .




void compute_model();		//function declaration
void sum_model();
void avg_model();
void store_model(int,int);
void initialization_mb();	
void initialize_input();
void alpha_computation();
void beta_computation();
void gamma_computation();
void delta_computation();
void xi_computation();
void update_model();
void display_model();
void sum_model_initialize();


long double a[6][6];			//global variable declaration and initialization
long double sum_a[6][6];
long double b[6][33];
long double sum_b[6][33];
int o[150];
long double pie[6];
long double sum_pie[6];
long double alpha[150][6];
long double beta[150][6];
long double gamma[150][6];
long double delta[150][6];
int psi[150][6];
long double p,p_old=1;
int q[150];
int n=5,m=32,T;
long double xi[150][6][6];





char obsfilename[50];
char modelfilename[50];






void build_model()
{
	
	
	for(int k=0;k<numWords;k++)
	{
		for(int i=1;i<=3;i++)
		{	
			iteration=i;
			sum_model_initialize();
			for(int j=1;j<=numUttrs;j++)
			{
				strcpy(obsfilename,"dataset\\obs\\");
				strcat(obsfilename,userName);
				strcat(obsfilename,"_");
				itoa(k,obsfilename+strlen(obsfilename),10);
				strcat(obsfilename,"_");
				itoa(j,obsfilename+strlen(obsfilename),10);
				strcat(obsfilename,".txt");


				


				compute_model();
				printf("%s\n", obsfilename);
				

				sum_model();
				
			}
			avg_model();
			//display_model();
			store_model(k,i);
			
		}
		
	}

}


void sum_model()
{
	int i=0,j=0;
	for(i=1;i<=n;i++)
	{
		sum_pie[i]+=pie[i];
	}
	

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			sum_a[i][j]+=a[i][j];
		}
		
	}

	

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			sum_b[i][j]+=b[i][j];
		}
		
	}

}

void sum_model_initialize()
{


	int i=0,j=0;
	for(i=1;i<=n;i++)
	{
		sum_pie[i]=0;
	}
	

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			sum_a[i][j]=0;
		}
		
	}

	

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			sum_b[i][j]=0;
		}
		
	}


}

void avg_model()
{
	int i=0,j=0;
	for(i=1;i<=n;i++)
	{
		pie[i]=sum_pie[i]/20;
	}
	

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			a[i][j]=sum_a[i][j]/20;
		}
		
	}

	

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			b[i][j]=sum_b[i][j]/20;
		}
		
	}



}




void compute_model()
{

	int k=0,i=0,j=0;
	if(iteration==1)
	{
		initialization_mb();
	}

	while(true)
	{	
		k++;
		
		alpha_computation();
		beta_computation();
		gamma_computation();
		delta_computation();
		xi_computation();
		update_model();

	//	printf("iteration=%d   p=%Le  q=%d\n",k,p,q[T]);
	/*	for(i=1;i<=T;i++)
			printf("%d  ",q[i]);*/

		//printf("\n\n\n");
		if(k==20)
			break;
	}
}


void display_model()
{
	int i=0,j=0;
	printf("\n\n\n pi matrix --\n");

	for(i=1;i<=n;i++)
	{
		printf("%.20Lf\t",pie[i]);
	}
	printf("\n\n a matrix--\n");

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			printf("%Le    ",a[i][j]);
		}
		printf("\n");
	}

	printf("\n\n\n b matrix --\n");

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			printf("%Le   ",b[i][j]);
		}
		printf("\n");
	}

}


void store_model(int digit,int iteration)
{
	int i=0,j=0;
	strcpy(modelfilename,"models\\iteration_");
	itoa(iteration,modelfilename+strlen(modelfilename),10);
	strcat(modelfilename,"\\");
	strcat(modelfilename, userName);
	strcat(modelfilename, "_");
	itoa(digit,modelfilename+strlen(modelfilename),10);
	strcat(modelfilename,"_it_");
	itoa(iteration,modelfilename+strlen(modelfilename),10);
	strcat(modelfilename,".txt");
	printf("%s\n", modelfilename);
	FILE *fp=fopen(modelfilename,"w");

	if(fp==NULL)
	{
		printf("proble in opening file %s ",modelfilename);
		exit(0);
	}

	for(i=1;i<=n;i++)
	{
		fprintf(fp,"%.20Lf\t",pie[i]);
	}
	fprintf(fp,"\n\n");

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			fprintf(fp,"%Le\t",a[i][j]);
		}
		fprintf(fp,"\n");
	}

	fprintf(fp,"\n\n");

	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			fprintf(fp,"%Le ",b[i][j]);
		}
		fprintf(fp,"\n");
	}


	fclose(fp);
	printf(" for iteration %d digit %d  model is saved successfully \n",iteration,digit);
}




void initialization_mb()				//This Function is used to initialize the model 
{
	int i=1,j=1;

	FILE *fp=fopen("a_matrix.txt","r");
	FILE *fp1=fopen("b_matrix.txt","r");
	FILE *fp2=fopen("pie_matrix.txt","r");
	FILE *fp3=fopen(obsfilename,"r");


	if(fp==NULL||fp1==NULL||fp2==NULL||fp3==NULL)
	{
		printf("problem in opening file");
		getch();
		exit(0);
	}

	
	while(true)									//initializing a-matrix
	{
		for(j=1;j<=n;j++)
			fscanf(fp,"%Lf ",&a[i][j]);
		i++;
		if(feof(fp))
			break;
	}
	
	
	i=1;
	while(true)									//initializing b-matrix
	{
		for(j=1;j<=m;j++)
			fscanf(fp1,"%Lf ",&b[i][j]);
		i++;
		if(feof(fp1))
			break;
	}
	
	
	i=1;
	while(true)								//initializing pie-matrix
	{
		fscanf(fp2,"%Lf ",&pie[i++]);
		if(feof(fp2))
			break;
	}

	T=0;
	while(true)								//initializing observation sequence
	{
		fscanf(fp3,"%d",&o[++T]);
		if(feof(fp3)||T==149)
			break;
	}
	T--;
	
	


	fclose(fp);							//File Closing
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	
}


void initialize_input()
{
	

	


	
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



void beta_computation()						//This function is used to compute parameter beta
{

	int i=1,j=1,t=1;
	long double sum=0;


	for(i=1;i<=n;i++)						//Initializing beta at t=T
	{
		beta[T][i]=1;
	}


	for(t=T-1;t>=1;t--)						//Induction step
	{
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				sum+=a[i][j]*b[j][o[t+1]]*beta[t+1][j];
			}
			beta[t][i]=sum;
			sum=0;
		}
	}

	

}


void gamma_computation()					//This function is used to compute parameter gamma
{


	int i=1,j=1,t=1;
	long double sum=0;

	for(t=1;t<=T;t++)
	{
		for(j=1;j<=n;j++)								//check here
			{
				sum+=alpha[t][j]*beta[t][j];
			}
		for(i=1;i<=n;i++)
		{
			gamma[t][i]=(alpha[t][i]*beta[t][i])/sum;
		}
		sum=0;
	}

}


void delta_computation()								//This function is used to compute parameter delta
{

	int i=1,j=1,t=1,maxindex=0;
	long double sum=0,max=FLT_MIN;

	for(i=1;i<=n;i++)									//Initialization step
	{
		delta[1][i]=pie[i]*b[i][o[1]];
		psi[1][i]=0;
	}

	for(t=2;t<=T;t++)								//Recursive step
	{
		for(j=1;j<=n;j++)
		{
			max=delta[t-1][1]*a[1][j];
			maxindex=1;
			for(i=2;i<=n;i++)
			{
				sum=delta[t-1][i]*a[i][j];

				if(max<sum)
				{
					max=sum;
					maxindex=i;
				}
			}
			delta[t][j]=max*b[j][o[t]];
			psi[t][j]=maxindex;
		}
		max=FLT_MIN;
		maxindex=0;
	}

	p_old=p;
	p=delta[T][1];
	q[T]=1;
	for(i=2;i<=n;i++)							//Termination step
	{
		
		if(p<delta[T][i])
		{
			p=delta[T][i];
			q[T]=i;
		}
	}

	for(t=T-1;t>=1;t--)
	{
		q[t]=psi[t+1][q[t+1]];
	}


}


void xi_computation()							//This function is used to compute parameter xi
{

	int i=1,j=1,t=1;
	long double sum=0;

	for(t=1;t<T;t++)
	{
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				sum+=alpha[t][i]*a[i][j]*b[j][o[t+1]]*beta[t+1][j];
			}
		}

		for(i=1;i<=n;i++)
		{
			for(j=1;j<=n;j++)
			{
				xi[t][i][j]=(alpha[t][i]*a[i][j]*b[j][o[t+1]]*beta[t+1][j])/sum;
			}
		}
		sum=0;
	}
	

	
}


void update_model()										//This function is used to adjust the model
{

	int i=1,j=1,t=1,k=1,count=0,maxindex=0;
	long double sum_xi=0,sum_gamma=0,sum1=0,max=0;

	for(i=1;i<=n;i++)			//updating pi matrix
	{
		pie[i]=gamma[1][i];
	}



	for(i=1;i<=n;i++)		//updating a matrix.
	{
		
			sum_gamma=0;
		for(t=1;t<T;t++)
			{
			
				sum_gamma+=gamma[t][i];
			}
		for(j=1;j<=n;j++)
		{
			sum_xi=0;
			for(t=1;t<T;t++)
			{
				sum_xi+=xi[t][i][j];
			
			}
			a[i][j]=sum_xi/sum_gamma;
			
		}
	}

	sum_gamma=0;
	for(j=1;j<=n;j++)			//updating b matrix.
	{
		for(k=1;k<=m;k++)
		{
			sum_gamma=0;
			sum1=0;
			for(t=1;t<=T;t++)
			{
				if(o[t]==k)
					sum1+=gamma[t][j];

				sum_gamma+=gamma[t][j];
			}
			
				b[j][k]=sum1/sum_gamma;
			
			
		}
	}


	
	long double th=powf(10,-30);			//checking for threshold for b matrix.
	sum1=0;
	for(j=1;j<=n;j++)
	{
		count=0;
		max=0;
		maxindex=0;
		sum1=0;
		
		for(k=1;k<=m;k++)
		{
			if(max<b[j][k])
			{
				max=b[j][k];
				maxindex=k;
			}
			if(b[j][k]<th)
			{

				count++;
				sum1+=b[j][k];
				b[j][k]=th;
			}
		}
		b[j][maxindex]=b[j][maxindex]-count*th+sum1;
		
	}
}