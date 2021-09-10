
// final_project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include<limits.h>
#include<Windows.h>


char userName[100];
char universeName[100];
char codebookName[100];
char obsName[100];
int numWords = 12;
int numUttrs = 20;


long double wt[]={1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
long double sine_window[]={1.000000000,2.552914271,4.000000000,5.242640687,6.196152423,6.795554958,7.000000000,6.795554958,6.196152423,5.242640687,4.000000000,2.552914271,1.000000000};
long double codebook[32][12];
char *words[] = {"bike","car","bus","train","bicycle","aeroplane","helicopter","boat","tractor","truck","rocket","jcb"};

#include"live_training.h"
#include"universe_builder.h"
#include"codebook_builder.h"
#include"model_builder.h"
#include"test_recording.h"



FILE *user_file = fopen("users.txt", "a+");
#include"play.cpp"
#include"train.cpp"

int _tmain(int argc, _TCHAR* argv[])
{
	int choice;
	while(1)
	{
		printf("Enter 1 to train.\nEnter 2. to Play.\nEnter 0 to exit:\t");
		scanf("%d", &choice);
		if(choice == 1)
			main_train();
		else if(choice == 2)
			main_play();
		else if(choice == 0)
			return 0;
		else
			printf("Wrong Choice.\n");
	}

	
	fclose(user_file);
	getch();
	return 0;
}

