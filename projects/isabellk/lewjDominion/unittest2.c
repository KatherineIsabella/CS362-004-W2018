#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {

	printf("\nTESTING ISGAMEOVER() FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int k[10] = { adventurer, outpost, feast, gardens, mine, sea_hag, treasure_map, village, baron, curse };
	struct gameState currentGame;
	int handCount = 5;
	int gameOverCheck;
	int testsFailed = 0;

	printf("\nTEST 1: All Province Cards Gone \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	
	currentGame.supplyCount[province] = 0;


	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 1) {
		printf( "----TEST PASSED----  THE GAME IS OVER");
	}
	else {
		printf( "---TEST FAILED----  THE GAME IS NOT OVER");
		testsFailed++;
	}

	printf("\n\nTEST 2: One supply pile gone \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentGame.supplyCount[adventurer] = 0;


	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 0) {
		printf( "----TEST PASSED----  THE GAME IS NOT OVER");
	}
	else {
		printf( "----TEST FAILED----  THE GAME IS OVER");
		testsFailed++;
	}


	printf("\n\nTEST 3: Two supply piles gone \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentGame.supplyCount[adventurer] = 0;
	currentGame.supplyCount[treasure_map] = 0;



	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 0) {
		printf( "----TEST PASSED----  THE GAME IS NOT OVER");
	}
	else {
		printf( "----TEST FAILED----  THE GAME IS OVER");
		testsFailed++;
	}


	printf("\n\nTEST 4: Three supply piles gone \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentGame.supplyCount[adventurer] = 0;
	currentGame.supplyCount[treasure_map] = 0;
	currentGame.supplyCount[gardens] = 0;



	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 1) {
		printf( "----TEST PASSED----  THE GAME IS OVER");
	}
	else {
		printf( "----TEST FAILED----  THE GAME IS NOT OVER");
		testsFailed++;
	}

	printf("\n\nTEST 5: Four supply piles gone \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentGame.supplyCount[adventurer] = 0;
	currentGame.supplyCount[treasure_map] = 0;
	currentGame.supplyCount[gardens] = 0;
	currentGame.supplyCount[sea_hag] = 0;



	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 1) {
		printf( "----TEST PASSED----  THE GAME IS OVER");
	}
	else {
		printf( "----TEST FAILED----  THE GAME IS NOT OVER");
		testsFailed++;
	}


	printf("\n\nTEST 6: Province pile empty and 2 supply piles empty \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentGame.supplyCount[adventurer] = 0;
	currentGame.supplyCount[treasure_map] = 0;
	currentGame.supplyCount[province] = 0;


	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 1) {
		printf( "----TEST PASSED----  THE GAME IS OVER");
	}
	else {
		printf( "----TEST FAILED----  THE GAME IS NOT OVER");
		testsFailed++;
	}


	printf("\n\nTEST 7: Province pile empty and 3 supply piles empty \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentGame.supplyCount[adventurer] = 0;
	currentGame.supplyCount[treasure_map] = 0;
	currentGame.supplyCount[gardens] = 0;
	currentGame.supplyCount[province] = 0;


	gameOverCheck = isGameOver(&currentGame);

	if (gameOverCheck == 1) {
		printf( "----TEST PASSED----  THE GAME IS OVER");
	}
	else {
		printf( "----TEST FAILED----  THE GAME IS NOT OVER");
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