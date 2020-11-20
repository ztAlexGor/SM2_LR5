#pragma once
#include "Card.h"
#include <windows.h>

enum {PLAYER1, PLAYER2};

class Game{
private:
	class Player
	{	
	public:
		int HP;
		int Money;
		static int NumberOfCards;
		Card** deck;
		Player();
		~Player();
	private:

	};
	
	double ProbTypeOfCards[TYPE_OF_CARDS_NUMBER] = { 0.5, 0.3, 0.2 };
	double ProbAtack[ATTACK_CARDS_NUMBER] = { 0.5, 0.3, 0.2 };
	double ProbDeffense[DEFFENSE_CARDS_NUMBER] = { 0.4, 0.3, 0.3 };
	double ProbMonetary[MONETARY_CARDS_NUMBER] = { 0.6, 0.3, 0.1 };

	//AttackCard* templateOfCards;
	HBITMAP* hImageAttack;
	BITMAP* imageAttack;
	Player player1, player2;
	int turn;
	bool isGameRun;
	int extraMoneyPerDay;
public:
	Game(HBITMAP* hImageAttack, BITMAP* imageAttack);
	void Initialize();
	Card* GenerateRandomCard();
	static int MakeAChoice(double*, int);
	int GetTurn();
	bool GetGameStatus();
	bool MakeMove(int player, int cardNumber);
	bool isLegalMove(int player, int cardNumber);
	bool SetIfGameWin();//0 if game continue, 1 - win
	bool Draw(HDC hdc);
};

