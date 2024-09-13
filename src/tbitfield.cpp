// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <memory>
#include <exception>
#include <bitset>

static const int SIZE = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw exception("Invalid bitfield size");
    }
    BitLen = len;
    MemLen = (len + SIZE - 1) / SIZE;
    pMem = (TELEM*)malloc(MemLen * sizeof(TELEM));
    if (pMem == nullptr) {
        throw exception("No memory allocated");
    }
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = (TELEM*)malloc(MemLen * sizeof(TELEM));
    if (pMem == nullptr) {
        throw exception("No memory allocated");
    }
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    free(pMem);
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / SIZE;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return TELEM(1 << n);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw exception("Invalid bit index");
    }
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw exception("Invalid bit index");
    }
    if (pMem[GetMemIndex(n)])
        pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] ^ GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw exception("Invalid bit index");
    }
    if (pMem[GetMemIndex(n)] & GetMemMask(n)) {
        return 1;
    }
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf != this) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        free(pMem);
        pMem = (TELEM*)malloc(MemLen * sizeof(TELEM));
        if (pMem == nullptr) {
            throw exception("No memory allocated");
        }
    }
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int dif = memcmp(pMem, bf.pMem, MemLen * sizeof(TELEM));
    if (dif == 0 && BitLen == bf.BitLen) {
        return 1;
    }
    return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int dif = memcmp(pMem, bf.pMem, MemLen * sizeof(TELEM));
    if (dif == 0 && BitLen == bf.BitLen) {
        return 0;
    }
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField res(1);
    if (BitLen > bf.BitLen) {
        res = *this;
    }
    else {
        res = bf;
    }

    for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField res(1);
    if (BitLen > bf.BitLen) {
        res = *this;
    }
    else {
        res = bf;
    }
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    for (int i = min(MemLen, bf.MemLen); i < res.MemLen; i++) {
        res.pMem[i] = 0;
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }
    TELEM mask = 0;
    for (int i = 0; i < BitLen % SIZE; i++) {
        mask += GetMemMask(i);
    }
    res.pMem[MemLen - 1] = res.pMem[MemLen - 1] & mask;
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.MemLen - 1; i >= 0; i--) {
        ostr << bitset<SIZE>(bf.pMem[i]).to_string();
    }
    return ostr;
}
