/* Copyright (c) 1998 by The National Chengchi Univ. All Rights Reserved */

/***************************************************************************
   NAME
     BJackDealer.cc
   PURPOSE
     
   NOTES
     
   AUTHOR
     Tsai-Yen Li (li@cs.nccu.edu.tw)
   HISTORY
     Tsai-Yen Li - Oct 12, 1998: Created.
***************************************************************************/

/* $Log:$ */

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
}

using namespace std;

#include <iostream>
#include "AnsiPrint.h"
#include "Card.h"
#include "CardPat.h"
#include "BJackPlayer.h"
#include "BJackDealer.h"
#include <string.h>


// constructor 
// nDeck is the number of standard decks that the dealer will deal with
BJackDealer::BJackDealer(int nSet) {

    for(int i = 0; i < nSet; i++)
    {
        for(int j = 0; j < 52; j++)
        {
            deck.push_back(Card(j));
        }
    }

    decknum = nSet * 52;
    name = "Dealer";
    credits = 3;
    start();

}

// distribute a card to the player
int BJackDealer::giveCard() {

    if(deckpin < decknum/2) shuffle();
    return deck[deckpin++].getID();
}

// give the dealer one card unconditionally
void BJackDealer::addCard() {
    handcard.push_back(giveCard());

}

int BJackDealer::totalPoints(void) const {
    int total=0;
    int point;
    int credit=0;

    for(int i=0; i<handcard.size(); i++) {
        point=handcard[i].getPip()+1;
        if (point==1) {
            credit++;
        } else if (point>10) {
            point=10;
        }
        total += point;
    }

    // add points depending on how many aces we have
    for(int i=0; i<credit; i++) {
        if (total+10 <=  21) {
            total += 10;
        } else {
            break;
        }
    }

    return total;
}


// give the dealer as many cards as necessary to exceed the player 
// of the given total points
// A special case happen when both of them have 21 points.
// It is a tie game and therefore the dealer does not get more cards.

void BJackDealer::addCards(int oppTotal) {
    while(totalPoints() < oppTotal || totalPoints() < 16)
    {
        addCard();
        if(totalPoints() >= 21 || handcard.size() >= 5) return;
    }
}

// Determine who has win the game.
// The total points that the dealer has should be greater than 
// the player at the point this function is called.

result BJackDealer::judge(int oppTotal) const {

    if(oppTotal > 21) return lose;
    if(totalPoints() > 21) return win;
    if(handcard.size() >= 5) return lose;
    if(oppTotal < totalPoints()) return lose;
    if(oppTotal == totalPoints()) return draw;
    if(oppTotal == 21 && totalPoints() == 21) return draw;
    return win;
}

// start a new game
// before starting a new game, check if there are enough cards left for 
// the next game.

void BJackDealer::start(void) {
    shuffle();
    is_open = false;
    while(handcard.size() > 0) handcard.pop_back();
}

// shuffle the deck of cards
void BJackDealer::shuffle(void) {
    deckpin = 0;
    for(int i = 0; i < decknum; i++)
    {
        int temp = deck[i].getID();
        int change = rand() % decknum;
        deck[i].setID(deck[change].getID());
        deck[change].setID(temp);
    }

}

// reset the seed before shuffling
void BJackDealer::shuffle(long seed) {
    srand(seed);
    shuffle();
}

// open the dealer's first card 
void BJackDealer::openFirstCard(void) {
    is_open = true;

}

int BJackDealer::getCredits()
{
    return credits;
}

void BJackDealer::addCredits(int a)
{
    credits += a;
}

inline int Min(int i, int j) {
  return (i<j)? i:j; 
}

// show the dealer's hand to the screen
void BJackDealer::showCards(void) const {

    unsigned char s,p;
    char line[kCardWidth+1];
    int curCard=0, lastCard;

    while(curCard < handcard.size()) {
        for(int i=0;i<kCardHeight;i++) {
            lastCard = Min(curCard+kNCardPerRow, handcard.size());
            if (i < int(name.size())) {
                cout << name[i] << " ";
            } else {
                cout << "  ";
            }
            for(int j=curCard;j<lastCard;j++) {
                s=handcard[j].getID()%kNSuit;
                p=handcard[j].getID()/kNSuit;
                if ((j==0)&&(is_open==false)) 
                    p=13;
                for(int k=0;k<kCardWidth;k++) {
                    line[k]=card[p][i][k];
                    if (line[k]=='x') 
                        line[k]=cardAbbrev[s];
                }
                line[kCardWidth]='\0';
                AnsiPrint(line,cardColor[s],white);
                cout << " ";
            }
            cout << endl;
        }
        curCard=lastCard;
    }
}

