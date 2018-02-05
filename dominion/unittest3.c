#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {

	printf("\nTESTING HANDCARD() FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int k[10] = { adventurer, outpost, feast, gardens, mine, sea_hag, treasure_map, village, baron, curse };
	struct gameState currentGame;
	int handCount = 5;
	int testsFailed = 0;
	int currentPlayer = 0;
	int currentCard = 0;


	printf("\nTEST 1: FIRST CARD IN HAND \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	
	//assign the handCount and whose turn it is
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.whoseTurn = currentPlayer;
	
	//assign the cards a value
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = feast;
	
	//call the function to get the 1st card in the hand
	currentCard = handCard(0, &currentGame);

	if (currentCard == feast) {
		printf( "----TEST PASSED----  THE CORRECT CARD WAS RETURNED");
	}
	else {
		printf( "----TEST FAILED----  THE CORRECT CARD WAS NOT RETURNED");
		testsFailed++;
	}

	printf("\n\nTEST 2: THE LAST CARD IN HAND \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	//assign the handCount and whose turn it is
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.whoseTurn = currentPlayer;

	//assign the cards a value
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = copper;

	//call the function to get the last card in the hand
	currentCard = handCard(4, &currentGame);

	if (currentCard == copper) {
		printf( "----TEST PASSED----  THE CORRECT CARD WAS RETURNED");
	}
	else {
		printf( "----TEST FAILED----  THE CORRECT CARD WAS NOT RETURNED");
		testsFailed++;
	}

	printf("\n\nTEST 3: ENSURE THE CARD IS STILL IN HAND AFTER RETURNING\n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	//assign the handCount and whose turn it is
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.whoseTurn = currentPlayer;

	//assign the cards a value
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = copper;

	//call the function to get the 4thst card in the hand
	currentCard = handCard(3, &currentGame);

	if (currentCard == curse && currentGame.hand[currentPlayer][3] == curse) {
		printf( "----TEST PASSED----  THE CORRECT CARD WAS RETURNED AND THE PLAYER STILL HAS THE CARD IN THEIR HAND");
	}
	else {
		printf( "----TEST FAILED----  THE CORRECT CARD WAS NOT RETURNED");
		testsFailed++;
	}

	printf("\n\nTEST 4: ENSURE THE FUNCTION WORKS FOR PLAYER 2\n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	//make the current player, player 2
	currentPlayer = 1;
	
	//assign the handCount and whose turn it is
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.whoseTurn = currentPlayer;

	//assign the cards a value
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = copper;

	//call the function to get the 4thst card in the hand
	currentCard = handCard(3, &currentGame);

	if (currentCard == curse && currentGame.hand[currentPlayer][3] == curse) {
		printf( "----TEST PASSED----  THE CORRECT CARD WAS RETURNED AND THE PLAYER STILL HAS THE CARD IN THEIR HAND");
	}
	else {
		printf( "----TEST FAILED----  THE CORRECT CARD WAS NOT RETURNED");
		testsFailed++;
	}

	//alets user how many (if any) tests failed
	if (testsFailed == 0) {
		printf("\nALL TESTS PASSED\n");
	}
	else {
		printf("\nNOT ALL TESTS PASSED. NUMBER OF FAILS: %d\n", testsFailed);
	}

	return 0;
}