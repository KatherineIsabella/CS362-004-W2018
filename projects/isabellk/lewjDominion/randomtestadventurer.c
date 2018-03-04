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
	int didNotAddTreasureToHand;
	int didNotReturnTreasureFromDeck;
	int didNotReturnTreasureFromDiscard;
	int didNotReturnProperly;
	int gameStateChanged;
	int didNotDiscardProperly;
	int failedTest;
};

//declare function
void adventurerTests(int currentPlayer, struct gameState *currentGameState, int tempHandSize,
	struct failureBreakdown* failures, int firstTreasureDeckPos, int firstTreasureDiscardPos,
	int secondTreasureDeckPos, int secondTreasureDiscardPos);

int main() {

	int maxTests = 10000;
	struct failureBreakdown failures;


	//set all the failures to 0 to start off with
	failures.didNotReturnTreasureFromDeck = 0;
	failures.didNotAddTreasureToHand = 0;
	failures.didNotReturnTreasureFromDiscard = 0;
	failures.failedTest = 0;
	failures.didNotReturnProperly = 0;
	failures.gameStateChanged = 0;
	failures.didNotDiscardProperly = 0;


	printf("\nTESTING ADVENTUREEFFECT() CARD EFFECT FUNCTION\n");


	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };

	//creating a potential hand with the first 7 being "real" values each game should have. The kingdom cards will later be overwritten with
	//the ones randomly chosen, but the treasure/victory/curse card should all have the potential to be in the hand and the  kingdom cards
	//NOT initialized in the game should not be in the hand
	int potentialHand[17] = { curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };


	//set up Random()
	//Citation: used betterTestDrawCard and read through rngs.c to figure out how to use rngs.h functionality
	SelectStream(2);
	PutSeed(3);


	struct gameState currentGame;

	int i = 0;

	//go through  10,000 tests of adventurer card with the base input randomly generated
	while (i < maxTests) {

		//starting with the 2nd card in the kingdom card array as we are testing the adventurer card so there should be a adventurer card in game
		//for the function to be called...
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


		//make sure handcount is at least 1. For the adventurer card to be played, 
		//the player actually needs to have the card in their hand...
		if (currentGame.handCount[currentPlayer] < MAX_HAND -1) {
			currentGame.handCount[currentPlayer] += 1;
		}


		//for adventurer we need to know where the treasures are
		int firstTreasureDeckPos = -1;
		int secondTreasureDeckPos = -1;
		int firstTreasureDiscardPos = -1;
		int secondTreasureDiscardPos = -1;

		//assign the currentPlayer a random assortment of *allowed* card types in their deck
		j = 0;
		while (j < currentGame.deckCount[currentPlayer]) {

			//using 17 here because that is 10 kingdom card + 3 treasure  + 3 victory  + 1 curse
			int card = floor(Random() * 17);
			currentGame.deck[currentPlayer][j] = potentialHand[card];

			//get the 1st and 2nd position of the treasure card in the deck
			if ((potentialHand[card] == copper || potentialHand[card] == silver 
				|| potentialHand[card] == gold) && firstTreasureDeckPos == -1) {
				firstTreasureDeckPos = j;
			}
			else if ((potentialHand[card] == copper || potentialHand[card] == silver
				|| potentialHand[card] == gold) && (firstTreasureDeckPos >= 0 && secondTreasureDeckPos == -1)) {
				secondTreasureDeckPos = j;
			}
		j++;

		}

		j = 0;

		//assign the currentPlayer a random assortment of *allowed* card types in their hand
		while (j < currentGame.handCount[currentPlayer]) {

			//using 17 here because that is 10 kingdom card + 3 treasure  + 3 victory  + 1 curse
			int card = floor(Random() * 17);
			currentGame.hand[currentPlayer][j] = potentialHand[card];
			j++;
		}
		j = 0;

		//assign the currentPlayer a random assortment of *allowed* card types in their discard
		while (j < currentGame.discardCount[currentPlayer]) {

			//using 17 here because that is 10 kingdom card + 3 treasure  + 3 victory  + 1 curse
			int card = floor(Random() * 17);
			currentGame.discard[currentPlayer][j] = potentialHand[card];

			//get the 1st and 2nd position of the treasure card in the discard
			if ((potentialHand[card] == copper || potentialHand[card] == silver
				|| potentialHand[card] == gold) && (firstTreasureDiscardPos == -1)) {
				firstTreasureDiscardPos = j;
			}
			else if ((potentialHand[card] == copper || potentialHand[card] == silver
				|| potentialHand[card] == gold) && (firstTreasureDiscardPos >= 0 && secondTreasureDiscardPos == -1)) {
				secondTreasureDiscardPos = j;
			}
			j++;
		}


		//tempHand needs to be big enough for a potential reshuffle and for there to be 0 treasures
		//but...shouldn't be bigger than max hand
		int tempHandSize = currentGame.discardCount[currentPlayer] + currentGame.handCount[currentPlayer] + currentGame.deckCount[currentPlayer];
		if (tempHandSize > MAX_HAND) {
			tempHandSize = MAX_HAND-1;
		}

		//call the adventurerTests with our random data
		adventurerTests(currentPlayer, &currentGame, tempHandSize, &failures,
			firstTreasureDeckPos, firstTreasureDiscardPos, secondTreasureDeckPos, secondTreasureDiscardPos);
		i++;
	}

	//print out our results
	printf("Number of tests out of %d that failed: %d\nBreakdown:\n", maxTests, failures.failedTest);
	printf("Did not return properly: %d\nDid not get deck treasure: %d\nDid not get discard treasure: %d\n",
		failures.didNotReturnProperly, failures.didNotReturnTreasureFromDeck, failures.didNotReturnTreasureFromDiscard);
	printf("Did not put treasure in hand: %d\nDid not discard: %d\nDid not keep the game state the same: %d\n",
		failures.didNotAddTreasureToHand, failures.didNotDiscardProperly, failures.gameStateChanged);

	return 0;
}

void adventurerTests(int currentPlayer, struct gameState *currentGameState, int tempHandSize,
	struct failureBreakdown* failures, int firstTreasureDeckPos, int firstTreasureDiscardPos,
	int secondTreasureDeckPos, int secondTreasureDiscardPos) {

	int properReturn = 0;
	int tempHand[tempHandSize];
	int testFailed = 0;

	struct gameState beforeFunctionState;

	//copy the gamestate into beforeFunctionState so we have a reference to use after we call our function
	memcpy(&beforeFunctionState, currentGameState, sizeof(struct gameState));

	//call the function
	properReturn = adventurerCardEffect(currentPlayer, currentGameState, 0);

	//check to make sure it returned properly
	if (properReturn != 0) {
		testFailed = 1;
		failures->didNotReturnProperly++;
	}

	//if there are two treasures in the deck, then 2 treasures should be added to the hand
	//but we only add 1 card as adventurer should be discarded
	if (firstTreasureDeckPos >= 0 && secondTreasureDeckPos >= 0) {
		beforeFunctionState.handCount[currentPlayer] += 2;
		beforeFunctionState.handCount[currentPlayer]--;

		//the deck should lose as many cards as needed to be flipped to find the treasure cards
		//+1 b/c array starts at 0
		beforeFunctionState.deckCount[currentPlayer] -= secondTreasureDeckPos + 1;

		//discard should gain the flipped deck cards (but not the treasure cards) and the adventurer discard
		//so we have secondTreasureDeckPos + 1 ( the flipped cards) - 2 ( the treasure cards) 
		//and +1 (the adventure card) which cancels out to secondTreasureDeckPos
		beforeFunctionState.discardCount[currentPlayer] += secondTreasureDeckPos;

		if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]) {
			testFailed = 1;
			failures->didNotAddTreasureToHand++;
		}
		if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]){
			testFailed = 1;
			failures->didNotDiscardProperly++;
		}
		if (beforeFunctionState.deckCount[currentPlayer] != currentGameState->deckCount[currentPlayer]) {
			testFailed = 1;
			failures->didNotReturnTreasureFromDeck++;
		}
		
		//only 1 treasure in deck and 1 in discard
		else if (firstTreasureDeckPos >= 0 && secondTreasureDeckPos == -1 && firstTreasureDiscardPos >= 0) {
			//if there are two treasures in the deck and deck combo, then 2 treasures should be added to the hand
			//but we only add 1 card as adventurer should be discarded

			beforeFunctionState.handCount[currentPlayer] += 2;
			beforeFunctionState.handCount[currentPlayer]--;

			int tempDeckCount = beforeFunctionState.deckCount[currentPlayer];

			//the deck should have been reshuffled so should now include none of its original cards and all the discard cards up to the 2nd treasure
			beforeFunctionState.deckCount[currentPlayer] = beforeFunctionState.discardCount[currentPlayer] - firstTreasureDiscardPos + 1;

			//discard should now include all the previous deck cards and all the previous discard cards up to the first treasure
			//+ the adventurer card itself discarded but it should lose its treasure card
			beforeFunctionState.discardCount[currentPlayer] += tempDeckCount + firstTreasureDiscardPos + 2 - 1;


			if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]) {
				testFailed = 1;
				failures->didNotAddTreasureToHand++;
			}
			if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
				testFailed = 1;
				failures->didNotDiscardProperly++;
				failures->didNotReturnTreasureFromDiscard++;
			}
			if (beforeFunctionState.deckCount[currentPlayer] != currentGameState->deckCount[currentPlayer]) {
				testFailed = 1;
				failures->didNotReturnTreasureFromDeck++;
			}

			//check if the treasure cards are only in discard
			else if (firstTreasureDeckPos == -1 && secondTreasureDeckPos == -1
				&& firstTreasureDiscardPos >= 0 && secondTreasureDiscardPos >= 0) {
				
				//if there are two treasures in the deck and deck combo, then 2 treasures should be added to the hand
				//but we only add 1 card as adventurer should be discarded

				beforeFunctionState.handCount[currentPlayer] += 2;
				beforeFunctionState.handCount[currentPlayer]--;
				
				int tempDeckCount = beforeFunctionState.deckCount[currentPlayer];

				//the deck should have been reshuffled so should now include none of its original cards and all the discard cards up to the 2nd discard treasure
				beforeFunctionState.deckCount[currentPlayer] = beforeFunctionState.discardCount[currentPlayer] - secondTreasureDiscardPos + 1;

				//discard should now include all the previous deck cards and all the previous discard cards up to the second treasure
				//+ the adventurer card itself discarded but it should lose its two treasure cards so the + 2 - 2 cancel out
				beforeFunctionState.discardCount[currentPlayer] += tempDeckCount + secondTreasureDiscardPos;


				if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotAddTreasureToHand++;
				}
				if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotDiscardProperly++;
					failures->didNotReturnTreasureFromDiscard++;
				}
			}

			//if only 1 treasure card in the discard
			else if (firstTreasureDeckPos == -1 && secondTreasureDeckPos == -1
				&& firstTreasureDiscardPos >= 0 && secondTreasureDiscardPos == -1) {

				//hand count will stay the same

				int tempDeckCount = beforeFunctionState.deckCount[currentPlayer];

				//the deck should be completely empty
				beforeFunctionState.deckCount[currentPlayer] = 0;

				//discard should now include all of the deck + the adventurer card + what it had before - the one treausre
				beforeFunctionState.discardCount[currentPlayer] += tempDeckCount + firstTreasureDiscardPos + 1;

				if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotAddTreasureToHand++;
				}
				if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotDiscardProperly++;
					failures->didNotReturnTreasureFromDiscard++;
				}
			}
			//if only 1 treasure card in the deck
			else if (firstTreasureDeckPos >= 0 && secondTreasureDeckPos == -1
				&& firstTreasureDiscardPos == -1 && secondTreasureDiscardPos == -1) {
				
				//hand count will stay the same

				//take out all the flipped cards
				beforeFunctionState.deckCount[currentPlayer] -= firstTreasureDeckPos + 1;

				//add them to the discard pile + the adventurer card
				beforeFunctionState.discardCount[currentPlayer] += firstTreasureDeckPos + 2;


				if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotAddTreasureToHand++;
				}
				if (beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotDiscardProperly++;
				}
				if (beforeFunctionState.deckCount[currentPlayer] != currentGameState->deckCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotReturnTreasureFromDeck++;
				}
			}

			//if no treasure cards in deck or discard
			else if (firstTreasureDeckPos == -1 && secondTreasureDeckPos == -1
				&& firstTreasureDiscardPos == -1 && secondTreasureDiscardPos == -1) {

				//hand count should lose adventurer card
				beforeFunctionState.handCount[currentPlayer]--;
				
				int tempDeckCount = beforeFunctionState.deckCount[currentPlayer];

				//deck should be completely empty as all cards should now be in discard as they were all flipped
				beforeFunctionState.deckCount[currentPlayer] = 0;

				//discard should now be what it was previously + the deck + the adventurer discard
				beforeFunctionState.discardCount[currentPlayer] += tempDeckCount + 1;

				if (beforeFunctionState.handCount[currentPlayer] != currentGameState->handCount[currentPlayer]
					&& beforeFunctionState.discardCount[currentPlayer] != currentGameState->discardCount[currentPlayer]) {
					testFailed = 1;
					failures->didNotDiscardProperly++;
				}
			}
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

