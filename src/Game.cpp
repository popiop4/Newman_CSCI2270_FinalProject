#include "Game.h"
#include <cstdlib>
#include "time.h"
#include <iostream>
#include <ctime>

using namespace std;
Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}
void Game::boardCreation(int boardSize){
    s = boardSize;//sets boardsize
}
int Game::playBot(Players* comp){//bots turn
    srand(time(NULL));
    int num = rand()%2;
    if(num = 1&&comp->pDeck){//50-50 chance of using a card if they have one

        int counter = 0;//counter to find out how many cards a bot has
        Deck *Cards = comp->pDeck;//points to first card
        while(Cards){//counts cards
            counter++;
            Cards = Cards->next;
        }
        int choice = rand()%counter+1;//chooses a rand number in amount of cards a bot has

        Deck *C = comp->pDeck;//points to first card
        for(int i = 1;i <choice;i++){//loops to number bot chose
            C = C->next;
            i;
        }
        cout<<comp->name<<" played "<<C->description<<endl;//says what card they chose
        if(C->prev == NULL&&C->next==NULL){//removes card from their deck
            comp->pDeck = NULL;//if they only have the one card
        }else if(C->next == NULL){//if at end of deck
            C->prev->next = NULL;
        }else if(C->prev == NULL){//if at front of deck
            C->next->prev = NULL;
            comp->pDeck = C->next;
        }else{//if in the middle
            C->next->prev = C->prev;
            C->prev->next = C->next;
        }
        if(C->type == 1){//move your player forward
            comp->location = comp->location + C->amt;//increases amt
            if(locationCheck(comp)){//checks for fights
                fight(locationCheck(comp),comp);
            }
        }
        if(C->type == 2){//moves another payer back
            Players *victim = comp;//sets a pointer t the computer
            int p = rand()%amtPlayers+1;//choses a player randomly
            for(int i = 0;i<p;i++){//loops to that player
                victim = victim->next;
            }
            victim->location = victim->location - C->amt;//moves them back
            cout<<"\"Take that, "<<victim->name<<"!"<<endl;
            if(locationCheck(victim)){//checks them for fights
                fight(locationCheck(victim),victim);
            }
            return 0;//ends
        }
        if(C->type == 3){//rolls multiple times if you should
            for(int o = 1;o<=C->amt;o++){
                movePlayer(comp);
                if(locationCheck(comp)){//fight check
                    fight(locationCheck(comp),comp);
                }
            }
            return 0;
        }
        if(C->type ==4){//for when a player needs to be slowed
            Players *victim = comp;
            int p = rand()%amtPlayers+1;//chooses a player randomly
            for(int i = 0;i<p;i++){//loops to that player
                victim = victim->next;
            }
            victim->slowAmt = C->amt;//slows them
            cout<<"\"Take that, "<<victim->name<<"!"<<endl;
            return 0;
        }
        if(C->type == 5){//skips next player
            return -1;
        }
    }
    //50-50 chance to just move
    movePlayer(comp);
    if(locationCheck(comp)){//fight check
        fight(locationCheck(comp),comp);
    }
}
void Game::createPlayer(string name, bool poc){//creates a player
    Players *newp = new Players();//new player
    newp->name = name;//initializes player values
    newp->human = poc;
    newp->location = 1;
    newp->pDeck = NULL;
    newp->next = NULL;
    newp->slowAmt = 1;
    newp->rollamt = 0;
    if(!firstPlayer){//if the first player to be added makes it the fist player
        firstPlayer = newp;
        curr = newp;
        amtPlayers = 1;
    }
    else{//otherwise adds player to loop
        amtPlayers++;
        curr->next = newp;
        curr = newp;
    }
    if(poc){//if a real player
    cout<<"Welcome, "<<newp->name<<"!"<<endl<<endl;
    }
}
void Game::movePlayer(Players* player){//moves player
    wait(1000);
    int num = diceRoll();//rolls a number 1-6
    cout<<player->name<<" rolled a "<<num<<"."<<endl;
    wait(1000);
    if(player->slowAmt == 1) player->location = player->location + num;//normally moves that many
    else{//otherwise moves by a slowed amount
        num = num/player->slowAmt;//divides their move by an amount
        cout<<"But was slowed down and only moved "<<num<<"places."<<endl;
        player->location = player->location + num;
    }
}
void Game::printBoard(){//board display
    Players *player = firstPlayer;
    cout<<">";//puts arrows at the from
    for(int l = 0;l<s;l++){//line along top
        cout<<"-";
    }
    cout<<"#";//pounds at end
    cout<<endl;
    for(int i = 0;i<amtPlayers;i++){//loops through a line for each player
        cout<<">";
        for(int k = 1;k<player->location;k++){
            cout<<" ";
        }
        cout<<player->name[0];
        for(int j = 0;j<s-player->location;j++){
            cout<<" ";
        }
        cout<<"#";
        player = player->next;
        cout<<endl;
    }
    cout<<">";
    for(int l = 0;l<s;l++){//line at bottom
        cout<<"-";
    }
    cout<<"#";
    cout<<endl;
    wait(1000);
}
void Game::createCard(string text,int type,int amt){//creates a new card
    Deck *newcard = new Deck;//new card
    newcard->description = text;//puts in values
    newcard->type = type;
    newcard->amt = amt;

    if(firstCard == NULL){//if first card makes it the first card
        newcard->prev = NULL;
        newcard->next = NULL;
        firstCard = newcard;
        cardsRem = 1;
    }else{
        Deck *temp = firstCard;
        while(temp->next){//goes to last card
            temp = temp->next;
        }
        temp->next = newcard;//adds card to back of deck
        newcard->prev = temp;
        newcard->next = NULL;
        cardsRem++;//adds amount of cards remaining
    }
}

int Game::viewCards(Players *player){
    if(!player->pDeck){//if player has no cards
        cout<<"You have no cards!"<<endl;
    }
    else{//otherwise
        int counter = 1;//counts how many cards the player has
        Deck *Cards = player->pDeck;
        while(Cards){//counts how many cards
            cout<<counter<<". "<<Cards->description<<endl;
            counter++;
            Cards = Cards->next;
        }
        cout<<endl<<"Which card would you like to use? (press zero to go back)"<<endl;
        int choice;
        cin>>choice;
        while(choice > counter){//if the player chooses an incorrect value
            cout<<"Please pick a card or press 0 to go back"<<endl;
            cin>>choice;
        }
        if(choice == 0){//exits menu
            return 0;
        }
        else{
            Deck *C = player->pDeck;
            for(int i = 1;i <choice;i++){//loops to the card they chose
                C = C->next;
            }
            if(C->prev == NULL&&C->next==NULL){//removes that card from their deck
                player->pDeck = NULL;
            }else if(C->next == NULL){
                C->prev->next = NULL;
            }else if(C->prev == NULL){
                C->next->prev = NULL;
                player->pDeck = C->next;
            }else{
                C->next->prev = C->prev;
                C->prev->next = C->next;
            }
            if(C->type == 1){//type for moving forward
                player->location = player->location + C->amt;//moves player
                if(locationCheck(player)){//fight check
                    fight(locationCheck(player),player);
                }
                return 0;
            }
            if(C->type == 2){//moves another player back
                cout<<"Which player would you like to move?"<<endl;//finds player they chose
                string name;
                cin>>name;
                Players *victim = player;
                bool found = false;
                while(!found){//tries to find
                    for(int i = 0;i<amtPlayers;i++){//loops through all players
                        victim = victim->next;
                        if(victim->name == name){//if name matches
                            victim->location = victim->location - C->amt;//moves them
                            if(locationCheck(victim)){//fight check
                                fight(locationCheck(victim),victim);
                            }
                            found = true;
                            return 0;//ends program
                        }
                    }
                    cout<<"That player could not be found, try again"<<endl;
                    cin>>name;//tries again if name wrong
                }
            }
            if(C->type == 3){//if type three sets amount for how many times to roll
                return C->amt - 1;
            }
            if(C->type ==4){//type for slowing down another player
                cout<<"Which player would you like to slow?"<<endl;
                string name;
                cin>>name;
                Players *victim = player;
                bool found = false;
                while(!found){//tries to find the player
                    for(int i = 0;i<amtPlayers;i++){//loops through all players
                        victim = victim->next;
                        if(victim->name == name){//if name matches
                            victim->slowAmt = C->amt;//sets their slow amount
                            found = true;
                            return 0;
                        }
                    }
                    cout<<"That player could not be found, try again"<<endl;//if name isn't found tries again
                    cin>>name;
                }
            }
            if(C->type == 5){//type  5 skips next player
                return -1;
            }
        }
    }return 0;
}
Deck* Game::drawCard(){
    if(cardsRem == 1){//if no cards left
        cout<<"there are no more cards!, but here's a piece of paper I found"<<endl;
        Deck *Blank = new Deck;
        Blank->amt = 0;
        Blank->description = "Still just a piece of paper";
        Blank->next = NULL;
        Blank->prev = NULL;
        Blank->type = 1;
        return Blank;
    }
    else{//otherwise
        srand(time(NULL));
        int num = rand()%cardsRem;//random chooses a number within the amount remaining
        Deck *card = firstCard;
        for(int i = 0;i<num;i++){//loops to that card
            card = card->next;
        }
        if(card == firstCard){//removes card from deck
            firstCard = card->next;
            firstCard->prev = NULL;
        }
        else if(card->next == NULL){
            card->prev->next = NULL;
        }
        else{
            card->prev->next = card->next;
            card->next->prev = card->prev;
        }
        if(cardsRem != 0)cardsRem--;//decrements amount of cards in deck
        return card;
    }

}
int Game::diceRoll(){
    //srand(time(NULL));
    int num = rand()%6+1;//chooses a number 1-6
    return num;

}
void Game::fight(Players* player1, Players* player2){
    cout<<"FIGHT"<<endl<<endl;
    printBoard();//display board
    int firstAttack =0;
    if(player1->human){//if first player is a human
        cout<<player1->name<<", what would you like to do?"<<endl<<"1. Rock."<<endl<<"2. Paper"<<endl<<"3. Scissors."<<endl;
        cin>>firstAttack;//chooses their attack
        while(firstAttack!=1&&firstAttack!=2&&firstAttack!=3){//tries again if it's wrong
            cout<<"You can't do that, try again"<<endl;
            cin>>firstAttack;
        }
    }
    else{//bot chooses an attack randomly
        srand(time(NULL));
        firstAttack = rand()%3+1;
    }
    int secondAttack=0;
    if(player2->human){//if first player is a human
        cout<<player2->name<<", what would you like to do?"<<endl<<"1. Rock."<<endl<<"2. Paper"<<endl<<"3. Scissors."<<endl;
        cin>>secondAttack;
        while(secondAttack!=1&&secondAttack!=2&&secondAttack!=3){//tries again if it's wrong
            cout<<"You can't do that, try again"<<endl;
            cin>>secondAttack;
        }
    }
    else{//bot chooses an attack randomly
        secondAttack = rand()%3+1;
    }//if they are the same
    if(firstAttack == secondAttack){
        cout<<"You tied oh well"<<endl;
    }
    if((firstAttack ==1 && secondAttack==3)||(firstAttack ==2 && secondAttack==1)||(firstAttack ==3 && secondAttack==2)){//if player one wins
        if(firstAttack == 1){
            cout<<"Rock beats scissors! "<<player1->name<<" wins! "<<player2->name<<" move back 5 spots!"<<endl;
        }
        if(firstAttack == 2){
            cout<<"Paper beats Rock! "<<player1->name<<" wins! "<<player2->name<<" move back 5 spots!"<<endl;
        }
        if(firstAttack == 3){
            cout<<"Scissors beat paper! "<<player1->name<<" wins! "<<player2->name<<" move back 5 spots!"<<endl;
        }
        if(player2->location - 5 <1){//moves loser back to start
            player2->location = 1;
        }
        else{//or back 5 places
            player2->location = player2->location - 5;
        }
    }
    if((firstAttack ==3 && secondAttack==1)||(firstAttack ==1 && secondAttack==2)||(firstAttack ==2 && secondAttack==3)){//if player 2 wins
        if(firstAttack == 3){
            cout<<"Rock beats scissors! "<<player2->name<<" wins! "<<player1->name<<" move back 5 spots!"<<endl;
        }
        if(firstAttack == 1){
            cout<<"Paper beats Rock! "<<player2->name<<" wins! "<<player1->name<<" move back 5 spots!"<<endl;
        }
        if(firstAttack == 2){
            cout<<"Scissors beat paper! "<<player2->name<<" wins! "<<player1->name<<" move back 5 spots!"<<endl;
        }
        if(player1->location - 5 <1){//moves loser back to start
            player1->location = 1;
        }
        else{//or back 5 spots
            player1->location = player1->location - 5;
        }
    }
}
Players* Game::locationCheck(Players* player){//checks for players in the same spot
    if(player->location == 1){//won't allow fights at start
        return NULL;
    }
    else{
        Players *temp = player->next;
        for(int i = 0;i<amtPlayers;i++){//loops through all players
            if(player->location==temp->location){//checks for matching location to player in question
                return temp;
            }
        }
    }
    return NULL;
}

void Game::wait(int milisec){//slow function
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
    continue;
}
