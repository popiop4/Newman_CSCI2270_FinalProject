#ifndef GAME_H
#define GAME_H
#include <iostream>

using namespace std;


struct Deck{
    Deck *next;//pointers for deck
    Deck *prev;
    int type;//type of card(1 - 5)
    int amt;//card modifier
    string description;
};
struct Players{
    std::string name;
    int location;
    Players *next;
    Deck *pDeck;//players first card
    bool human;//1 ia human 0 is bot
    int slowAmt;//how slowed they should be next turn
    int rollamt;//amount of times to roll
    Players(){};
	Players(string inname,bool poc)
	{
	    slowAmt = 1;
	    human = poc;
        name = inname;
		location = 0;
        pDeck = NULL;
        Players *next = NULL;
	}
};


class Game
{
    public:
        void boardCreation(int boardSize);//sets boardsize private variable
        int playBot(Players* comp);//Bots "AI"
        void createPlayer(string name, bool poc);//adds player
        void movePlayer(Players* player);//moves player
        void printBoard();//displays board
        void createCard(string text,int type,int amt);//adds a card
        int viewCards(Players *player);//card running function
        Deck* drawCard();//draws a random card
        void fight(Players* player1, Players* player2);//runs fight between two players
        Players* locationCheck(Players* player);//checks if 2 players or in same spot
        Game();
        ~Game();
        Players *curr;//current player
        Players *firstPlayer = NULL;//first player
    protected:

    private:
        int diceRoll();//picks num 1-6
        Deck *firstCard = NULL;//bottom of deck
        int cardsRem = 0;//number of cards remaining
        int amtPlayers;//number of payers in the game
        int s;//size of board
        void wait(int sec);//wait function to slow down program

};

#endif // GAME_H
