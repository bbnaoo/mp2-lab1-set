// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "iomanip"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField() {
    MemLen = 0;
    pMem = nullptr;
}

TBitField::TBitField(int len)
{
    
    if (len == 31) {
        MemLen = 1;
    }
    else {
        BitLen = len;
        MemLen = ((len - 1) >> 5) + 1;
        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
}
TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}
TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return  1 << (n & 31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n > BitLen)) {
        throw n;
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen)) {
        throw n;
    }
    pMem[GetMemIndex(n)] &= GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) {
        throw n;
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (*this == bf) {
        return *this;
    }

    this->MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
  }
    else
    {
        for (int i = 0; i < MemLen; i++) {
            TELEM memMask = 0;
            if (i < MemLen - 1)
            {
                memMask = 0xffffffff;
            }
            else{
                memMask = (1 << i) << (BitLen % 32) - 1;
            }  
            if ((memMask & bf.pMem[i]) != (memMask & pMem[i])) {
                return 0;
            }
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = BitLen;
    if (bf.BitLen) {
        len = bf.BitLen;
    }
    TBitField tmp(len);
    for (int i = 0; i < MemLen; i++) {
        tmp.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        tmp.pMem[i] |= bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = BitLen;
    if (bf.BitLen > len) {
        len = bf.BitLen;
    }
    TBitField tmp(len);
    for (int i = 0; i < MemLen; i++) {
        tmp.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        tmp.pMem[i] &= bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    int len = BitLen;
    TBitField tmp(len);
    for (int i = 0; i < MemLen; ++i) {
        tmp.pMem[i] = ~pMem[i];
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char ch;
    while (1) {
        istr >> ch;
        if (ch == '0') {
            bf.ClrBit(i++);
        }
        else {
            if (ch == '1') {
                bf.SetBit(i++);
            }
            else
                break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    int len = bf.BitLen;

    for (int i = 0; i < len; i++) {
        if (bf.GetBit(i)) {
            ostr << "1";
        }
        else {
            ostr << "0";
        }
    }

    return ostr;
}
