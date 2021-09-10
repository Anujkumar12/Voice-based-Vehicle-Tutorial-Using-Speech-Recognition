void show_image(int result)											//Shows image of result for 5 seconds and then hides it.
{
	char imageName[500];
	strcpy(imageName, "images\\");
	itoa(result,imageName+strlen(imageName),10);
	strcat(imageName, ".jpg");

	system(imageName);											
	Sleep(5000);
	system("taskkill /IM \"Microsoft.Photos.exe\" /F");
}

int notExists(char *name2, char *fileName)							//To check whether username exists in users.txt
{																	//returns 0 if exists, 1 otherwise.

	FILE *fp = fopen(fileName, "r");
	char user[500];
	while(fscanf(fp, "%s", user) != EOF)
	{
		int i=0,flag = 0;
		while(name2[i] != '\0')
		{
			if(name2[i] != user[i])
			{
				flag = 1;
				break;
			}
			i++;
		}
		if(user[i] == '\0' && flag == 0)
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

void main_play()													//Users speak vehicle name and gets a respective image.
{
	printf("Enter existing name:  ");
	scanf("%s", userName);

	if(notExists(userName,"users.txt"))
	{
		printf("Error! User Not Found.\n");
		getch();
		return;
	}

	strcpy(universeName, "universe\\");
	strcat(universeName, userName);
	strcat(universeName, ".txt");

	strcpy(codebookName, "codebooks\\");
	strcat(codebookName, userName);
	strcat(codebookName, ".txt");

	strcpy(obsName, "dataset\\obs\\");
	while(1)
	{
		int choice;
		/*printf("Enter 0 to exit:\t");
		scanf("%d", &choice);
		if(choice == 0)
				return;*/
		
		int result = recording_test();

		if(result != -1)
			show_image(result);
		else
			printf("Speak Correctly!\n");
	}
}
