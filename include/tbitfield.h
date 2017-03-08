// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#ifndef __BITFIELD_H__
#define __BITFIELD_H__

#include <iostream>

using namespace std;

typedef unsigned int TELEM;

class TBitField
{
private:
  int  BitLen; // длина битового поля 
  TELEM *pMem; // память для представления битового поля
  int  MemLen; // к-во эл-тов памти
 
 //-------
  unsigned int BitInTELEM = sizeof(TELEM)* 8; // бит в ячейке памяти TELEM на текущей аппаратной машине
  int deg = 0;//BitInTELEM - это два в степени deg = BitInTELEM на текущей аппаратной машине
  //-------

  // методы реализации
  int   GetMemIndex(const int n) const; // индекс в pМем для бита n       (#О2)
  TELEM GetMemMask (const int n) const; // битовая маска для бита n       (#О3)
public:
  TBitField(int len);                // Конструктор                                  (#О1)
  TBitField(const TBitField &bf);    //    Конструктор копирования                               (#П1)
  ~TBitField();                      //       деструктор                             (#С)

  // доступ к битам
  int GetLength(void) const;      // получить длину поля         (#О)
  void SetBit(const int n);       // установить бит в 1                     (#О4)
  void ClrBit(const int n);       // очистить бит в 0                          (#П2)
  int  GetBit(const int n) const; // получить значение бита N              (#Л1)

  // битовые операции
  int operator==(const TBitField &bf) const; // сравнение                 (#О5)
  int operator!=(const TBitField &bf) const; // сравнение
  TBitField& operator=(const TBitField &bf); // присваивание              (#П3)
  TBitField  operator|(const TBitField &bf); // операция "или"            (#О6)
  TBitField  operator&(const TBitField &bf); // операция "и"              (#Л2)
  TBitField  operator~(void);                // отрицание                  (#С)

  friend istream &operator>>(istream &istr, TBitField &bf);       //      (#О7)
  friend ostream &operator<<(ostream &ostr, const TBitField &bf); //      (#П4)
};
// Структура хранения битового поля
//   бит.поле - набор битов с номерами от 0 до BitLen
//   массив pМем рассматривается как последовательность MemLen элементов
//   биты в эл-тах pМем нумеруются справа налево (от младших к старшим)
// О8 Л2 П4 С2

#endif
