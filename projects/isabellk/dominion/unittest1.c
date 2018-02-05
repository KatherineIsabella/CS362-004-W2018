#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {
	
	printf("\nTESTING UPDATECOINS() FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int currentPlayer = 0;
	int bonus = 0;
	int k[10] = { adventurer, outpost, feast, gardens, mine, sea_hag, smithy, village, baron, curse };
	struct gameState currentGame;
	int handCount = 5;
	int properReturn;
	int testsFailed = 0;

	printf("\nTEST 1: All cards copper, no bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	//make all the cards in the hand copper
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = copper;
	}

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//copper is worth 1 coin so handcount * 1 + bonus is what the coins should be 
	if (currentGame.coins == (handCount * 1 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf("----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}

	printf("\n\nTEST 2: All cards silver, no bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	//make all the cards in the hand silver
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = silver;
	}

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//silver is worth 2 coin so handcount * 2 + bonus is what the coins should be 
	if (currentGame.coins == (handCount * 2 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}


	printf("\n\nTEST 3: All cards gold, no bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	//make all the cards in the hand gold
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = gold;
	}

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//gold is worth 3 coin so handcount * 3 + bonus is what the coins should be 
	if (currentGame.coins == (handCount * 3 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE   COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}



	printf("\n\nTEST 4: No Treasure Cards, no bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	//make all the card non-treasure cards
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = feast;

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//no treasure cards to coins should be 0 + bonus
	if (currentGame.coins == (handCount * 0 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}


	printf("\n\nTEST 5: No Treasure Cards, a bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	bonus = 2;

	//make all the card non-treasure cards
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = feast;

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//no treasure cards to coins should be 0 + bonus
	if (currentGame.coins == (handCount * 0 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}

	printf("\n\nTEST 6: All Treasure Cards, mix of types, no bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	bonus = 0;

	//make the hand a mix of the Treasure cards
	currentGame.hand[currentPlayer][0] = copper;
	currentGame.hand[currentPlayer][1] = silver;
	currentGame.hand[currentPlayer][2] = copper;
	currentGame.hand[currentPlayer][3] = gold;
	currentGame.hand[currentPlayer][4] = silver;

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//the above hand is copper + silver + copper + gold + silver, or 1 + 2 + 1 + 3 + 2
	if (currentGame.coins == (1 + 2 + 1 + 3 + 2 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}

	printf("\n\nTEST 7: Some Treasure Cards, mix of types, no bonus \n");
	
	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	bonus = 0;

	//make the hand a mix of Treasure and non-Treasure cards
	currentGame.hand[currentPlayer][0] = copper;
	currentGame.hand[currentPlayer][1] = feast;
	currentGame.hand[currentPlayer][2] = mine;
	currentGame.hand[currentPlayer][3] = gold;
	currentGame.hand[currentPlayer][4] = silver;

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//the above hand has copper + silver + gold or 1 + 2 + 3 for the amount of coins
	if (currentGame.coins == (1 + 2 + 3 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}

	printf("\n\nTEST 8: Some Treasure Cards, mix of types, a bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;

	bonus = 3;

	//make the hand a mix of Treasure and non-Treasure cards
	currentGame.hand[currentPlayer][0] = copper;
	currentGame.hand[currentPlayer][1] = feast;
	currentGame.hand[currentPlayer][2] = mine;
	currentGame.hand[currentPlayer][3] = gold;
	currentGame.hand[currentPlayer][4] = silver;

	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//the above hand has copper + silver + gold or 1 + 2 + 3 for the amount of coins
	if (currentGame.coins == (1 + 2 + 3 + bonus)) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}

	printf("\n\nTEST 9: No cards in hand, no bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = 0;

	bonus = 0;

	
	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	if (currentGame.coins == (0 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
		testsFailed++;
	}

	printf("\n\nTEST 10: No cards in hand, a bonus \n");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = 0;

	bonus = 1;


	properReturn = updateCoins(currentPlayer, &currentGame, bonus);

	//the above hand has copper + silver + gold or 1 + 2 + 3 for the amount of coins
	if (currentGame.coins == (0 + bonus) && properReturn == 0) {
		printf( "----TEST PASSED---- THE COINS WERE PROPERLY UPDATED");
	}
	else {
		printf( "----TEST FAILED---- THE COINS WERE NOT PROPERLY UPDATED");
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