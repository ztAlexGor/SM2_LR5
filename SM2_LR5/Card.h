#pragma once
#include <windows.h>
#include <string>
#include "Resource.h"
const int TYPE_OF_CARDS_NUMBER = 3;
const int ATTACK_CARDS_NUMBER = 3;
const int DEFFENSE_CARDS_NUMBER = 3;
const int MONETARY_CARDS_NUMBER = 3;
enum typeOfCards { Attack = 0, Deffence, Monetary };


class Card{
public:
	HBITMAP* hImage;
	BITMAP* image;
	int type;
	int cost;
	Card(int cost, int type, HBITMAP* hImage, BITMAP* image);
	int GetCost();
	int GetType();
	virtual void Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney) = 0;
	virtual bool Draw(HDC hdc, int player, int cardNumber) = 0;
};



class AttackCard : public Card{
	int AttackPower;
public:
	AttackCard(int p, int c, HBITMAP* hIm, BITMAP* im);
	void Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney) override;
	bool Draw(HDC hdc, int player, int cardNumber) override;
private:

};

class DeffenceCard : public Card {
	int DeffencePower;
public:
	DeffenceCard(int p, int c, HBITMAP* hIm, BITMAP* im);
	void Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney) override;
	bool Draw(HDC hdc, int player, int cardNumber) override;
private:

};

class MonetaryCard : public Card {
	int MonetaryPower;
public:
	MonetaryCard(int p, int c, HBITMAP* hIm, BITMAP* im);
	void Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney) override;
	bool Draw(HDC hdc, int player, int cardNumber) override;
private:

};