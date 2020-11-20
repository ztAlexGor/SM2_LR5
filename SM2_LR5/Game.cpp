#include "Game.h"
#include <iostream>
#include <string>


Game::Player::Player() {
	this->HP = 50;
	this->Money = 200;
	deck = new Card * [NumberOfCards];
}

Game::Player::~Player(){
	delete[] deck;
}

Game::Game(HBITMAP* hImageAttack, BITMAP* imageAttack) {
	this->turn = PLAYER1;
	this->isGameRun = true;
	this->extraMoneyPerDay = 10;
	this->hImageAttack = hImageAttack;
	this->imageAttack = imageAttack;
}

void Game::Initialize(){
	for (int i = 0; i < Player::NumberOfCards; i++) {
		player1.deck[i] = this->GenerateRandomCard();
		player2.deck[i] = this->GenerateRandomCard();
	}
}

Card* Game::GenerateRandomCard(){
	int decision = MakeAChoice(ProbTypeOfCards, TYPE_OF_CARDS_NUMBER);
	switch (decision)
	{
	case Attack:
		decision = MakeAChoice(ProbAtack, ATTACK_CARDS_NUMBER);
		switch (decision)
		{
		case 0:
			return new AttackCard{ 5, 10, hImageAttack, imageAttack };
		case 1:
			return new AttackCard{ 10, 25, hImageAttack, imageAttack };
		case 2:
			return new AttackCard{ 20, 45, hImageAttack, imageAttack };
		}
	case Deffence:
		decision = MakeAChoice(ProbDeffense, DEFFENSE_CARDS_NUMBER);
		switch (decision)
		{
		case 0:
			return new DeffenceCard{ 5, 10, hImageAttack, imageAttack };//DeffenceCard
		case 1:
			return new DeffenceCard{ 8, 20, hImageAttack, imageAttack };
		case 2:
			return new DeffenceCard{ 10, 30, hImageAttack, imageAttack };
		}
	case Monetary:
		decision = MakeAChoice(ProbMonetary, MONETARY_CARDS_NUMBER);
		switch (decision)
		{
		case 0:
			return new MonetaryCard{ 15, 0, hImageAttack, imageAttack };//MonetaryCard
		case 1:
			return new MonetaryCard{ 25, 0, hImageAttack, imageAttack };
		case 2:
			return new MonetaryCard{ 35, 0, hImageAttack, imageAttack };
		}
	default:
		return nullptr;
	}
}

int Game::MakeAChoice(double* prob, int size) {
	double randValue = (double)std::rand() / (double)RAND_MAX;
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += prob[i];
		if (randValue < sum)return i;
	}
	return size - 1;
}

int Game::GetTurn(){
	return turn;
}

bool Game::GetGameStatus(){
	return isGameRun;
}

bool Game::MakeMove(int player, int cardNumber){
	if (!isLegalMove(player, cardNumber) || isGameRun == false)return false;

	if (player == PLAYER1) {
		player1.deck[cardNumber]->Use(player1.HP, player2.HP, player1.Money, player2.Money);
		delete player1.deck[cardNumber];
		player1.deck[cardNumber] = GenerateRandomCard();
		player2.Money += extraMoneyPerDay;
		turn = PLAYER2;
	}
	else {
		player2.deck[cardNumber]->Use(player2.HP, player1.HP, player2.Money, player1.Money);
		delete player2.deck[cardNumber];
		player2.deck[cardNumber] = GenerateRandomCard();
		player1.Money += extraMoneyPerDay;
		turn = PLAYER1;
	}
	SetIfGameWin();
	return true;
}

bool Game::isLegalMove(int player, int cardNumber){
	if (player != turn)return false;
	if (player == PLAYER1) {
		if (player1.Money < player1.deck[cardNumber]->cost)return false;
	}
	else {
		if (player2.Money < player2.deck[cardNumber]->cost)return false;
	}
	return true;
}

bool Game::SetIfGameWin(){
	if (player1.HP <= 0) {
		player1.HP = 0;
		isGameRun = false;
		return true;
	}
	else if (player2.HP <= 0) {
		player2.HP = 0;
		isGameRun = false;
		return true;
	}
	return false;
}

bool Game::Draw(HDC hdc) {
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 0, 255));

	RECT textR1{ 100, 10, 235, 30 };
	RECT textR2{ 750, 10, 885, 30 };
	WCHAR  buff[100];
	std::wstring sBuff = L"Player 1";
	//DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR1, DT_CENTER | DT_TOP);//wcslen(buff)

	wsprintf(buff, TEXT("%d"), player1.HP);
	sBuff = L"HP: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR1, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);

	wsprintf(buff, TEXT("%d"), player1.Money);
	sBuff = L"|  Coins: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR1, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

	///////////////////////////////////////////////////////////////////////////////////////////

	//sBuff = L"Player 2";
	//DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR2, DT_CENTER | DT_TOP);//wcslen(buff)

	wsprintf(buff, TEXT("%d"), player2.HP);
	sBuff = L"HP: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR2, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);

	wsprintf(buff, TEXT("%d"), player2.Money);
	sBuff = L"|  Coins: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR2, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

	///////////////////////////////////////////////////////////////////////////////////////////
	//wsprintf(buff, TEXT("%d"), player1.NumberOfCards);
	//TextOut(hdc, 10, 90, buff, lstrlen(buff));
	for (int i = 0; i < Player::NumberOfCards; i++) {
		player1.deck[i]->Draw(hdc, PLAYER1, i);
	}
	
	for (int i = 0; i < Player::NumberOfCards; i++) {
		player2.deck[i]->Draw(hdc, PLAYER2, i);
	}
	return true;
}

