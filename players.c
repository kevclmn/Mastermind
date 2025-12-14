#include <string.h> // Te deja usar strcpy 
#include <stdio.h>  // La clasica 
#include <stdlib.h> // Random stuff
#include <time.h>   // El tiempo
#include <math.h>   // Deja usar pow

#include "players.h"

// prototypes of  functions not used outside the library, only local
// prototypes of the functions that can be used in the program are available in players.h
void initFeedback(int board[ATTEMPTS][2]);
void initBoard(int board[ATTEMPTS][SIZE]);
void generate(int code[]);
int verify(int code[], int guess[], int *black, int *white);

// *****************************
// **** library functions   *****
// *****************************


// *****************************************************
// **** Funcionalidad basicas. Funcionalidad Juego *****
// *****************************************************

void generateSecretCode  (struct typeGame *game){
	// returns a vector of four elements containing a random secret code
  int colorPegs[NCOLORS]={1, 2, 3, 4, 5, 6};  //vector containing available pegs
  int t=NCOLORS; //number of available pegs, initially 8
  int num;
  int i, j;
  
  for (i=0; i<SIZE; i++){
      num=rand()%t;     //generate random number from 0 to t-1
      game->secretCode[i]=colorPegs[num];
      //we remove that colour from the vector containing available pegs
      // we move them all one position forward 
      for (j=num; j<t; j++){
          colorPegs[j]=colorPegs[j+1];
      }
      t=t-1; //number of available pegs is updated
  }
  return;
}

int verifyCode(struct typeGame game, int *black, int *white){
	// secretCode: secretCode to verify (input) 1x4
	// guess: colors entered by the user (input) 1x4
	// feedback = number of white and black pegs (output, by reference)
	// return; 1 if valid, 0 if not valid
  int i=0;
  int j=0;
  for(i=0;i<SIZE;i++){
    for(j=0; j<SIZE; j++){
      if(game.secretCode[i]==game.board[game.nAttempts][j] && i==j){
        (*black)++;
      }
      if(game.secretCode[i]==game.board[game.nAttempts][j] && i!=j){
        (*white)++;
      }
    }
  }

 return 1;
}

void scanGuess (struct typeGame *game){
	// reads values for a vector of size t	
	int i;
  int num=123; // Store the ibput
  int check=123;
	scanf("%d",&num);
  // &v[0]=(num/1000)%10;
  // &v[1]=(num/100)%10;   Thats the concept of the for, which decomposes the number
  // &v[2]=(num/10)%10;
  // &v[3]=num%10;
  if(num==6969){          //Codigo secreto para que se chive el juego
    while(check!=0){
      printf("You are a cheater...\nThe code is: ");
      for(i=0;i<SIZE;i++){
        printf("%d",game->secretCode[i]);
      }
      printf("\nType 0 to continue as nothing happened: ");
      scanf("%d",&check);
    }
  }
  for (i=0; i<SIZE; i++){ 
   game->board[game->nAttempts][i]=(int)(num/(pow(10,SIZE-i-1)))%10;
  }
	return;
}

struct typeGame play(struct typeGame *game, struct typePlayer *player){
  setbuf(stdout, NULL); //for debugging purposes
	int b=0,w=0; // vars for number of blacks and number of whites
  int check=123;
	srand (time(NULL));   // seed random number generator
  system("clear");
  
  //Welcome message
  generateSecretCode(game);

  printf("Hi, welcome to mastermind\n");
  printf("To win you have to guess a %d digit code\n", SIZE);

  while(game->nAttempts<ATTEMPTS){

    displayBoard(*game);

    printf("\nGuess nº %i  (Up to %d numbers): ", game->nAttempts+1,SIZE);
    scanGuess(game);
    verifyCode(*game,&b,&w);

    game->feedback[game->nAttempts][0]=b;
    b=0;
    game->feedback[game->nAttempts][1]=w;
    w=0;

    if(game->feedback[game->nAttempts][0]==SIZE){
      game->nAttempts++;
      displayGame(game, player);    //Mensaje de victoria + cambio scores  
      return *game;
    }

    
    system("clear");
    game->nAttempts++;
  }
  while(check!=0){
    system("clear");
    printf("Ohh you are such a bad decoder. The code was ");
      for(int i=0; i<SIZE; i++){
        printf("%d",game->secretCode[i]);
      }
    printf("\nMaybe you are luckier next time.");
    printf("\nType 0 to exit: ");
    scanf("%d",&check);
	}
  system("clear");
  return *game;
  
}

int selectPlayer(struct typeGame *game, struct typePlayer list_players[],int nPlayers){    //Cambiamos el id del jugador para que se estoree ahí la info. Id=3 luegoal usar loadListOfPlayers[i] i=Id
  
  int input = 0;                             //Variable para ajustar el input a la posición natural de una lista array 1-> arr[0]
  do{
    system("clear");
    // displayListOfPlayers(struct typePlayer listP[],int nPlayers)   
    printf("Who is going to play?");
    MyDisplayListOfPlayers(list_players,nPlayers);
    printf("\nType the Id of the player (0 to exit): ");
    scanf("%d", &input);
    if(input==0){
      return 1;   
    }

  }while(input>nPlayers || input<0 );
  game->playerId=input-1;
  return 0;
}

// ****************************************
// **** Funcionalidad para displayear *****
// ****************************************

void displayBoard (struct typeGame game){
 //Create some kind of header for the table.
  printf("| YOUR GUESS | Black White|\n");
  printf("---------------------------\n");

  //The plan is to create a board initiallized by zeros, and dinamicly changes with the feedback and tries
   
  for(int i=0;i<game.nAttempts;i++){
    printf("|");
    for(int j=0; j<SIZE; j++){
      printf(" %d ",game.board[i][j]);
    }
    printf("|");
    for(int k=0; k<2; k++){
      printf("   %d  ",game.feedback[i][k]);
    }
    printf("|\n");
  }
  for(int i=0;i<ATTEMPTS-game.nAttempts;i++){
      printf("|");
      for(int j=0; j<SIZE; j++){
        printf(" - ");
      }
        printf("|");  
      for(int k=0; k<2; k++){
        printf("   -  ");
    }
    printf("|\n");
  }

  return;
}

void displayGame(struct typeGame *game, struct typePlayer *player){
  int check=1;
    while(check!=0){
      system("clear");                          //Hay dos escores
      game->score=(MAX_SCORE+10)-game->nAttempts*10; //Score de la partida (El +10 arregla el desfase para que 1º Attempt -> 100 puntos)
      player->score=player->score+game->score;  //Score global del jugador Se supone que tenía que ser una funciío a parte pero ya esta implementado. No veo la utilidad de la función updatePlayersScore
  /*Note that there are other more efficient ways to implement this, such as updating the score every time a  new game is played. We will not do this to simplify the project, instead, we will recalculate all the scores  before displaying the players, every time we want to see the players.*/ 
      //Eso lo dice en el step 6, supongo que no habrá problema en dejarlo como lo tenemos. Creo que esa es una de las formas más "eficientes"
      displayBoard(*game);
      printf("Congratulations!!! You broke the code with just %d attempts.\nThose are %d points",game->nAttempts,game->score);
      printf("\n\nType 0 to exit: ");
      scanf("%d",&check);
    }
  return;
}

void displayListOfGames(struct typeGame listG[],int nGame){
  int check=123;
  while(check!=0){
  system("clear");
  header_game();
  printf("|  Game  |  Secret Code  |  Score  |  Attempts|\n");
  printf(" --------------------------------------------- \n");
  for(int j=0; j<nGame;j++){
  printf("     %d     ",j+1);
  printf("     ");
  for(int i=0; i<SIZE; i++){
    printf("%d",listG[j].secretCode[i]);
  }
  printf("     ");
  printf("     %d          %d     \n", listG[j].score, listG[j].nAttempts);
  }
  printf("\n\n\nInsert 0 to exit: ");
  scanf("%d",&check);
  }
  return;
}

void displayListOfPlayers(struct typePlayer listP[],int *nPlayers){
	int i;
  int check=123;
  system("clear");

	while(check!=0){
    printf("\n\nid\tUser       \tScore\tnGames\n");
	  printf("--\t----       \t-----\t------\n");
 	  for (i=0;i<*nPlayers;i++){ 
	  	printf("%i\t", listP[i].id);
		  printf("%-10s \t", listP[i].name);  
		  printf("%i\t", listP[i].score);
	  	printf("%i\n", listP[i].nGPlayed);
	  }
    printf("\n\n0 - Exit\n1 - Add new player\nInput: ");
    scanf("%d",&check);
    if(check==1){
      AddPlayers(listP,nPlayers);
      return;
    }
  }
  return; 
}
// Modificacion de la del profe para usar en el programa
void MyDisplayListOfPlayers(struct typePlayer listP[],int nPlayers){
	int i;
	printf("\n\nid\tUser\n");
	printf("--\t----\n");
 	for (i=0;i<nPlayers;i++){ 
		printf("%i\t", listP[i].id);
		printf("%-10s \n", listP[i].name);
	}
}
// Otro mod del display del profe para ensñar el rank
void displayRankOfPlayers(struct sortedPlayers listP[],int nPlayers){
	int i;
	printf("\n\nRank\tUser       \tScore\tnGames\n");
	printf("--\t----       \t-----\t------\n");
 	for (i=0;i<nPlayers;i++){ 
		printf("%i\t", listP[i].rank);
		printf("%-10s \t", listP[i].name);
		printf("%i\t", listP[i].score);
		printf("%i\n", listP[i].nGPlayed);
	}
}


// ****************************************
// ****   Funcionalidad para ordenar   ****
// ****************************************

void rankPlayers(struct typePlayer list_players[], struct sortedPlayers ranked_list[],int nPlayers){
  //Primero copiar los datos de estructura a estructura 
  int check=0;
  do{
  system("clear");
  header_rank();           //Cabecero ASCII 
  for(int i=0;i<nPlayers;i++){
    strcpy(ranked_list[i].name, list_players[i].name);
    ranked_list[i].score=list_players[i].score;
    ranked_list[i].nGPlayed=list_players[i].nGPlayed;
    ranked_list[i].rank=list_players[i].id;
  }
  //Ahora toca sortear la lista
  int copyRank=0;
  int i=0;        //Valor para no perder posiciones
  int j=0;
  for(j=0;j<nPlayers;j++){
    for(i=0;i<nPlayers-1;i++){
      if(ranked_list[i].score<ranked_list[i+1].score){
        //Primero cambia los rank y luego el pointer (posición)
        copyRank=ranked_list[i].rank;
        ranked_list[i].rank=ranked_list[i+1].rank;
        ranked_list[i+1].rank=copyRank;
        struct sortedPlayers copyStruct = ranked_list[i];
        ranked_list[i]=ranked_list[i+1];
        ranked_list[i+1]=copyStruct;
      }
    }
  }
  displayRankOfPlayers(ranked_list,nPlayers);
  printf("\n\nType 0 to exit: "); 
  scanf("%d",&check);
  }while(check!=0);
  return;
}

void showTop(struct typePlayer list_players[], struct sortedPlayers ranked_list[],int nPlayers){
  int check=0;
  int top=0;
  system("clear");
  header_top();            //Cabecero ASCII
  printf("How many player do you want to show in the top: ");
  scanf("%d", &top);
  if(top>nPlayers){
    top=nPlayers;
  }
  else if(top<=0){
    return;
  }
  do{
  system("clear");
  header_top(); 
  printf("How many player do you want to show in the top: %d",top); //Hace que si tipeas algo !=0 no te permita cambiar el top
  for(int i=0;i<nPlayers;i++){
    strcpy(ranked_list[i].name, list_players[i].name);
    ranked_list[i].score=list_players[i].score;
    ranked_list[i].nGPlayed=list_players[i].nGPlayed;
    ranked_list[i].rank=list_players[i].id;
  }
  //Ahora toca sortear la lista
  int copyRank=0;
  int i=0;        //Valor para no perder posiciones
  int j=0;
  for(j=0;j<nPlayers;j++){
    for(i=0;i<nPlayers-1;i++){
      if(ranked_list[i].score<ranked_list[i+1].score){
        //Primero cambia los rank y luego el pointer (posición)
        copyRank=ranked_list[i].rank;
        ranked_list[i].rank=ranked_list[i+1].rank;
        ranked_list[i+1].rank=copyRank;
        struct sortedPlayers copyStruct = ranked_list[i];
        ranked_list[i]=ranked_list[i+1];
        ranked_list[i+1]=copyStruct;
      }
    }
  }
  displayRankOfPlayers(ranked_list,top);
  printf("\n\nType 0 to exit: "); 
  scanf("%d",&check);
  }while(check!=0);
  return;

}

// ****************************************
// ****     Funcionalidad arte ACII     ***
// ****************************************

void header_mastermind(){
  printf("\n███╗░░░███╗░█████╗░░██████╗████████╗███████╗██████╗░███╗░░░███╗██╗███╗░░██╗██████╗░\n");
  printf("████╗░████║██╔══██╗██╔════╝╚══██╔══╝██╔════╝██╔══██╗████╗░████║██║████╗░██║██╔══██╗\n");
  printf("██╔████╔██║███████║╚█████╗░░░░██║░░░█████╗░░██████╔╝██╔████╔██║██║██╔██╗██║██║░░██║\n");
  printf("██║╚██╔╝██║██╔══██║░╚═══██╗░░░██║░░░██╔══╝░░██╔══██╗██║╚██╔╝██║██║██║╚████║██║░░██║\n");
  printf("██║░╚═╝░██║██║░░██║██████╔╝░░░██║░░░███████╗██║░░██║██║░╚═╝░██║██║██║░╚███║██████╔╝\n");
  printf("╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═════╝░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═════╝░\n");
  return;
}
void header_players(){
printf("\n██████╗ ██╗      █████╗ ██╗   ██╗███████╗██████╗ ███████╗\n");
printf("██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗██╔════╝\n");
printf("██████╔╝██║     ███████║ ╚████╔╝ ███████╗██████╔╝███████╗\n");
printf("██╔═══╝ ██║     ██╔══██║  ╚██╔╝  ██═════╝██╔══██╗╚════██║\n");
printf("██║     ███████╗██║  ██║   ██║   ███████║██║  ██║███████║\n");
printf("╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝\n");
}
void header_rank(){
printf("\n██████╗  █████╗ ███╗   ██╗██╗  ██╗\n");
printf("██╔══██╗██╔══██╗████╗  ██║██║ ██╔╝\n");
printf("██████╔╝███████║██╔██╗ ██║█████╔╝ \n");
printf("██╔══██╗██╔══██║██║╚██╗██║██╔═██╗ \n");
printf("██║  ██║██║  ██║██║ ╚████║██║  ██╗\n");
printf("╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝\n");
}
void header_top(){
printf("\n████████╗ ██████╗ ██████╗\n");
printf("╚══██╔══╝██╔═══██╗██╔══██╗\n");
printf("   ██║   ██║   ██║██████╔╝ \n");
printf("   ██║   ██║   ██║██╔═══╝ \n");
printf("   ██║   ╚██████╔╝██║     \n");
printf("   ╚═╝    ╚═════╝ ╚═╝     \n");

}
void header_game(){
printf("\n ██████╗  █████╗ ███╗   ███╗███████╗███████╗\n");
printf("██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔════╝\n");
printf("██║  ███╗███████║██╔████╔██║█████╗  ███████╗\n");
printf("██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ╚════██║\n");
printf("╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗███████║\n");
printf(" ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚══════╝\n\n");

}
// ********************************************
// **** Funciones para inicializar struct *****
// ********************************************
  
int AddPlayers(struct typePlayer listP[],int *nPlayer){
  char user[256];
  int verify=123;
  if(*nPlayer==MAX_PLAYERS){
      while(verify!=0){
        printf("You have reached the maximum of players\nType 0 to exit: ");
        scanf("%d",&verify);
      }
      return 0;
  }

  printf("\n\nType the new username: ");
  scanf("%s",user);
  while(verify!=0){

    printf("\nIs %s correct?\nExit->0 Yes->1 No->2: ",user);
    scanf("%d",&verify);
    if(verify==0){

    }
    if(verify==1){
      strcpy(listP[*nPlayer].name,user);
      listP[*nPlayer].id=*nPlayer+1; 
      *nPlayer=*nPlayer+1;
      return 1;
    }
    if(verify==2){
      printf("Type the new username: ");
      scanf("%s",user);
    }

  }
    return 0;
}


void loadListOfGames(struct typeGame listG[], int *nGames){
	int i,j,b,w;
	int r; // row in the board
	int n; // local var for nAttempts
	*nGames=rand()%6+6; //random number of games from 5 to 11
	
	for (i=0;i<*nGames;i++){ 
		initBoard(listG[i].board); //initialize Board to zeros
		initFeedback(listG[i].feedback); //initialize feedback to zeros
		
		// generate random number of attempts
		n=rand()%10+1; // nAttempts from 1 to 11

		// generate secret code
		generate(listG[i].secretCode);
				
		// copy secret code to last row according to number of attempts n, i.e. n-1
		for (j=0;j<4;j++){ 	// copy secret code last row
			listG[i].board[n-1][j]=listG[i].secretCode[j];
		}
		
		// fill in remaining rows with random values
		for  (r=0;r<n-1;r++){ 	
			generate(listG[i].board[r]); // use generateSecretCode to generate random guess -- it is not the secret code here
		}
				
		if (n == ATTEMPTS){ 
			// two options 10 and last row correct or 10 and last wrong
			if (rand()%2){
				// last row is wrong
				// change one value of last row -- for example, add one
				listG[i].board[n-1][1]=	listG[i].board[n-1][1]+1;
				// if adding one we get number 7, put it back to one
				if (listG[i].board[n-1][1]==7)
					listG[i].board[n-1][1]=1;
			}
		}
		
		// use verify code to populate feedback 
		for  (r=0;r<n;r++){ 
			verify(listG[i].secretCode, listG[i].board[r], &b, &w);
			listG[i].feedback[r][0]=b; //4 blacks
			listG[i].feedback[r][1]=w; //0 white
		}
			
		// set score and nAttempts	
		listG[i].nAttempts=n;
		listG[i].score=110-(listG[i].nAttempts*10);	
		if ((n == ATTEMPTS)&&(listG[i].feedback[ATTEMPTS][0]==4)) {//  code not guessed
			listG[i].score=0;
		}
		// assign random player
		listG[i].playerId=rand()%N_PLAYERS+1;	
	} 
}

void storePlayesr(struct typePlayer listP[MAX_PLAYERS], int nPlayers){
  FILE* datafile = fopen("players.txt","w");
  for(int i=0;i<nPlayers;i++){
    fprintf(datafile, "%d ",listP[i].id);
    fprintf(datafile, "%s ",listP[i].name);
    fprintf(datafile, "%d ",listP[i].score);
    fprintf(datafile, "%d\n",listP[i].nGPlayed);
  }
  fclose(datafile);
  return;
}
// *****************************
// *** functions not available in the library, local	
// *****************************

void initBoard(int m[ATTEMPTS][SIZE]){
	int i,j;
	for (i=0;i<ATTEMPTS;i++){ 
	  for (j=0;j<SIZE;j++){  
	  	m[i][j]=0;
		}	
	}
}

void initFeedback(int m[ATTEMPTS][2]){
	int i,j;
	for (i=0;i<ATTEMPTS;i++){ 
		for (j=0;j<2;j++){  
			m[i][j]=0;
		}
	}
}

void generate  (int code[]){
  int colorPegs[NCOLORS]={1, 2, 3, 4, 5, 6}; 
  int t=NCOLORS; 
  int num;
  int i, j;
  
  for (i=0; i<SIZE; i++){
      num=rand()%t;     
      code[i]=colorPegs[num];
      for (j=num; j<t; j++){
          colorPegs[j]=colorPegs[j+1];
      }
      t=t-1; 
  }
  return;
}

int verify(int code[], int guess[], int *blacks, int *whites){
	*whites=0;  
	*blacks=0;
	int valid=0; 
	int i, j;
	for (i=0; i<SIZE; i++){
		for(j=0; j< SIZE; j++){
			if (code[i]==guess[j]){
				if (i==j)
					(*blacks)++;
				else
					(*whites)++;
			}
		}
	}
	if (*blacks==SIZE){
		valid=1;
	}
	return valid;
}



