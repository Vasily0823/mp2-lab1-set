// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) :BitField(mp)//конструктор . список инициализации
{
	MaxPower = mp;
}

TSet::TSet(const TSet &s) : BitField(s.BitField)// конструктор копирования
{
	MaxPower = s.MaxPower;
}

TSet::TSet(const TBitField &bf) : BitField(bf)// конструктор преобразования типа
{
	MaxPower = bf.GetLength();
}

TSet::operator TBitField() // оператор преобразования к TBitField/ получить битфилд
{
	TBitField result(BitField);
	return result;
}

int TSet::GetMaxPower(void) const // Максимальная мощность множества/получить макс. к-во эл-тов. получить мощность
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // Проверить наличие элемента в множестве/элемент множества?.. получить значение бита
{
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // Включить элемент в множество . в 1
{
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) //Удалить элемент из множества . в 0
{
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание перегрузка
{
	BitField = s.BitField;
	MaxPower = s.GetMaxPower();
	return *this;
}

int TSet::operator==(const TSet &s) const // сравнение перегрузка
{
	return BitField == s.BitField; //перегружено в битфилд
}

int TSet::operator!=(const TSet &s) const // сравнение
{
	return BitField != s.BitField; //перегружено в битфилд
}

TSet TSet::operator+(const TSet &s) // объединение со всем полем
{
	TSet result(BitField | s.BitField); //перегружено в битфилд
	return result;
}

TSet TSet::operator+(const int Elem) // объединение с одним элементом элметом
{
	if (Elem > BitField.GetLength())
	{
		throw ("Elem > Length");
	}
	TBitField temp(MaxPower);
	temp.SetBit(Elem);
	TSet result(BitField | temp); //перегружено в битфилд
	return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	if (Elem > BitField.GetLength())
	{
		throw ("Elem > Length");
	}
	TBitField temp(MaxPower);
	temp.SetBit(Elem);
	TSet result(BitField & temp); //перегружено в битфилд
	return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
	TSet result(BitField & s.BitField);
	return result;
}

TSet TSet::operator~(void) // дополнение .. инфверсия
{
	TSet result(~BitField); //перегружено в битфилд
	return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод {s1,s2,s3}
{
	char c;
	printf("__Input set\n");
	printf("lenght of set = %i\n", s.GetMaxPower());
	for (int i = 0; i < s.GetMaxPower(); i++)
	{
		printf("set(%i) = ", i);
		istr >> c;
		if (c == '1') s.InsElem(i); else s.DelElem(i);
	}
	printf("end.\n\n");
	return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
	ostr << "__Print set" << endl;
	ostr << "lenght of set = " << s.GetMaxPower() << endl;
	for (int i = 0; i < s.GetMaxPower(); i++)
		ostr << "set(" << i << ")= " << s.IsMember(i) << endl;
	ostr << "end." << endl << endl;
	return ostr;
}
