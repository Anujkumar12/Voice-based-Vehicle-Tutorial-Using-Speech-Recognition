
void main_train()
{
	
	printf("Enter name:  ");
	scanf("%s", userName);

	if(!notExists(userName, "users.txt"))							//Users may skip training if user data already exists
	{
		printf("User already exists. Press 1 to retrain:\t");
		int choice;
		scanf("%d", &choice);
		if(choice != 1)
			return;
	}

	strcpy(universeName, "universe\\");
	strcat(universeName, userName);
	strcat(universeName, ".txt");

	strcpy(codebookName, "codebooks\\");
	strcat(codebookName, userName);
	strcat(codebookName, ".txt");

	strcpy(obsName, "dataset\\obs\\");


	live_train(userName);											//Record all utterenes of user data
	build_universe(0);												//Builds universe of user recorded data
	
	main1();														//generates codebook
	
	build_universe(1);												//Generates observation Sequences.
	
	build_model();													//Models are built.
	fprintf(user_file, userName);									//User name is saved.
	fprintf(user_file, "\n");

}