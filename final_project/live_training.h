




long double energy[500];
long double avg_energy;
long double sample1[320];

char fname[100];

void trim_signal(int,int,char *);
long double compute_energy(int);




void live_train(char *username)
{
	for(int i=0;i<numWords;i++)
	{
		for(int j=1;j<=numUttrs;j++)
		{
			printf("Word is %s.\nUttr is %d\n", words[i], j);
			printf("*******recording word *****************");
	
			system("Recording_Module.exe 2 input_file.wav input_file.txt");				//recording module call

			trim_signal(i,j, username);
			printf("\n%s  file saved \n",fname);
			getch();
		}
		printf("\nPress 1 to rerecord %s:\t", words[i]);
		int choice;
		scanf("%d", &choice);
		if(choice == 1)
			i--;
	}

	printf("\nrecording completed\n");
}



void trim_signal(int digit,int utterance, char *username)
{

	int count=0,f=0,max_index=0,start_frame=0,end_frame=0;
	long double x=0,max=0,sum=0,sum1=0,max_energy=0;
	FILE *fp1=fopen("input_file.txt","r");

	strcpy(fname,"dataset\\train\\");
	strcat(fname,userName);
	strcat(fname,"_");
	itoa(digit,fname+strlen(fname),10);
	strcat(fname,"_");
	itoa(utterance,fname+strlen(fname),10);
	strcat(fname,".txt");

	FILE *fp=fopen(fname,"w");
	if(fp1==NULL||fp==NULL)
	{
		printf("problem in opening file ");
		exit(0);
	}
	
	while(true)
	{
		
		fscanf(fp1,"%Lf",&x);
		sample1[count++]=x;
		//sum1+=x;
		
		
		if(feof(fp1))
			break;
		
		if(count==80)
		{	
			x=compute_energy(80);
			energy[f++]=x;
			sum+=x;
			count=0;
		}
		
		
	}
	
	avg_energy=sum/f;

	
	for(int i=25;i<f;i++)
	{
		if(energy[i]>=1.8*avg_energy)
		{
			start_frame=i-1;
			break;
		}
	}
	if(start_frame<=0)
		start_frame=1;
	else
		start_frame=start_frame-10;

	
	for(int i=f-1;i>0;i--)
	{
		if(energy[i]>=0.05*avg_energy)
		{
			end_frame=i-1;
			break;
		}
	}
	if(end_frame==0)
		end_frame=f-1;
	else
		end_frame=end_frame+20;

	if(end_frame>f)
		end_frame=f-10;

	printf("\nstart %d,end %d nframes %d \n",start_frame,end_frame,end_frame-start_frame);
	int f1=0;
	rewind(fp1);
	while(true)
	{
		
		fscanf(fp1,"%d",&x);
		if(feof(fp1))
			break;
		f1++;
		 if(f1>=start_frame*80&&f1<=end_frame*80)
		{
			
				fprintf(fp,"%d\n",x);
		}
		
		
		
		
	}



	fclose(fp);
	
	fclose(fp1);
}



long double compute_energy(int x)		//This function is used to compute energy of a frame
{
	long double sum=0;

	for(int i=0;i<x;i++)
	{
		sum+=sample1[i]*sample1[i];
	}
	return sum/x;

}