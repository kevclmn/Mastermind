#define ATTEMPTS 10  // max number of attempts
#define SIZE 4 		 // size of the secret code
#define NCOLORS 6    // number of possible color	
#define MAX_GAMES 50   // max number of games
#define MAX_PLAYERS 20 // max number of players
#define MAX_SCORE 100   //Set to 110 just to not change the whole scoring system (First attempt solution 100 point)
#define N_PLAYERS 10     
// structure to store player information
struct typePlayer{
   int id;			// player id
   char name[256];
   int score;			// score
   int nGPlayed;		// number of games played
};

// structure to store game information
struct typeGame {
	int nAttempts;
	int secretCode[SIZE];
	int board [ATTEMPTS][SIZE];
	int feedback [ATTEMPTS][2];
	int score;
	int playerId;

};
struct sortedPlayers{           //Nueva estructura para rankear
   int rank;			// player rank
   char name[256];
   char surname[256];
   int score;			// score
   int nGPlayed;		// number of games played
};


//FUNCIONES BÁSICAS FUNCIONALIDAD JUEGO
void generateSecretCode  (struct typeGame *game);
int verifyCode(struct typeGame game, int *black, int *white);	
void scanGuess (struct typeGame *game);
struct typeGame play(struct typeGame *game, struct typePlayer *player);
int selectPlayer(struct typeGame *game, struct typePlayer list_players[],int nPlayers);
  
//FUNCIONES PARA HACER DISPLAY
void displayBoard (struct typeGame game);
void displayGame(struct typeGame *game, struct typePlayer *player);
void displayListOfGames(struct typeGame listG[],int nGame);    

    //Estas son todas un pequeño mod de displayListOfPlayers del profe
void displayListOfPlayers(struct typePlayer listP[],int *nPlayers);
void MyDisplayListOfPlayers(struct typePlayer listP[],int nPlayers);  //Essentialy the same as the one made by the teacher. Just does not print score and attempts 
void displayRankOfPlayers(struct sortedPlayers listP[],int nPlayers); //Uses rank instead of id (It's used in Top players and Players Ranking)

//FUNCIONES PARA ORDENAR  (Tienen el display integrado)
void rankPlayers(struct typePlayer list_players[], struct sortedPlayers ranked_list[], int nPlayers); //Para rankear a los players
void showTop(struct typePlayer list_players[], struct sortedPlayers ranked_list[],int nPlayers);

//CABECEROS ASCII
void header_mastermind();
void header_players();
void header_rank();
void header_top();
void header_game();

//DESCARGA Y CARGA DE DATOS
void loadListOfGames(struct typeGame listG[], int *nGames);
int AddPlayers(struct typePlayer listP[],int *nPlayer);
//void storeGames();
void storePlayesr(struct typePlayer listP[MAX_PLAYERS], int nPlayers);
void loadPlayers();
