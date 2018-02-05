#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


int main() {

	printf("\nTESTING VILLAGEEFFECT() CARD EFFECT FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int currentPlayer = 0;
	int k[10] = { adventurer, smithy, feast, gardens, mine, sea_hag, smithy, village, baron, curse };
	struct gameState currentGame;
	int properReturn;
	int testsFailed = 0;

	int handCount = 5;
	int discardCount = 5;
	int deckCount = 5;

	printf("\n\nTEST 1: PROPER RETURN OCCURS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard village
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = village;
		currentGame.deck[currentPlayer][i] = village;
		currentGame.discard[currentPlayer][i] = village;
	}

	properReturn = villageEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly
	if (properReturn == 0) {
		printf( "\n----TEST PASSED----  THE FUNCTION DID NOT CRASH AND RETURNED PROPERLY");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 2: +TWO ACTIONS ARE GIVEN AFTER CARD USED");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//set the number of actions to 0
	currentGame.numActions = 0;

	//make all the cards in deck/hand/discard village
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = village;
		currentGame.deck[currentPlayer][i] = village;
		currentGame.discard[currentPlayer][i] = village;
	}

	properReturn = villageEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly
	if (currentGame.numActions == 2) {
		printf( "\n----TEST PASSED----  TWO ADDITIONAL ACTIONS WERE GAINED");
	}
	else {
		printf( "\n----TEST FAILED----  TWO ADDITIONAL ACTIONS WERE NOT GAINED");
		testsFailed++;
	}

	printf("\n\nTEST 3: ONE CARD IS DRAWN");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//set the number of actions to 0
	currentGame.numActions = 0;

	//make all the cards in deck/hand/discard village
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = village;
		currentGame.deck[currentPlayer][i] = copper;
		currentGame.discard[currentPlayer][i] = gold;
	}

	properReturn = villageEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the hand gained a card (but stayed the same because the village card is discarded) and the deck lost a card
	if (currentGame.handCount[currentPlayer] == handCount && currentGame.deckCount[currentPlayer] == deckCount - 1) {
		printf( "\n----TEST PASSED----  ONE CARD WAS DRAWN");
	}
	else {
		printf( "\n----TEST FAILED----  ONE CARD WAS NOT DRAWN");
		testsFailed++;
	}

	printf("\n\nTEST 4: CALLING THE FUNCTION DOES NOT CHANGE THE OTHER PLAYER'S CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard different cards
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = village;
		currentGame.deck[currentPlayer][i] = gold;
		currentGame.discard[currentPlayer][i] = adventurer;
	}

	//make next player's hand/discard/deck piles
	currentPlayer = 1;
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;

	//make all the cards in deck/hand/discard different cards
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = gold;
		currentGame.deck[currentPlayer][i] = copper;
		currentGame.discard[currentPlayer][i] = smithy;
	}

	//calls smithyEffect for first player
	properReturn = villageEffect(0, &currentGame, 0);


	//checks to make sure the 2nd player's cards were not alte by the 1st player using the village card
	if (properReturn == 0 && currentGame.handCount[currentPlayer] == handCount
		&&	currentGame.discardCount[currentPlayer] == discardCount
		&&	currentGame.deckCount[currentPlayer] == deckCount) {
		printf( "\n----TEST PASSED----  THE OTHER PLAYER'S HAND/DECK/DISCARD WAS NOT CHANGED");
	}
	else {
		printf( "\n----TEST FAILED----  THE OTHER PLAYER'S HAND/DECK/DISCARD WAS CHANGED");
		testsFailed++;
	}

	printf("\n\nTEST 5: CALLING THE FUNCTION DOES NOT CHANGE SUPPLY COUNT OF THE CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard village
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = village;
		currentGame.deck[currentPlayer][i] = village;
		currentGame.discard[currentPlayer][i] = village;
	}

	//goes through and makes all the supply counts equal to 4
	for (i = 0; i < treasure_map + 1; i++) {
		currentGame.supplyCount[i] = 4;
	}

	//calls villageEffect for first player
	properReturn = villageEffect(currentPlayer, &currentGame, 0);

	int supplyCountSame = 0;

	//goes through and checks to make sure all the supply counts are still the same
	for (i = 0; i < treasure_map + 1; i++) {
		if (currentGame.supplyCount[i] != 4) {
			supplyCountSame = 1;
		}
	}

	//checks to make sure the supplyCounts did not change
	if (supplyCountSame == 0) {
		printf( "\n----TEST PASSED----  THE SUPPLY COUNT DID NOT CHANGE");
	}
	else {
		printf( "\n----TEST FAILED----  THE SUPPL COUNT CHANGED");
		testsFailed++;
	}

	printf("\n\nTEST 6: CHECK TO MAKE SURE THE FUNCTION WORKS PROPERLY IF VILLAGE IS THE ONLY CARD THE PLAYER HAS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = 1;
	handCount == 1;
	currentGame.discardCount[currentPlayer] = 0;
	discardCount = 0;
	currentGame.deckCount[currentPlayer] = 0;
	deckCount = 0;

	currentGame.hand[currentPlayer][0] = village;
		

	//set the number of actions to 0
	currentGame.numActions = 0;


	properReturn = villageEffect(0, &currentGame, 0);

	//checks to make sure the function returns properly with the correct actions if there is only 1 card the player has
	if (properReturn == 0 && currentGame.numActions == 2) {
		printf( "\n----TEST PASSED----  THE FUNCTION RETURNED PROPERLY");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}


	printf("\n\nTEST 7: CHECK TO MAKE SURE THE VILLAGE CARD IS DISCARDED");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = 1;
	handCount == 1;
	currentGame.discardCount[currentPlayer] = 0;
	discardCount = 0;
	currentGame.deckCount[currentPlayer] = 0;
	deckCount = 0;

	currentGame.hand[currentPlayer][0] = village;

	properReturn = villageEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the card is discarded after used
	if (properReturn == 0 && currentGame.handCount[currentPlayer] == handCount - 1 && currentGame.discardCount[currentPlayer] == discardCount + 1) {
		printf( "\n----TEST PASSED----  THE CARD WAS PROPERLY DISCARDED");
	}
	else {
		printf( "\n----TEST FAILED----  THE CARD WAS NOT PROPERLY DISCARDED");
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