#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {

	printf("\n\nTESTING ADVENTUREREFFECT() CARD EFFECT FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int currentPlayer = 0;
	int k[10] = { adventurer, smithy, feast, gardens, mine, sea_hag, smithy, village, baron, curse };
	struct gameState currentGame;
	int properReturn;
	int testsFailed = 0;
	int temphand[MAX_HAND];

	int handCount = 5;
	int discardCount = 5;
	int deckCount = 5;

	printf("\nTEST 1: NO TREAUSRE IN HAND/DECK/DISCARD");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard adventurer
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = adventurer;
		currentGame.discard[currentPlayer][i] = adventurer;
	}

	properReturn = adventurerCardEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly and as there is no treasure, all cards in deck should be discarded 
	// (and all card in discard should still be in discard so as we started with 5 in each, we should have 10 at the end)
	if (properReturn == 0 && currentGame.discardCount[currentPlayer] == 10 && currentGame.deckCount[currentPlayer] == 0) {
		printf( "\n----TEST PASSED----  THE FUNCTION DID NOT CRASH AND RETURNED PROPERLY");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 2: ONE TREASURE IN DECK");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard adventurer
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = adventurer;
		currentGame.discard[currentPlayer][i] = adventurer;
	}
	
	currentGame.deck[currentPlayer][1] = copper;

	properReturn = adventurerCardEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly and as there is only 1 treasure. The treasure should be moved to the hand (so 6 cards there) and 
	//the rest of the deck moved to discard (as we had to reveal them all to find there was only 1 treasure) should then be 9
	if (properReturn == 0 && currentGame.discardCount[currentPlayer] == discardCount + deckCount - 1 && currentGame.deckCount[currentPlayer] == 0 
		&& currentGame.handCount[currentPlayer] == handCount + 1) {
		printf( "\n----TEST PASSED----  ONE TREASURE FOUND AND PROPERLY ADDED");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}


	printf("\n\nTEST 3: TWO TREASURE IN DECK - FIRST TWO CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard adventurer
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = adventurer;
		currentGame.discard[currentPlayer][i] = adventurer;
	}

	currentGame.deck[currentPlayer][3] = copper;
	currentGame.deck[currentPlayer][4] = copper;


	properReturn = adventurerCardEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly and as there is 2treasure. The treasure should be moved to the hand (so 7 cards there) and 
	//the no cards from the deck should be moved to discard as the treasure cards were the first two in the deck
	if (properReturn == 0 && currentGame.discardCount[currentPlayer] == discardCount && currentGame.deckCount[currentPlayer] == deckCount - 2
		&& currentGame.handCount[currentPlayer] == handCount + 2) {
		printf( "\n----TEST PASSED----  TWO TREASURES FOUND AND PROPERLY ADDED");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 4: THREE TREASURES IN DECK");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard adventurer
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = adventurer;
		currentGame.discard[currentPlayer][i] = adventurer;
	}

	//give the deck 3 treasures
	currentGame.deck[currentPlayer][4] = copper;
	currentGame.deck[currentPlayer][3] = silver;
	currentGame.deck[currentPlayer][2] = copper;



	properReturn = adventurerCardEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly when there are 3 treasures in the hand. 
	//The hand should only have gained 2 treasures and the deck only lost 2
	if (properReturn == 0 && currentGame.discardCount[currentPlayer] == discardCount && currentGame.deckCount[currentPlayer] == deckCount - 2
		&& currentGame.handCount[currentPlayer] == handCount + 2) {
		printf( "\n----TEST PASSED----  TWO TREASURES FOUND AND PROPERLY ADDED");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 5: TREASURES ONLY IN THE DISCARD PILE  \n");


	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand adventurer and all in discard a treasure
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = adventurer;
		currentGame.discard[currentPlayer][i] = gold;
	}


	properReturn = adventurerCardEffect(currentPlayer, &currentGame, 0);

	//checks to make sure the function returns properly when the treasures are only in the discard
	if (properReturn == 0 && currentGame.discardCount[currentPlayer] == discardCount + deckCount - 2 && currentGame.deckCount[currentPlayer] == 0
		&& currentGame.handCount[currentPlayer] == handCount + 2) {
		printf( "\n----TEST PASSED----  TWO TREASURES FOUND AND PROPERLY ADDED");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 6: CALLING THE FUNCTION DOES NOT CHANGE THE SUPPLY COUNT");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard smithy
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = gold;
		currentGame.discard[currentPlayer][i] = silver;
	}

	//goes through and makes all the supply counts equal to 4
	for (i = 0; i < treasure_map + 1; i++) {
		currentGame.supplyCount[i] = 4;
	}



	//calls adventurerEffect for first player
	properReturn = adventurerCardEffect(currentPlayer, &currentGame, 0);

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

	//alets user how many (if any) tests failed
	if (testsFailed == 0) {
		printf("\nALL TESTS PASSED\n");
	}
	else {
		printf("\nNOT ALL TESTS PASSED. NUMBER OF FAILS: %d\n", testsFailed);
	}

	return 0;
}