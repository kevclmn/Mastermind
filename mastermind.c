#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>   
#include "players.h"
 

int main (void){
int index=100;
int nGame=0;
int nPlayers=0;
struct typeGame games[MAX_GAMES];
struct typePlayer players[MAX_PLAYERS];
struct sortedPlayers ranking[MAX_PLAYERS];

while(index!=0){
    system("clear");
    header_mastermind();
    printf("\nWelcome, what do you want to do:\n1. Display all games\n2. Play game\n3. Display list of players\n4. Display ranking of players\n5. Display top players\n0. Exit\n");
    printf("Your option (Just Numeric): ");
    scanf("%d", &index);
    if(index==1){ 
      displayListOfGames(games, nGame);     
    }

    else if(index==2){

      if(selectPlayer(&games[nGame],players,nPlayers)==0){
      play(&games[nGame], &players[games[nGame].playerId]);
      players[games[nGame].playerId].nGPlayed++;  
      nGame++;
      }
      else  
    
    }
    else if(index==3){
        header_players();          //Cabecero ASCII
        displayListOfPlayers(players,&nPlayers);

    }
    else if(index==4){
      rankPlayers(players,ranking,nPlayers);    
    }
    else if(index==5){
      showTop(players, ranking, nPlayers);
    }
    else{
      system("clear");
    }
}
    system("exit");

}










  







