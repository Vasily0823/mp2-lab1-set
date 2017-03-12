// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле


/*
ПУСТЬ BitInTELEM = 32. deg = 5

-------------------------------
def:
“Логический сдвиг двоичного числа эквивалентен умножению / делению на число, кратное степени двойки.
-------------------------------

Зачем нужен дег?
в тбитфилд.н
ф-я BitInTELEM. вычисляет сколько бит в телем 
дег=0

1. конструктор
если дег=0, то его считаем, чтобы считать один раз, потом он не ноль и не считаем
дег это степень. дег это в какой стпени два = BitInTELEM
т.е 
если BitInTELEM = 8,то 2^ 3 = 8 сл дег = 3 
если BitInTELEM = 6,то 2^ 4 = 8 сл дег = 4
далее вычисляем, сколько нужно ячеек ТЕЛЕМ для хранения битового поля:
пример кода:
#include "tbitfield.h"
int DlinaPola = 63;
TBitField NamePola(DlinaPola);
// написав строку выше, мы вызываем конструктор. что произойдет? -
мы хотим создать поле длиной DlinaPola = 63, те хоти поле из 63 битов, как его хранмить?
в одном ТЕЛЕМе мб только BitInTELEM битов, те 32, а мы хотим создать поле длиной 63 битаа. явно одной ячсейки ТЕЛЕМ мало.
Как посчитать, склько ячеек ТЕЛЕМ нужно:
MemLen = (len + (BitInTELEM - 1)) >> deg;

Len = 1;  MemLen = (1 + (32 - 1)) / 32 = 32 / 32=1 ячеек TELEM
( выделим 1 ячейку ТЕЛЕМ (анс инт), в ней мб сохранено 32 бита, а нам нужно только 1. 1 исп-ся, а другие ненужны.

…

Len = 32;  MemLen = (32 + (32 - 1)) / 32 = 63 / 32=1 ячеек TELEM
( выделим 1 ячейку ТЕЛЕМ (анс инт), в ней мб сохранено 32 бита, и нам нужно  32. Все ячейки исп-ся.

Len = 33;  MemLen = (33 + (32 - 1)) / 32 =  64/ 32=2 ячеек TELEM
( выделим 2 ячейку ТЕЛЕМ (анс инт), в ней мб сохранено 64 бита, и нам нужно  33. 
Т.е в первой ячейке ТЕЛЕМ ис-ся все биты (32), а во второй ячейке только 1 из 32.)

2. в методе 

int TBitField::GetMemIndex(const int n) const // индекс Мемlen для бита n,
{
return n >> deg;    деление на 32
}

Что это такое? Передаем в метод число n - узнать в какой по счету ячейке ТЕЛЕМ лежит n-ый бит  
|....| |n...| |....| |....|
|0-я ячейка| |1-я ячейка| |2-я ячейка|
в конструкторе индексация идет с 0, фор и = 0
ответ - бит н лежит в ячейке 1.

*/
#include "tbitfield.h"

/*^*/ TBitField::TBitField(int len) {
	
	if (len<=0) {
		throw invalid_argument("Len<0...");
	}
	if (deg == 0)// Чтобы считать степень один раз, а не при каждом создании объекта
	{
		if (BitInTELEM == 8)  deg = 3; 
		if (BitInTELEM == 16) deg = 4;
		if (BitInTELEM == 32) deg = 5;
		if (BitInTELEM == 64) deg = 6;
		if (BitInTELEM == 128) deg = 7;
	}
	MemLen = (len + (BitInTELEM - 1)) >> deg; // Считаем кол-во ячеек памяти, нужных для хранения поля/ делим на 32
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

/* конструктор копирования
Аргументы MemLen и BitLen из private секции класса инициализируются полученными от объекта 
значениями  bf.MemLen и bf.BitLen соответственно. Затем выделяется MemLen ячеек TELEM памяти, и каждый бит 
ячейки инициализируется нулем за счет присваивания pMem[i] = 0 для i={1,…,MemLen}
*/

TBitField::~TBitField() {
	if (pMem != NULL) {
		delete[]pMem;
		pMem = NULL;
	}
}

/*
Если указатель на что-то указывает, то удаляем то, на что указывает указатель и стираем адресс, на который указывает указатель
если- ято не так, то на экран сообщение
*/

int TBitField::GetMemIndex(const int n) const // индекс Мемlen для бита n, 
{
	return n >> deg; // 2^deg = BitInTelem// просто деление на два в степени дег 
}

/*
Метод возвращает целую часть от деления n на количество бит в ячейке памяти TELEM.
*/

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (BitInTELEM - 1));// 00001000  
}

/*
«Битовая маска» — число, у которого в соответствующих битовому полю разрядах 
установлены единицы, а в остальных разрядах — нули.”
Другими словами при значении BitInTELEM=32:
При n=0  1 << (0 & (32 - 1)) =1 << (0 & 31) =1<<0=00001
При n=1  1 << (1 & (32 – 1)) =1 << (1 & 31) =1<<1=00010
При n=2  1 << (2 & (32 – 1)) =1 << (2 & 31) =1<<2=00100
При n=3  1 << (3 & (32 – 1)) =1 << (3 & 31) =1<<3=01000
При n=4  1 << (4 & (32 – 1)) =1 << (4 & 31) =1<<4=10000

*/

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину битового поля 
{
	return BitLen;
}
/*Метод возвращает длину битового поля BitLen.*/

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

/*
“Для установки битов выполняют логическое сложение (операцию «битовое ИЛИ») числа x с маской, 
у которой в позициях, соответствующих битовому полю, установлены единицы. Пример установки бита однобитового поля a: x1 = x | 00000001b”
При попытке установить биту с отрицательным индексом значение, срабатывает исключение:
throw invalid_argument("Len<0...")
При попытке установить значение биту с индексом большим, чем битов в поле, срабатывает исключение:
throw invalid_argument("BitLen < n...")
Для бита n с помощью GetMemIndex(n), метод находит индекс элемента TELEM. Для бита n с помощью GetMemMask(n), 
метод находит необходимую маску. После чего на найденный элемент TELEM 
и найденную маску метод воздействует логической операцией “ИЛИ”
*/

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

/*
... найденную маску метод воздействует логической операцией “И ”
*/

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
/*
При попытке получить бит с отрицательным индексом, срабатывает исключение:
throw invalid_argument("Len<0...")
При попытке получить бит с индексом большим, чем битов в поле, срабатывает исключение:
throw invalid_argument("BitLen < n...")
Метод получает значение бита следующим образом:

Для бита n с помощью GetMemIndex(n), метод находит индекс элемента TELEM. 
Для бита n с помощью GetMemMask(n), метод находит необходимую маску. 
После чего на найденный элемент TELEM и найденную маску метод воздействует логической операцией “И ”, 
если в результат воздействия логической операцией “И ” – 1, то метод возвращает единицу, в противном случае 
метод возвращает 0.
*/
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание перегрузка = 
{
	if (pMem != bf.pMem) 
	{
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
/*
В случае неравенства информации, лежащей в ячейке памяти pMem и bf.pMem, метод инициализирует
BitLen и MemLen значениями объекта bf.BitLen и bf.MemLen соответственно. Затем память, на которую указывает указатель 
pMem, освобождаются. После чего выделяется MemLen ячеек TELEM памяти и указатель pMem указывает на нее. 
Затем битовое поле объекта копируется в память, на которую указывает pMem.
*/

int TBitField::operator==(const TBitField &bf) const // сравнение перегрузка 
{
	int result = 1;
	if (BitLen != bf.BitLen) result = 0;
	else
	for (int i = 0; i < MemLen; i++)
	if (pMem[i] != bf.pMem[i]) 
	{ 
		result = 0; break; 
	}
	return result;
}

/*
Метод объявляет и инициализирует единицей переменную result. Если битовые поля имею разную длину,
то переменная result инициализируется нулем, и это значение возвращается методом. 
Если битовые поля имеют одинаковую длину, то поля сравниваются поэлементно с помощью метода GetBit, 
и, в случае неравенства элемента GetBit(i) элементу bf.GetBit(i), переменная result инициализируется нулем. 
По завершению проверки метод возвращает переменную result.
*/

int TBitField::operator!=(const TBitField &bf) const // сравнение перегрузка. отрицание метода выше
{
	if (*this == bf) 
		return 0;
	else 
		return 1;
}

/*отрицание того, что выше */

TBitField TBitField::operator|(const TBitField &bf) // операция "или" перегрузка
{
	int tmpBitLen;//tmpBitLen хранит максимальную длину поля
	int maxMem;//minMem хранит максимальное количество ячеек TELEM
	int n = 0;//счетчик
	if (BitLen >= bf.BitLen) // определяется наибольшая длина BitLen
	{
		tmpBitLen = BitLen;
		maxMem = MemLen;
	}
	else
	{
		tmpBitLen = bf.BitLen;//tmpBitLen хранит максимальную длину поля
		maxMem = bf.MemLen;//minMem хранит максимальное количество ячеек TELEM
	}
	TBitField tmp(tmpBitLen);
	while (n<maxMem) {
		tmp.pMem[n] = pMem[n] | bf.pMem[n];// в Си есть операция | для битов.
		n++;
	}
	return tmp;
}

/* 
Метод объявляет переменные int tmpBitLen (хранит длину максимальную поля), int minMem (хранит количество ячеек памяти TELEM, 
необходимое для хранения битового поля), int n (счетчик)  и переменную n инициализирует 0.  Затем метод определяет поле с 
наибольшей длиной и этим значением инициализирует переменную tmpBitLen, а переменную minMem инициализирует значением, 
равным количеству ячеек TELEM, необходимых для хранения этого битового поля. Затем метод объявляет битовое поле tmp длиной 
tmpBitLen. Поле tmp инициализируется битовым полем, которое получено в результате воздействия логической операцией “ИЛИ” на
битовые поля pMem и bf.pMem.
*/

TBitField TBitField::operator&(const TBitField &bf) // операция "и" перегрузка.
{
	int tmpBitLen;//tmpBitLen хранит максимальную длину поля
	int maxMem;//minMem хранит максимальное количество ячеек TELEM
	int n = 0;//счетчик
	if (BitLen >= bf.BitLen)
	{
		tmpBitLen = BitLen;
		maxMem = MemLen;
	}
	else
	{
		tmpBitLen = bf.BitLen;//tmpBitLen хранит максимальную длину поля
		maxMem = bf.MemLen;//minMem хранит максимальное количество ячеек TELEM
	}
	TBitField tmp(tmpBitLen);
	while (n<maxMem) {
		tmp.pMem[n] = pMem[n] & bf.pMem[n];// в Си есть операция & для битов. мы реализуем операцию & для битовых полей
		n++;
	}
	return tmp;
}

/* так же, как выше, только вместо | испуользуется & */

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
/*
Метод объявляет битовое поле tmp длиной BitLen. Поле tmp инициализируется битовым полем, которое получено 
в результате воздействия логической операцией “отрицание” на битовое поле pMem следующим образом: 
если бит поля pMem равен 0, то его значение будет рано 1 (операция tmp.SetBit(i)
если бит поля pMem равен 1, то его значение будет рано 0 (операция tmp.ClrBit(i);)
*/

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод перегрузка
{
	char c;
	cout<<"__Input bietfield__\n"
	<<"lenght of bietfield = %i\n" <<endl<< bf.BitLen;
	for (int i = 0; i < bf.BitLen; i++)
	{
		cout << "bit(%i) = " << i;
		istr >> c;
		if (c == '1') bf.SetBit(i); else bf.ClrBit(i);
	}
	cout << "end.\n\n";
	return istr;

}

/*
Метод объявляет переменную char c и информирует пользователя о начале ввода битового поля (bf) и о количестве битов 
в нем (bf.BitLen). После чего метод считывает нажатия пользователя. Если пользователь ввел единицу, то текущему биту
будет присвоено значение единица (с помощью метода bf.SetBit), в противном случае биту будет присвоено значение 
оль(с помощью метода bf.ClrBit).
*/

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод перегрузка
{
	ostr << "__Print bietfield__" << endl;
	ostr << "lenght of bietfield = " << bf.BitLen << endl;
	for (int i = 0; i < bf.BitLen; i++)
		ostr << "bit(" << i << ")= " << bf.GetBit(i) << endl;
	ostr << "end." << endl << endl;
	return ostr;
}
/*
Метод информирует пользователя о начале вывода битового поля (bf) и о количестве битов в нем (bf.BitLen). После чего 
метод поочередно с новой строки выводи каждый бит битового поля (с помощью метода bf.GetBit).
*/