#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {

	printf("\nTESTING OUTPOSTEFFECT() CARD EFFECT FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int currentPlayer = 0;
	int k[10] = { adventurer, smithy, feast, gardens, outpost, sea_hag, smithy, village, baron, curse };
	struct gameState currentGame;
	int properReturn;
	int testsFailed = 0;

	int handCount = 5;
	int discardCount = 5;
	int deckCount = 5;

	printf("\nTEST 1: PROPER RETURN OCCURS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard outpost
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = outpost;
		currentGame.deck[currentPlayer][i] = outpost;
		currentGame.discard[currentPlayer][i] = outpost;
	}

	//sets whose turn it is
	currentGame.whoseTurn = 0;

	//call the function
	properReturn = cardEffect(outpost, 0, 0, 0, &currentGame, 0, 0);

	//checks to make sure the function returns properly
	if (properReturn == 0) {
		printf( "\n----TEST PASSED----  THE FUNCTION DID NOT CRASH AND RETURNED PROPERLY");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 2: OUTPOST FLAG IS SET AFTER FUNCTION CALL");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;

	//sets outpostPlayed to 0
	currentGame.outpostPlayed = 0;

	//make all the cards in deck/hand/discard outpost
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = outpost;
		currentGame.deck[currentPlayer][i] = outpost;
		currentGame.discard[currentPlayer][i] = outpost;
	}

	//sets whose turn it is
	currentGame.whoseTurn = currentPlayer;

	//call the function
	properReturn = cardEffect(outpost, 0, 0, 0, &currentGame, 0, 0);

	//checks to make sure the outpost flag is set
	if (currentGame.outpostPlayed == 1) {
		printf( "\n----TEST PASSED----  THE OUTPOST FLAG WAS SET");
	}
	else {
		printf( "\n----TEST FAILED----  THE OUTPOST FLAG WAS NOT SET");
		testsFailed++;
	}

	printf("\n\nTEST 3: CALLING THE FUNCTION DOES NOT CHANGE THE OTHER PLAYER'S CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard different cards
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = outpost;
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

	//calls outpost for first player

	//sets whose turn it is
	currentGame.whoseTurn = 0;

	//call the function
	properReturn = cardEffect(outpost, 0, 0, 0, &currentGame, 0, 0);
	


	//checks to make sure the 2nd player's cards were not alte by the 1st player using the smithy card
	if (properReturn == 0 && currentGame.handCount[currentPlayer] == handCount
		&&	currentGame.discardCount[currentPlayer] == discardCount
		&&	currentGame.deckCount[currentPlayer] == deckCount) {
		printf( "\n----TEST PASSED----  THE OTHER PLAYER'S HAND/DECK/DISCARD WAS NOT CHANGED");
	}
	else {
		printf( "\n----TEST FAILED----  THE OTHER PLAYER'S HAND/DECK/DISCARD WAS CHANGED");
		testsFailed++;
	}

	printf("\n\nTEST 4: CALLING THE FUNCTION DOES NOT CHANGE SUPPLY COUNT OF THE CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard outpost
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = outpost;
		currentGame.deck[currentPlayer][i] = outpost;
		currentGame.discard[currentPlayer][i] = outpost;
	}

	//goes through and makes all the supply counts equal to 4
	for (i = 0; i < treasure_map + 1; i++) {
		currentGame.supplyCount[i] = 4;
	}

	//calls outpostEffect for first player
	//sets whose turn it is
	currentGame.whoseTurn = 0;

	//call the function
	properReturn = cardEffect(outpost, 0, 0, 0, &currentGame, 0, 0);

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

	printf("\n\nTEST 5: CHECK TO MAKE SURE THE OUTPOST CARD IS DISCARDED");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = 1;
	handCount == 1;
	currentGame.discardCount[currentPlayer] = 0;
	discardCount = 0;
	currentGame.deckCount[currentPlayer] = 0;
	deckCount = 0;

	currentGame.hand[currentPlayer][0] = outpost;

	//sets whose turn it is
	currentGame.whoseTurn = 0;

	//call the function
	properReturn = cardEffect(outpost, 0, 0, 0, &currentGame, 0, 0);

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