/* Copyright (c) 1998 by The National Chengchi Univ. All Rights Reserved */

/***************************************************************************
   NAME
     BJackGame.cc
   PURPOSE
     
   NOTES
     
   AUTHOR
     Tsai-Yen Li (li@cs.nccu.edu.tw)
   HISTORY
     Tsai-Yen Li - Oct 12, 1998: Created.
***************************************************************************/

/* $Log:$ */

#include <iostream>
#include "AnsiPrint.h"
#include "Card.h"
#include "BJackGame.h"
#include "BJackPlayer.h"
#include "BJackDealer.h"
#include "BJackGame.h"

using namespace std;

// only constructor
// Must be given a valid (initialized) game player and dealer
// Menu is constructed internally to provide basic text interface
// We must use an initialization list here.
BJackGame::BJackGame(BJackPlayer &bjp, BJackDealer &bjd) : menu(sizeof(menuItems)/sizeof(char *), menuItems), bjplayer(bjp), bjdealer(bjd) {
    need_restart = true;
    restart();
    bjplayer.showCards();
    bjdealer.showCards();
}

// one run of the game
// take and process a user action
// show current hands for both players at the end of a run.
bool
BJackGame::oneRun() {
    menu.print();
    if(need_restart)
    {
        switch(menu.getAnswer())
        {
            case 3:
                restart();
                break;
            case 4:
                return false;
        }
    }
    else
    {
        switch(menu.getAnswer())
        {
            case 1:
                moreCard();
                break;
            case 2:
                enough();
                break;
            case 3:
                restart();
                break;
            default:
                return false;
        }
    }
    bjplayer.showCards();
    bjdealer.showCards();
    return true;
}

// give the player one more card
// check if we can do that first
void BJackGame::moreCard() {
    if(bjplayer.totalPoints() <= 21)
    {
        bjplayer.addCard(bjdealer.giveCard());
    }
}

// give the dealer enough cards to try to win the player
// determine who win the game at the end
void BJackGame::enough() {
    bjdealer.addCards(bjplayer.totalPoints());
    result result = bjdealer.judge(bjplayer.totalPoints());
    bjdealer.openFirstCard();
    if(result == win)
    {
        cout << "You win. Good job!";
        bjplayer.addCredits(1);
        bjdealer.addCredits(-1);
    }
    else if(result == lose)
    {
        cout << "I win. Try again.";
        bjplayer.addCredits(-1);
        bjdealer.addCredits(1);
    }
    else
    {
        cout << "It's a Tie.";
    }

    cout << "(You have " << bjplayer.getCredits() << " Credits, I have " << bjdealer.getCredits() << " Credits.)" << endl;
    need_restart = true;
    //bjdealer.showCards();
}

// restart the game by giving each player two new cards
// need to check if any one broke (bankrupt).
void BJackGame::restart() {
    if (!need_restart)
    {
        cout << "Game is not over yet. Choose 'enough' to end the game" << endl;
        return;
    }
    if (bjdealer.getCredits() <= 0)
    {
        cout << "Sorry. Game over. No loan here." << endl;
        return;
    }
    //cout << "restart" << endl;
    bjplayer.start();
    bjdealer.start();
    bjplayer.addCard(bjdealer.giveCard());
    bjplayer.addCard(bjdealer.giveCard());
    bjdealer.addCard();
    bjdealer.addCard();
    need_restart = false;
}

