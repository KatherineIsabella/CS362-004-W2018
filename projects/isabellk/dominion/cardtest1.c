#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


int main() {

	printf("\n\nTESTING SMITHYEFFECT() CARD EFFECT FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int currentPlayer = 0;
	int k[10] = { adventurer, smithy, feast, gardens, mine, sea_hag, smithy, village, baron, curse };
	struct gameState currentGame;
	int properReturn = -1;
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


	//make all the cards in deck/hand/discard smithy
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = smithy;
		currentGame.deck[currentPlayer][i] = smithy;
		currentGame.discard[currentPlayer][i] = smithy;
	}
	i = 0;

	properReturn = smithyEffect(currentPlayer, &currentGame, 0, i);

	//checks to make sure the function returns properly
	if (properReturn == 0) {
		printf( "\n----TEST PASSED----  THE FUNCTION DID NOT CRASH AND RETURNED PROPERLY");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
		testsFailed++;
	}

	printf("\n\nTEST 2: THREE CARDS ARE DRAWN FROM THE PLAYER'S HAND");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//sets up what cards are in the player's hand/deck/discard
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = smithy;
	currentGame.hand[currentPlayer][2] = village;
	currentGame.hand[currentPlayer][3] = curse;
	currentGame.hand[currentPlayer][4] = feast;

	currentGame.deck[currentPlayer][0] = copper;
	currentGame.deck[currentPlayer][1] = silver;
	currentGame.deck[currentPlayer][2] = village;
	currentGame.deck[currentPlayer][3] = mine;
	currentGame.deck[currentPlayer][4] = curse;


	currentGame.discard[currentPlayer][0] = mine;
	currentGame.discard[currentPlayer][1] = feast;
	currentGame.discard[currentPlayer][2] = gold;
	currentGame.discard[currentPlayer][3] = treasure_map;
	currentGame.discard[currentPlayer][4] = sea_hag;

	//calls smithyEffect
	properReturn = smithyEffect(currentPlayer, &currentGame, 1, 0);


	//checks to make sure 3 cards are moved from the deck to the hand - handCount only goes up by 2 because smithy is moved to the playedCards pile
	if (properReturn == 0 && (currentGame.deckCount[currentPlayer] == deckCount - 3) && (currentGame.handCount[currentPlayer] == handCount + 2)) {
		printf( "\n----TEST PASSED----  THE PLAYER GAINED THREE CARDS FROM THEIR DECK AND SMITHY MOVED TO THE PLAYEDCARDS PILE");
	}
	else {
		printf( "\n----TEST FAILED----  THE PLAYER DID NOT GAIN THREE CARDS");
		testsFailed++;
	}


	printf("\n\nTEST 3: CALLING THE FUNCTION DOES NOT CHANGE THE OTHER PLAYER'S CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;

	
	//make all the cards in deck/hand/discard smithy
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = smithy;
		currentGame.deck[currentPlayer][i] = smithy;
		currentGame.discard[currentPlayer][i] = smithy;
	}

	//make next player's hand/discard/deck piles
	currentPlayer = 1;
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;

	//make all the cards in deck/hand/discard adventurer
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = adventurer;
		currentGame.deck[currentPlayer][i] = adventurer;
		currentGame.discard[currentPlayer][i] = adventurer;
	}

	//calls smithyEffect for first player
	properReturn = smithyEffect(0, &currentGame, 1, 0);


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


	//make all the cards in deck/hand/discard smithy
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = smithy;
		currentGame.deck[currentPlayer][i] = smithy;
		currentGame.discard[currentPlayer][i] = smithy;
	}

	//goes through and makes all the supply counts equal to 4
	for (i = 0; i < treasure_map + 1; i++) {
		currentGame.supplyCount[i] = 4;
	}

	//calls smithyEffect for first player
	properReturn = smithyEffect(currentPlayer, &currentGame, 1, 0);

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

	printf("\n\nTEST 5: CHECK TO MAKE SURE THE GAME DOES NOT CRASH IF THE PLAYER ONLY HAS THE SMITHY CARD");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);

	currentPlayer = 0;
	currentGame.handCount[currentPlayer] = 1;
	currentGame.discardCount[currentPlayer] = 0;
	currentGame.deckCount[currentPlayer] = 0;

	currentGame.hand[currentPlayer][0] = smithy;

	//calls smithyEffect for first player
	properReturn = smithyEffect(currentPlayer, &currentGame, 0, 0);


	//checks to make sure the game does not crash if the only card the player has is the smithy card
	if (properReturn == 0) {
		printf( "\n----TEST PASSED----  THE FUNCTION RETURNED PROPERLY");
	}
	else {
		printf( "\n----TEST FAILED----  THE FUNCTION DID NOT RETURN PROPERLY");
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