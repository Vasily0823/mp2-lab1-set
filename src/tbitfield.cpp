// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) {
	
	if (len<=0) {
		throw invalid_argument("Len<0...");
	}
	if (deg == 0)// Чтобы считать степень один раз, а не при каждом создании объекта
	{
		if (BitInTELEM == 8)  deg = 3; //BitInTELEM - функция , описанная в заголов файле
		if (BitInTELEM == 16) deg = 4;
		if (BitInTELEM == 32) deg = 5;
		if (BitInTELEM == 64) deg = 6;
		if (BitInTELEM == 128) deg = 7;
	}
	MemLen = (len + (BitInTELEM - 1)) >> deg; // Считаем кол-во ячеек памяти, нужных для хранения поля
	BitLen = len; // записываем переданную длину в свойства объекта
	pMem = new TELEM[MemLen]; 
	if (pMem != NULL)
	for (int i = 0; i < MemLen; i++)	// зануляем
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)	
	for (int i = 0; i < MemLen; i++)	
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() {
	if (pMem != NULL) {
		delete[]pMem;
		pMem = NULL;
	}
	if (pMem) {
		std::cout << "Error clean " << pMem << std::endl;
	}
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n, 
{
	return n >> deg; // 2^deg = BitInTelem// просто деление на два дег раз
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (BitInTELEM - 1));// 00001000  
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину битового поля 
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит в 1
{
	if (n<0) {
		throw invalid_argument("Bit < 0...");
	}
	if (n>=BitLen) {
		throw invalid_argument("BitLen < n...");
	}
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] |= GetMemMask(n);// если 0 то 1, если 1 то 1. // не ко всему полю, а к участку pMem[GetMemIndex(n)]
}

void TBitField::ClrBit(const int n) // очистить бит в 0 
{
	if (n<0) {
		throw invalid_argument("Bit < 0...");
	}
	if (n >= BitLen) {
		throw invalid_argument("BitLen < n...");
	}
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] &= ~GetMemMask(n); // если 0 то 0, если 1 то 0. // не ко всему полю, а к участку pMem[GetMemIndex(n)]
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0) {
		throw invalid_argument("n < 0...");
	}
	if (n >= BitLen) {
		throw invalid_argument("BitLen < n...");
	}
	if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == 0) return 0;// если получилитсь все нуцли, то это ноль int. Если где-то 1, то это не 0 int, а какое-то число т.е в этом бите 1
	else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание перегрузка = 
{
	if (pMem != bf.pMem) {
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[]pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение перегрузка 
{
	int result = 1;
	if (BitLen != bf.BitLen) result = 0;
	else
	for (int i = 0; i < MemLen; i++)
	if (pMem[i] != bf.pMem[i]) { result = 0; break; }
	return result;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение перегрузка. отрицание метода выше
{
	if (*this == bf) return 0; else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или" перегрузка
{
	int tmpBitLen;//tmpBitLen хранит максимальную длину поля
	int minMem;//minMem хранит максимальное количество ячеек TELEM
	int n = 0;//счетчик
	if (BitLen >= bf.BitLen) // определяется наибольшая длина Битлен и мемлен
	{
		tmpBitLen = BitLen;
		minMem = MemLen;
	}
	else
	{
		tmpBitLen = bf.BitLen;//tmpBitLen хранит максимальную длину поля
		minMem = bf.MemLen;//minMem хранит максимальное количество ячеек TELEM
	}
	TBitField tmp(tmpBitLen);
	while (n<minMem) {
		tmp.pMem[n] = pMem[n] | bf.pMem[n];// в Си есть операция | для битов. мы реализуем операцию | для битовых полей
		n++;
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и" перегрузка как выще
{
	int tmpBitLen;//tmpBitLen хранит максимальную длину поля
	int minMem;//minMem хранит максимальное количество ячеек TELEM
	int n = 0;//счетчик
	if (BitLen >= bf.BitLen)
	{
		tmpBitLen = BitLen;
		minMem = MemLen;
	}
	else
	{
		tmpBitLen = bf.BitLen;//tmpBitLen хранит максимальную длину поля
		minMem = bf.MemLen;//minMem хранит максимальное количество ячеек TELEM
	}
	TBitField tmp(tmpBitLen);
	while (n<minMem) {
		tmp.pMem[n] = pMem[n] & bf.pMem[n];// в Си есть операция & для битов. мы реализуем операцию & для битовых полей
		n++;
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание было 00 стало 11. побитово 
{

		TBitField tmp(BitLen);
		for (int i = 0; i < BitLen; i++)
		{
			if (GetBit(i) == 1) tmp.ClrBit(i);
			else tmp.SetBit(i);
		}
		return tmp;
		
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод перегрузка
{
	char c;
	printf("__Input bietfield__\n");
	printf("lenght of bietfield = %i\n", bf.BitLen);
	for (int i = 0; i < bf.BitLen; i++)
	{
		printf("bit(%i) = ", i);
		istr >> c;
		if (c == '1') bf.SetBit(i); else bf.ClrBit(i);
	}
	printf("end.\n\n");
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод перегрузка
{
	ostr << "__Print bietfield__" << endl;
	ostr << "lenght of bietfield = " << bf.BitLen << endl;
	for (int i = 0; i < bf.BitLen; i++)
		ostr << "bit(" << i << ")= " << bf.GetBit(i) << endl;
	ostr << "end." << endl << endl;
	return ostr;
}
