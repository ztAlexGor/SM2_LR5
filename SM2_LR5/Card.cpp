#include "Card.h"

Card::Card(int cost, int type, HBITMAP* hImage, BITMAP* image) {
	this->cost = cost;
	this->type = type;
	this->hImage = hImage;
	this->image = image;
}

int Card::GetCost(){
	return cost;
}

int Card::GetType(){
	return type;
}

//////////////////////////////////////////////////////////////////////////////////////

AttackCard::AttackCard(int p, int c, HBITMAP* hIm, BITMAP* im) : Card(c, Attack, hIm, im) {
	this->AttackPower = p;
}

void AttackCard::Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney){
	enemyHP -= this->AttackPower;
	myMoney -= this->cost;
}

bool AttackCard::Draw(HDC hdc, int player, int cardNumber){
	HDC hLocalDC = CreateCompatibleDC(hdc);
	if (hLocalDC == NULL) {
		MessageBox(NULL, L"CreateCompatibleDC Failed", L"Error", MB_OK);
		return false;
	}

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hLocalDC, *hImage);
	if (hOldBitmap == NULL) {
		MessageBox(NULL, L"SelectObject Failed", L"Error", MB_OK);
		return false;
	}

	// Blit the dc which holds the bitmap onto the window's dc
	BOOL qRetBlit = BitBlt(hdc, 10 + player * 1120, 150 + cardNumber * 300, image->bmWidth, image->bmHeight, hLocalDC, 0, 0, SRCCOPY);
	if (!qRetBlit) {
		MessageBox(NULL, L"Blit Failed", L"Error", MB_OK);
		return false;
	}

	// Unitialize and deallocate resources
	SelectObject(hLocalDC, hOldBitmap);
	DeleteDC(hLocalDC);
	DeleteObject(hOldBitmap);

	RECT textR{ 10 + player * 1120, 150 + cardNumber * 300, 150 + player * 1120, 200 + cardNumber * 300 };
	WCHAR  buff[100];
	std::wstring sBuff = L"Attack";

	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_CENTER | DT_TOP);//wcslen(buff)

	wsprintf(buff, TEXT("%d"), this->AttackPower);
	sBuff = L"Power: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);

	wsprintf(buff, TEXT("%d"), this->cost);
	sBuff = L"Coins: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);










	return true;
}

//////////////////////////////////////////////////////////////////////////////////////

DeffenceCard::DeffenceCard(int p, int c, HBITMAP* hIm, BITMAP* im) :Card(c, Deffence, hIm, im) {
	this->DeffencePower = p;
}

void DeffenceCard::Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney) {
	myHP += this->DeffencePower;
	myMoney -= this->cost;
}

bool DeffenceCard::Draw(HDC hdc, int player, int cardNumber) {
	RECT textR{ 100 + player * 650, 250 + cardNumber * 300, 250 + player * 650, 300 + cardNumber * 300 };
	WCHAR  buff[100];
	std::wstring sBuff = L"Deffence";

	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_CENTER | DT_TOP);//wcslen(buff)

	wsprintf(buff, TEXT("%d"), this->DeffencePower);
	sBuff = L"Power: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);

	wsprintf(buff, TEXT("%d"), this->cost);
	sBuff = L"Coins: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////

MonetaryCard::MonetaryCard(int p, int c, HBITMAP* hIm, BITMAP* im) :Card(c, Monetary, hIm, im) {
	this->MonetaryPower = p;
}

void MonetaryCard::Use(int& myHP, int& enemyHP, int& myMoney, int& enemyMoney) {
	myMoney += this->MonetaryPower;
}

bool MonetaryCard::Draw(HDC hdc, int player, int cardNumber) {
	
	RECT textR{ 100 + player * 650, 250 + cardNumber * 300, 250 + player * 650, 300 + cardNumber * 300 };
	WCHAR  buff[100];
	std::wstring sBuff = L"Monetary";

	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_CENTER | DT_TOP);//wcslen(buff)

	wsprintf(buff, TEXT("%d"), this->MonetaryPower);
	sBuff = L"Power: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);

	wsprintf(buff, TEXT("%d"), this->cost);
	sBuff = L"Coins: ";
	sBuff += buff;
	DrawTextW(hdc, sBuff.c_str(), sBuff.size(), &textR, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
	return true;
}

