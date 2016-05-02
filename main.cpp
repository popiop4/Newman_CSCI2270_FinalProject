#include "Game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(){
    int humanPlayers = 0;
    int compPlayers = 0;
    bool range = false;

    cout<<"Welcome to BOARD GAME THE MUSICAL"<<endl<<endl;

    cout<<"How many human players are playing? 1-5 "<<endl;
    cin>>humanPlayers;//Amount of humans playing
    while(humanPlayers!= 1 &&humanPlayers!= 2 &&humanPlayers!= 3 &&humanPlayers!= 4 &&humanPlayers!= 5){//if the player picks a diff value
        cout<<"Please pick a number between 1-5 "<<endl;
        cin>>humanPlayers;
    }
    cout<<endl;

    if(humanPlayers==1){//if only one human allows 1 to 5-human players amount of computers
        cout<<"How many computer players would you like? 1-"<<5-humanPlayers<<endl;
        while(range == false){
            cin>>compPlayers;
            if(compPlayers > 0 && compPlayers < 5-humanPlayers){
                range = true;
            }
            else if(compPlayers<0 || compPlayers>(5-humanPlayers)){
                cout<<"Please choose a number between 1-"<<5-humanPlayers<<endl;
            }
        }
        cout<<endl;
    }

    else if(humanPlayers!=5){//if there are multiple humans allows 0 computers
        cout<<"How many computer players would you like? 0-"<<5-humanPlayers<<endl;
        while(range == false){
            cin>>compPlayers;
            if(0 < compPlayers && compPlayers < 5-humanPlayers){
                range = true;
            }
            else{
                cout<<"Please choose a number between 1-"<<5-humanPlayers<<endl;
            }
        }
    }
    cout<<endl;
    Game g;//starts game class
    cin.ignore();
    for(int i = 0;i<humanPlayers;i++){//get's name and initializes players

        string name;
        cout<<"Welcome player "<<i+1<<", what is your name?"<<endl;
        getline(cin,name);
        g.createPlayer(name,true);//true because human
        cout<<endl;
    }
    for(int i = 0;i<compPlayers;i++){//creates bots
        string c = "bot";
        string name = char(49+i) + c + char(49+i);//makes them all have diff names
        cout<<"Welcome, "<<name<<"!"<<endl;
        g.createPlayer(name,false);//false because bot
    }
    g.curr->next = g.firstPlayer;//loops the linked list of players into a circle
    cout<<"How large would you like the board to be? 10-75"<<endl<<endl;
    int bsize;
    cin>>bsize;//gets size of board
    while(bsize<10||bsize>75){//Asks again if size out of bounds
        cout<<"Please enter a number between 10-75"<<endl<<endl;
        cin>>bsize;
    }
    g.boardCreation(bsize);//creates board

    string filename = "cards.txt";//sets filename
    ifstream file;
    file.open(filename.c_str());//opens file

    string line;//initializations:
    string word;
    string text;
    int type;
    int amt;
    int counter = 1;

    while(getline(file,line)){//reads file, file is in three's so the counter increments to 3 then restarts
            if(counter == 1){//gets the description line
                text = line;
            }
            if(counter == 2){//gets the type, 1-5, of the card
                type = atoi(line.c_str());//the atoi turns the string into an int
            }
            if(counter == 3){//gets modifier value
                int amt = atoi(line.c_str());
                counter = 0;//restarts counter
                g.createCard(text,type,amt);
            }
            counter++;//increases counter
    }

    bool win = false;//end game
    bool quit = false;

    g.curr=g.firstPlayer;//starts at first player
    g.printBoard();
    while(!win&&!quit){//plays while no one has won or quit
        bool skipNext = false;//for when a player is skipped
        if(g.curr->human){//plays a human turn
            cout<<g.curr->name<<", it's your turn! What would you like to do?"<<endl<<endl;
            string choice = "0";
            int amt = 0;
            while(choice!="1"&&choice!="3"){//keeps showing menu until turn is over
                cout<<"1. Roll dice"<<endl;
                cout<<"2. View Cards"<<endl;
                cout<<"3. Flip Board"<<endl;
                cout<<"4. View Board"<<endl<<endl;

                cin>>choice;

                while(choice!="1"&&choice!="2"&&choice!="3"&&choice!="4"){//if player chooses wrong value
                    cout<<"Try again"<<endl;
                    cin>>choice;
                }
                if(choice == "1"){//roll dice to end turn
                    for(int k = 0;k<=g.curr->rollamt;k++){
                        g.movePlayer(g.curr);//runs move function
                        if(g.locationCheck(g.curr)){//checks if a fight needs to start
                            g.fight(g.locationCheck(g.curr),g.curr);
                        }
                        if(g.curr->location%2 == 1){// if location is odd gives them a card
                            cout<<"You found a card!"<<endl<<endl;
                            Deck* card = g.drawCard();//draws card
                            Deck* cardcopy = card;//copies card to give to player's deck
                            cardcopy->next = NULL;
                            cardcopy->prev = NULL;
                            if(g.curr->pDeck == NULL){//if player has no cards
                                g.curr->pDeck = cardcopy;//makes this their first card
                            }
                            else{//otherwise
                                Deck* temp = g.curr->pDeck;
                                while(temp->next){//loops to last card in players deck
                                    temp = temp->next;
                                }
                                temp->next = cardcopy;//adds card
                                cardcopy->prev = temp;
                            }
                        }

                    }
                    g.printBoard();//prints board
                    g.curr->rollamt = 0;
                    g.curr->slowAmt = 1; //resets their slow value to one encase it was changed
                }
                if(choice == "2"){//shows cards
                    amt = g.viewCards(g.curr);//runs card function, returns amount for repetitive rolls
                    if(amt == -1){//if it puts out a -1 value then you should skip next player
                        skipNext = true;
                        amt = 0;//resets it to 1
                    }
                    g.curr->rollamt = amt;
                }
                if(choice == "3"){//quits game
                    cout<<"You flip the board, everyone loses"<<endl;
                    quit = true;
                    return 0;
                }
                if(choice == "4"){//prints board
                    g.printBoard();
                }
                if(g.curr->location>=bsize){//if player reaches the end stops game
                    cout<<"You won "<<g.curr->name<<", congrats"<<endl;
                    win = true;
                    return 0;
                }
            }
            if(!skipNext){
                g.curr = g.curr->next;
            }//rotates game to next player
            else{
                g.curr = g.curr->next;
                g.curr = g.curr->next;
            } //if skip is true then skips next player and rotates to player after next
        }
        else{//bots turn
            amt = g.playBot(g.curr);//plays bot
            if(g.locationCheck(g.curr)){
                g.fight(g.locationCheck(g.curr),g.curr);
            }
            if(g.curr->location>=bsize){//if bot wins ends game
                cout<<"You won "<<g.curr->name<<", congrats"<<endl;
                win = true;
                return 0;
            }
            if(g.curr->location%2 == 1){//if they land on an odd square gives bot a card
                cout<<g.curr->name<<" found a card!"<<endl<<endl;
                Deck* card = g.drawCard();
                Deck* cardcopy = card;
                cardcopy->next = NULL;
                cardcopy->prev = NULL;
                if(g.curr->pDeck == NULL){
                    g.curr->pDeck = cardcopy;
                }
                else{
                    Deck* temp = g.curr->pDeck;
                    while(temp->next){
                        temp = temp->next;
                    }
                    temp->next = cardcopy;
                    cardcopy->prev = temp;
                }
            }
            if(amt == -1){// if amt is -1 then skip next player
                skipNext = true;
                amt = 0;
            }
            g.printBoard();
            g.curr->slowAmt = 1;

            if(!skipNext){
                g.curr = g.curr->next;
            }//skips if skip is true
            else{
                g.curr = g.curr->next;
                cout<<"Skipping "<<g.curr->name<<endl;
                g.curr = g.curr->next;
            }

        }
    }
}
