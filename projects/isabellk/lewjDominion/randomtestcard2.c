#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"


//variables to keep track how many times each issue fails (gives a breakdown
//so the tester will get more info then "x tests failed")
struct failureBreakdown {
	int didNotReturnZero;
	int didNotAddTwoActions;
	int didNotDrawOneCard;
	int gameStateChanged;
	int villageCardNotDiscarded;
	int failedTest;
};


//declare function
void villageTests(int currentPlayer, struct gameState *currentGameState, struct failureBreakdown* failures, int villagePos);

int main() {

	int maxTests = 10000;
	struct failureBreakdown failures;

	//set all the failures to 0 to start off with
	failures.didNotAddTwoActions = 0;
	failures.didNotDrawOneCard = 0;
	failures.didNotReturnZero = 0;
	failures.failedTest = 0;
	failures.gameStateChanged = 0;
	failures.villageCardNotDiscarded = 0;

	printf("\nTESTING VILLAGEEFFECT() CARD EFFECT FUNCTION\n");


	int k[10] = { village, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };

	//creating a potential hand with the first 7 being "real" values each game should have. The kingdom cards will later be overwritten with
	//the ones randomly chosen, but the treasure/victory/curse card should all have the potential to be in the hand and the  kingdom cards
	//NOT initialized in the game should not be in the hand
	int potentialHand[17] = { curse, estate, duchy, province, copper, silver, gold, village, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };


	//set up Random()
	//Citation: used betterTestDrawCard and read through rngs.c to figure out how to use rngs.h functionality
	SelectStream(2);
	PutSeed(5);

	struct gameState currentGame;

	int i = 0;

	//go through  10,000 tests of village card with the base input randomly generated
	while (i < maxTests) {

		//starting with the 2nd card in the kingdom card array as we are testing the village card so there should be a village card in game...
		int j = 1;
		while (j < 10) {

			//randomly generate a kingdom card - the treasure_map - adventurer and then 
			//adding back in adventurer is to make sure the card isn't a treasure or victory card or curse
			int cardPick = floor(Random() * (treasure_map - adventurer + 1));
			cardPick += adventurer;

			k[j] = cardPick;
			int goodCard = 0;

			//make sure we picked a good kingdom card - not a duplicate
			while (goodCard < j) {
				if (k[goodCard] != k[j]) {
					j++;
				}
				goodCard++;
			}
		}

		//now we need to update the potential hand array with our above kingdom cards
		j = adventurer + 1;
		int kingdomCount = 1;
		while (j < 17) {
			potentialHand[j] = k[kingdomCount];
			j++;
			kingdomCount++;
		}

		//randomly generate the seed
		int seed = floor(Random() * 1000);

		//generate a random amount of players. The game must have between 2-4 players so we only want to test those values
		int numPlayers = floor(Random() * 3);
		numPlayers += 2;

		//randomly generate the currentPlayer based on the number of players
		int currentPlayer = floor(Random() * numPlayers);


		// clear the game state and initialize the game with the random inputs
		memset(&currentGame, 23, sizeof(struct gameState));
		initializeGame(numPlayers, k, seed, &currentGame);



		//generate counts for the deck, discard, and hand of each of the players
		//CITATION: used method shown in lectures and betterTestDrawCard
		currentGame.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
		currentGame.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
		currentGame.handCount[currentPlayer] = floor(Random() * MAX_HAND);


		//make sure handcount is at least 1. For the village card to be played, 
		//the player actually needs to have the card in their hand...
		if (currentGame.handCount[currentPlayer] < MAX_HAND-1) {
			currentGame.handCount[currentPlayer] += 1;
		}

		//assign the currentPlayer a random assortment of *allowed* card types in their deck
		j = 0;
		while (j < currentGame.deckCount[currentPlayer]) {

			//using 17 here because that is 10 kingdom card + 3 treasure  + 3 victory  + 1 curse
			int card = floor(Random() * 17);
			currentGame.deck[currentPlayer][j] = potentialHand[card];
			j++;
		}
		j = 0;


		int villagePos = -1;
		
		//assign the currentPlayer a random assortment of *allowed* card types in their hand
		while (j < currentGame.handCount[currentPlayer]) {

			//using 17 here because that is 10 kingdom card + 3 treasure  + 3 victory  + 1 curse
			int card = floor(Random() * 17);
			currentGame.hand[currentPlayer][j] = potentialHand[card];


			//get the position of the village card
			if (potentialHand[card] == village && villagePos == -1) {
				villagePos = j;
			}
			j++;
		}

		//if the village card is not in the hand, we need to put it in
		if (villagePos == -1) {
			int pos = floor(Random() * (currentGame.handCount[currentPlayer]));
			currentGame.hand[currentPlayer][pos] = village;
			villagePos = pos;
		}


		j = 0;

		//assign the currentPlayer a random assortment of *allowed* card types in their discard
		while (j < currentGame.discardCount[currentPlayer]) {

			//using 17 here because that is 10 kingdom card + 3 treasure  + 3 victory  + 1 curse
			int card = floor(Random() * 17);
			currentGame.discard[currentPlayer][j] = potentialHand[card];
			j++;
		}

		//call the villageTests with our random data
		villageTests(currentPlayer, &currentGame, &failures, villagePos);
		i++;
	}

	//print out our results
	printf("Number of tests out of %d that failed: %d\nBreakdown:\n", maxTests, failures.failedTest);
	printf("Did not return properly: %d\nDid not add two actions: %d\nDid not draw a card: %d\nDid not discard the card: %d\nChanged the game state: %d\n",
		failures.didNotReturnZero, failures.didNotAddTwoActions, failures.didNotDrawOneCard, failures.villageCardNotDiscarded, failures.gameStateChanged);


	return 0;
}

void villageTests(int currentPlayer, struct gameState *currentGameState, struct failureBreakdown* failures, int villagePos) {
	
	int properReturn = 0;


	int testFailed = 0;

	struct gameState beforeFunctionState;

	//copy the gamestate into beforeFunctionState so we have a reference to use after we call our function
	memcpy(&beforeFunctionState, currentGameState, sizeof(struct gameState));

	//call the function
	properReturn = villageCardEffect(currentPlayer, currentGameState, villagePos);

	if (properReturn != 0) {
		testFailed = 1;
		failures->didNotReturnZero++;
	}


	//add two actions to the before game state as the village card does this
	beforeFunctionState.numActions += 2;

	//checks to make sure that the numActions are the same so we know the villageEffect increased action by 2
	if (beforeFunctionState.numActions != currentGameState->numActions) {
		testFailed = 1;
		failures->didNotAddTwoActions++;
	}



	//if deck count is >0 we draw from the deck
	//Note: we know handcount cannot be 0 because the player must at least have the village card.
	if (beforeFunctionState.deckCount[currentPlayer] > 0) {
		//changebefore function state to make sure the hand gained a card (but stayed the same because 
		//the village card is discarded) and the deck lost a card and the discard gained a card
		beforeFunctionState.deckCount[currentPlayer]--;
		beforeFunctionState.discardCount[currentPlayer]++;

		//if the discards are not the same, the village card was not discared
		if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
			failures->villageCardNotDiscarded++;
			testFailed = 1;
		}

		//check to make sure hand stayed same and deck properly lost a card as a card was moved from deck to hand
		if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]
			&& beforeFunctionState.deckCount[currentPlayer] != currentGameState->deckCount[currentPlayer]) {
			failures->didNotDrawOneCard++;
			testFailed = 1;
		}
	}
	//if the deckCount is 0, and the discardCount is not 0, then we get the card from the discardCount with a shuffle
	else if (beforeFunctionState.discardCount[currentPlayer] > 0) {

		//hand should stay the same as it gains 1 from discard and discards the village

		//move the discardCount into the deck and remove the 1 card adventurer took
		beforeFunctionState.deckCount[currentPlayer] = beforeFunctionState.discardCount[currentPlayer] - 1;

		//discard pile should now only have the village card in it
		beforeFunctionState.discardCount[currentPlayer] = 1;

		//if the discards are not the same, the village card was not discared
		if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
			failures->villageCardNotDiscarded++;
			testFailed = 1;
		}

		//check to make sure hand stayed same and deck properly gained the discard and lost a card as a card was moved from deck to hand
		if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]
			&& beforeFunctionState.deckCount[currentPlayer] != currentGameState->deckCount[currentPlayer]) {
			failures->didNotDrawOneCard++;
			testFailed = 1;
		}
	}

	//if both discard and deck are empty, the only thing that should happen is that the hand loses its card to discard
	else {

		//hand loses its card and discard gains a card
		beforeFunctionState.handCount[currentPlayer]--;
		beforeFunctionState.discardCount[currentPlayer]++;
		//if the discards are not the same, the village card was not discared and the deck didn't change
		if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]
			&& beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]
			&& beforeFunctionState.deckCount[currentPlayer] != currentGameState->deckCount[currentPlayer]) {
			failures->villageCardNotDiscarded++;
			testFailed = 1;
		}
	}

	//now we need to make sure calling the function didn't change anything in the game state...
	if (memcmp(&beforeFunctionState, currentGameState, sizeof(struct gameState)) != 0) {
		testFailed = 1;
		failures->gameStateChanged++;
	}

	//if any of the above tests failed, then failedTest goes up by 1
	if (testFailed == 1) {
		failures->failedTest++;
	}
}

