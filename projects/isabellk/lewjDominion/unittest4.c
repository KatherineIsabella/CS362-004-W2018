#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


int main() {

	printf("\n\nTESTING FULLDECKCOUNT() FUNCTION\n");

	//Citation: used the updateCoins demonstration unittest file as a template for setting up the game to test it
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int currentPlayer = 1;
	int k[10] = { adventurer, outpost, feast, gardens, mine, sea_hag, smithy, village, baron, curse };
	struct gameState currentGame;
	int handCount = 5;
	int discardCount = 5;
	int deckCount = 5;
	int testsFailed = 0;
	int count = 0;

	printf("\nTEST 1: ALL CARDS IN DISCARD, DECK, AND HAND ARE THE CARD WE ARE LOOKING FOR");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard curse
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = curse;
		currentGame.deck[currentPlayer][i] = curse;
		currentGame.discard[currentPlayer][i] = curse;
	}

	//gets the count of the times curse is in the player's hand/deck/discard
	count = fullDeckCount(currentPlayer, curse, &currentGame);

	//each pile has ALL cards as curse and all are the same size so handCount * 3 should be what count returns
	if (count == (handCount * 3)) {
		printf( "\n----TEST PASSED----  THE CARDS WERE PROPERLY COUNTED");
	}
	else {
		printf( "\n----TEST FAILED----  THE CARDS WERE NOT PROPERLY COUNTED");
		testsFailed++;
	}

	printf("\n\nTEST 2: NO CARDS IN DISCARD, DECK, AND HAND ARE THE CARD WE ARE LOOKING FOR");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;


	//make all the cards in deck/hand/discard curse
	for (i = 0; i < handCount; i++) {
		currentGame.hand[currentPlayer][i] = curse;
		currentGame.deck[currentPlayer][i] = curse;
		currentGame.discard[currentPlayer][i] = curse;
	}

	//calls the fullDeckCount function to get the coutn of adventurer
	count = fullDeckCount(currentPlayer, adventurer, &currentGame);


	//No adventurer cards are in the hand/deck/discard so count should return 0
	if (count == 0) {
		printf( "\n----TEST PASSED----  THE CARDS WERE PROPERLY COUNTED");
	}
	else {
		printf( "\n----TEST FAILED----  THE CARDS WERE NOT PROPERLY COUNTED");
		testsFailed++;
	}


	printf("\n\nTEST 3: PLAYER HAS NO CARDS IN HAND, DECK, OR DISCARD");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = 0;
	currentGame.deckCount[currentPlayer] = 0;
	currentGame.discardCount[currentPlayer] = 0;

	count = fullDeckCount(currentPlayer, adventurer, &currentGame);


	if (count == 0) {
		printf( "\n----TEST PASSED----  THE CARDS WERE PROPERLY COUNTED");
	}
	else {
		printf( "\n----TEST FAILED----  THE CARDS WERE NOT PROPERLY COUNTED");
		testsFailed++;
	}

	printf("\n\nTEST 4: MIX OF CARD IN HAND/DECK/DISCARD");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	currentGame.handCount[currentPlayer] = handCount;
	currentGame.discardCount[currentPlayer] = discardCount;
	currentGame.deckCount[currentPlayer] = deckCount;

	//sets up what cards are in the player's hand/deck/discard
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;
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

	//calls the fullDeckCount function to get the count of feast
	count = fullDeckCount(currentPlayer, feast, &currentGame);

	//looking for feast and we gave the player 3 feast cards spread out in hand
	if (count == 3) {
		printf( "\n----TEST PASSED----  THE CARDS WERE PROPERLY COUNTED");
	}
	else {
		printf( "\n----TEST FAILED----  THE CARDS WERE NOT PROPERLY COUNTED");
		testsFailed++;
	}

	printf("\n\nTEST 5: CHECK TO MAKE SURE THE FUNCTION DOES NOT CHANGE THE CARDS");

	memset(&currentGame, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &currentGame);
	
	//makes the hand/deck/discard smaller for simpler checks
	currentGame.handCount[currentPlayer] = 2;
	currentGame.deckCount[currentPlayer] = 2;
	currentGame.discardCount[currentPlayer] = 2;
	
	//sets up what cards are in the player's hand/deck/discard
	currentGame.hand[currentPlayer][0] = feast;
	currentGame.hand[currentPlayer][1] = mine;

	currentGame.deck[currentPlayer][0] = copper;
	currentGame.deck[currentPlayer][1] = silver;


	currentGame.discard[currentPlayer][0] = mine;
	currentGame.discard[currentPlayer][1] = feast;

	//calls the fullDeckCount function
	count = fullDeckCount(currentPlayer, feast, &currentGame);

	int cardsSame = 0;

	if (currentGame.hand[currentPlayer][0] == feast && currentGame.hand[currentPlayer][1] == mine
		&& currentGame.deck[currentPlayer][0] == copper && currentGame.deck[currentPlayer][1] == silver
		&&	currentGame.discard[currentPlayer][0] == mine && currentGame.discard[currentPlayer][1] == feast) {
		cardsSame = 0;
	}
	else {
		cardsSame = 1;
	}

	//looking for feast and we gave the player 2 feast cards spread out in hand. Also making sure the cards are the same after the function is called
	if (count == 2 && cardsSame == 0) {
		printf( "\n----TEST PASSED----  THE CARDS WERE PROPERLY COUNTED");
	}
	else {
		printf( "\n----TEST FAILED----  THE CARDS WERE NOT PROPERLY COUNTED");
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