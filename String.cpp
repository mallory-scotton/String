///////////////////////////////////////////////////////////////////////////////
///
/// MIT License
///
/// Copyright(c) 2024 Mallory SCOTTON
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following coditions:
///
/// The above copyright notice and this permission notice shall be included
/// in all copies or substantial portions of the Software?
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////////
#include "String.hpp"

///////////////////////////////////////////////////////////////////////////////
// Namespace Ax
///////////////////////////////////////////////////////////////////////////////
namespace Ax
{

///////////////////////////////////////////////////////////////////////////////
using sizeType = size_t;

///////////////////////////////////////////////////////////////////////////////
TString::TString(void)
{
    _setCapacity(0);
    _setLength(0);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(const TString& Other)
{
    _append(Other._str, Other._strLen);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(const TString& Other, sizeType Pos, size_t Len)
{
    Append(Other._str, Pos, Len);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(TString&& Other)
{
    *this = std::move(Other);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(const char* Other)
{
    _append(Other);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(const char* Other, size_t Len)
{
    Append(Other, 0, Len);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(size_t Len, char Filler)
{
    Append(Len, Filler);
}

///////////////////////////////////////////////////////////////////////////////
TString::TString(const ConstIterator First, const ConstIterator Second)
{
    Append(First, Second);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::operator=(const TString& Other)
{
    if (this != &Other)
    {
        _setLength(0);
        _append(Other._str, Other._strLen);
    }
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::operator=(TString&& Other)
{
    if (this != &Other)
    {
        SafeDeleteArray(_str);
        _str = Other._str;
        _strLen = Other._strLen;
        _strCap = Other._strCap;
        Other._str = nullptr;
        Other._strLen = 0;
        Other._strCap = 0;
    }
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::~TString(void)
{
    SafeDeleteArray(_str);
    _strLen = 0;
    _strCap = 0;
    _increaseBy = 0;
}

///////////////////////////////////////////////////////////////////////////////
bool operator==(const TString& Lhs, const TString& Rhs)
{
    return (Lhs._compare(Rhs) == 0);
}

///////////////////////////////////////////////////////////////////////////////
bool operator!=(const TString& Lhs, const TString& Rhs)
{
    return (Lhs._compare(Rhs) != 0);
}

///////////////////////////////////////////////////////////////////////////////
bool operator<(const TString& Lhs, const TString& Rhs)
{
    return (Lhs._compare(Rhs) < 0);
}

///////////////////////////////////////////////////////////////////////////////
bool operator>(const TString& Lhs, const TString& Rhs)
{
    return (Lhs._compare(Rhs) > 0);
}

///////////////////////////////////////////////////////////////////////////////
bool operator<=(const TString& Lhs, const TString& Rhs)
{
    return (Lhs._compare(Rhs) <= 0);
}

///////////////////////////////////////////////////////////////////////////////
bool operator>=(const TString& Lhs, const TString& Rhs)
{
    return (Lhs._compare(Rhs) >= 0);
}

///////////////////////////////////////////////////////////////////////////////
int TString::_compare(const TString& Rhs) const
{
    if (_strLen < Rhs._strLen)
        return (1);
    else if (_strLen > Rhs._strLen)
        return (-1);
    return (::strcmp(_str, Rhs._str));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::operator[](sizeType Index) const
{
    return (*(_str + Index));
}

///////////////////////////////////////////////////////////////////////////////
char& TString::operator[](sizeType Index)
{
    return (*(_str + Index));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::At(sizeType Index) const
{
    if (_strLen <= Index)
        throw;
    return (operator[](Index));
}

///////////////////////////////////////////////////////////////////////////////
char& TString::At(sizeType Index)
{
    if (_strLen <= Index)
        throw;
    return (operator[](Index));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::Front(void) const
{
    return (operator[](0));
}

///////////////////////////////////////////////////////////////////////////////
char& TString::Front(void)
{
    return (operator[](0));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::Back(void) const
{
    return (operator[](_strLen - 1));
}

///////////////////////////////////////////////////////////////////////////////
char& TString::Back(void)
{
    return (operator[](_strLen - 1));
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::operator+=(const TString& Other)
{
    _append(Other._str, Other._strLen);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::operator+=(const char* Other)
{
    _append(Other);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::operator+=(char Ch)
{
    _append(&Ch, 1);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Append(const TString& Str)
{
    _append(Str._str, Str._strLen);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Append(const TString& Str, sizeType SubPos, size_t SubLen)
{
    char* Buffer = nullptr;

    SubLen = _getLength(Str, SubPos, SubLen);
    _substr(Buffer, Str._str, SubPos, SubLen);
    _append(Buffer, SubLen);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Append(const char* Str)
{
    _append(Str);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Append(const char* Str, size_t Len)
{
    char* Buffer = nullptr;

    _substr(Buffer, Str, 0, Len);
    _append(Buffer, Len);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Append(sizeType Len, char Filler)
{
    char* Buffer = nullptr;

    _allocCString(Buffer, Len, Filler);
    _append(Buffer, Len);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Append(const ConstIterator First, const ConstIterator Second)
{
    const size_t Len = _getLength(First, Second);

    if (Len != 0)
    {
        char* Buffer = nullptr;
        _allocCString(Buffer, Len, First, Second);
        _append(Buffer, Len);
        SafeDelete(Buffer);
    }
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::PushBack(char Ch)
{
    _append(&Ch, 1);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Insert(sizeType Pos, const TString& Other)
{
    _insertstr(Pos, Other._str, Other._strLen);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Insert(sizeType Pos, const TString& Other, sizeType SubPos,
    size_t SubLen)
{
    char* Buffer = nullptr;

    SubLen = _getLength(Other, SubPos, SubLen);
    _substr(Buffer, Other._str, SubPos, SubLen);
    _insertstr(Pos, Buffer, SubLen);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Insert(sizeType Pos, const char* Other)
{
    _insertstr(Pos, Other);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Insert(sizeType Pos, const char* Other, size_t Len)
{
    char* Buffer = nullptr;

    _substr(Buffer, Other, 0, Len);
    _insertstr(Pos, Buffer, Len);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Insert(sizeType Pos, size_t Len, char Filler)
{
    char* Buffer = nullptr;

    _allocCString(Buffer, Len, Filler);
    _insertstr(Pos, Buffer, Len);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
void TString::Insert(Iterator Ptr, size_t Len, char Ch)
{
    char* Buffer = nullptr;

    _allocCString(Buffer, Len, Ch);
    _insertstr(Ptr.current.pos, Buffer, Len);
    SafeDelete(Buffer);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Insert(Iterator Ptr, char Ch)
{
    _insertstr(Ptr.current.pos, &Ch, 1);
    return (Ptr);
}

///////////////////////////////////////////////////////////////////////////////
void TString::Insert(Iterator Ptr, const ConstIterator First,
    const ConstIterator Second)
{
    const size_t Len = _getLength(First, Second);

    if (Len == 0)
    {
        return;
    }
    char* Buffer = nullptr;
    _allocCString(Buffer, Len, First, Second);
    _insertstr(Ptr.current.pos, Buffer, Len);
    SafeDelete(Buffer);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Erase(sizeType Pos, size_t Len)
{
    _erase(Pos, Len);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Erase(ConstIterator Ptr)
{
    _erase(Ptr.current.pos, 1);
    return (Ptr.current);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Erase(ConstIterator First, ConstIterator Last)
{
    _erase(First.current.pos, _getLength(First, Last));
    return (First.current);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(sizeType Pos, size_t Len, const TString& Other)
{
    _replace(Pos, Len, Other._str, Other._strLen);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(ConstIterator It1, ConstIterator It2,
    const TString& Other)
{
    Replace(It1.current.pos, _getLength(It1, It2), Other._str, Other._strLen);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(sizeType Pos, size_t Len, const TString& Other,
    sizeType SubPos, size_t SubLen)
{
    char* Buffer = nullptr;

    SubLen = _getLength(Other, SubPos, SubLen);
    _substr(Buffer, Other._str, SubPos, SubLen);
    _replace(Pos, Len, Buffer, ::strlen(Buffer));
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(sizeType Pos, size_t Len, const char* Str)
{
    _replace(Pos, Len, Str, ::strlen(Str));
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(ConstIterator It1, ConstIterator It2,
    const char* Other)
{
    _replace(It1.current.pos, _getLength(It1, It2), Other,
        ::strlen(Other));
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(sizeType Pos, size_t Len, const char* Other, size_t n)
{
    _replace(Pos, Len, Other, n);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(ConstIterator It1, ConstIterator It2,
    const char* Other, size_t n)
{
    _replace(It1.current.pos, _getLength(It1, It2), Other, n);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(sizeType Pos, size_t Len, size_t n, char Filler)
{
    char* Buffer = nullptr;

    _allocCString(Buffer, n, Filler);
    _replace(Pos, Len, Buffer, n);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(ConstIterator It1, ConstIterator It2, sizeType n,
    char Ch)
{
    char* Buffer = nullptr;

    _allocCString(Buffer, n, Ch);
    _replace(It1.current.pos, _getLength(It1, It2), Buffer, n);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Replace(ConstIterator It1, ConstIterator It2,
    ConstIterator First, ConstIterator Second)
{
    const size_t Len = _getLength(First, Second);
    char* Buffer = nullptr;

    _allocCString(Buffer, Len, First, Second);
    _replace(It1.current.pos, _getLength(It1, It2), Buffer, Len);
    SafeDelete(Buffer);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
void TString::Swap(TString& Other)
{
    TString temp = std::move(*this);
    *this = std::move(Other);
    Other = std::move(temp);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::PopBack(void)
{
    _erase((End() - 1).current.pos, 1);
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
inline void TString::_append(const char* Other)
{
    return (_append(Other, ::strlen(Other)));
}

///////////////////////////////////////////////////////////////////////////////
void TString::_append(const char* Other, size_t Len)
{
    if (!Other || Len == 0)
        return;
    _increaseCapacity(_strLen + Len);
    sizeType i = 0;
    for (; i < Len; ++i)
        operator[](_strLen + i) = Other[i];
    operator[](_strLen + i) = '\0';
    _setLength(_strLen + Len);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_insertstr(sizeType Pos, const char* Other)
{
    if (Other != nullptr)
        _insertstr(Pos, Other, ::strlen(Other));
}

///////////////////////////////////////////////////////////////////////////////
void TString::_insertstr(sizeType Pos, const char* Other, size_t Len)
{
    if (Pos == _strLen)
        return (_append(Other, Len));
    if (Pos > _strLen)
        throw;
    if (Len == 0)
        return;
    _increaseCapacity(_strLen + Len);
    if (_str)
    {
        char* Buffer = nullptr;
        _substr(Buffer, _str, Pos, _strLen);
        _clearStr(Pos);
        _append(Other, Len);
        _append(Buffer, ::strlen(Buffer));
        SafeDelete(Buffer);
    }
    else _append(Other, Len);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_substr(char*& Buffer, const char* Other, sizeType Pos,
    size_t Len) const
{
    if (Other == nullptr)
        throw;
    _allocCString(Buffer, Len);
    for (sizeType i = 0; i < _strLen; ++i)
    {
        Buffer[i] = Other[Pos + i];
    }
}

///////////////////////////////////////////////////////////////////////////////
void TString::_erase(sizeType Pos, size_t Len)
{
    Len = _getLength(*this, Pos, Len);
    for (size_t i = Pos + Len; i < _strLen; ++i)
    {
        operator[](i - Len) = operator[](i);
    }
    _clearStr(_strLen - Len);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_replace(sizeType Pos, size_t Len, const char* Other, size_t n)
{
    char* Buffer = nullptr;
    char* RBuffer = nullptr;

    Len = _getLength(*this, Pos, Len);
    _substr(Buffer, _str, Pos + Len, _strLen);
    _clearStr(Pos);
    _substr(RBuffer, Other, 0, n);
    _append(RBuffer);
    _append(Buffer);
    SafeDelete(Buffer);
    SafeDelete(RBuffer);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::_getLength(const TString& Str, sizeType Pos, size_t Len) const
{
    if (Len == npos)
        Len = _strLen - Pos;
    if (Pos + Len > Str._strLen)
        throw;
    return (Len);
}

///////////////////////////////////////////////////////////////////////////////
const char* TString::CStr(void) const
{
    return (_str);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Copy(char* Str, size_t Len, sizeType Pos) const
{
    if (!Str)
        throw;
    Len = _getLength(*this, Pos, Len);
    for (sizeType i = 0; i < Len; ++i)
        *(Str + i) = operator[](Pos + i);
    return (Len);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Find(const TString& Other, sizeType Pos) const
{
    return (_find(Other._str, Other._strLen, Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Find(const char* Other, sizeType Pos) const
{
    return (_find(Other, ::strlen(Other), Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Find(const char* Other, sizeType Pos, sizeType n) const
{
    return (_find(Other, n, Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Find(char Ch, sizeType Pos) const
{
    return (_find(&Ch, 1, Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::RFind(const TString& Other, sizeType Pos) const
{
    return (_rfind(Other._str, Other._strLen, Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::RFind(const char* Other, sizeType Pos) const
{
    return (_rfind(Other, ::strlen(Other), Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::RFind(const char* Other, sizeType Pos, size_t n) const
{
    return (_rfind(Other, n, Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::RFind(char Ch, sizeType Pos) const
{
    return (_rfind(&Ch, 1, Pos));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstOf(const TString& Other, sizeType Pos) const
{
    return (_findFirstOf(Other._str, Other._strLen, Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstOf(const char* Other, sizeType Pos) const
{
    return (_findFirstOf(Other, ::strlen(Other), Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstOf(const char* Other, sizeType Pos, size_t n) const
{
    return (_findFirstOf(Other, n, Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstOf(char Ch, sizeType Pos) const
{
    return (_findFirstOf(&Ch, 1, Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastOf(const TString& Other, sizeType Pos) const
{
    return (_findLastOf(Other._str, Other._strLen, Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastOf(const char* Other, sizeType Pos) const
{
    return (_findLastOf(Other, ::strlen(Other), Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastOf(const char* Other, sizeType Pos, size_t n) const
{
    return (_findLastOf(Other, n, Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastOf(char Ch, sizeType Pos) const
{
    return (_findLastOf(&Ch, 1, Pos, true));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstNotOf(const TString& Other, sizeType Pos) const
{
    return (_findFirstOf(Other._str, Other._strLen, Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstNotOf(const char* Other, sizeType Pos) const
{
    return (_findFirstOf(Other, ::strlen(Other), Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstNotOf(const char* Other, sizeType Pos, size_t n) const
{
    return (_findFirstOf(Other, n, Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindFirstNotOf(char Ch, sizeType Pos) const
{
    return (_findFirstOf(&Ch, 1, Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastNotOf(const TString& Other, sizeType Pos) const
{
    return (_findLastOf(Other._str, Other._strLen, Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastNotOf(const char* Other, sizeType Pos) const
{
    return (_findLastOf(Other, ::strlen(Other), Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastNotOf(const char* Other, sizeType Pos, size_t n) const
{
    return (_findLastOf(Other, n, Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::FindLastNotOf(char Ch, sizeType Pos) const
{
    return (_findLastOf(&Ch, 1, Pos, false));
}

///////////////////////////////////////////////////////////////////////////////
TString TString::SubStr(sizeType Pos, size_t Len) const
{
    char* Buffer = nullptr;

    Len = _getLength(*this, Pos, Len);
    _substr(Buffer, _str, Pos, Len);
    TString toReturn(Buffer);
    SafeDelete(Buffer);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::_find(const char* Other, size_t Len, sizeType Pos) const
{
    size_t toReturn = npos;

    if (Pos == npos)
        Pos = _strLen - 1;
    for(; Pos <= _strLen; ++Pos)
    {
        if (operator[](Pos) == *Other && _findCompare(Other, Len, Pos))
        {
            toReturn = Pos;
            break;
        }
    }
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::_rfind(const char* Other, size_t Len, sizeType Pos) const
{
    size_t toReturn = npos;

    if (Pos == npos || Pos + Len > _strLen + 1)
        Pos = _strLen - Len + 1;
    for (int i = (int)Pos; 0 <= i; --i)
    {
        if (operator[](i) == *Other && _findCompare(Other, Len, i))
        {
            toReturn = i;
            break;
        }
    }
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
bool TString::_findCompare(const char* Other, size_t Len, sizeType Pos) const
{
    for (sizeType i = 1; i < Len; ++i)
    {
        if (operator[](Pos + i) != *(Other + i))
        {
            return (false);
        }
    }
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::_findFirstOf(const char* Other, size_t Len, sizeType Pos,
    bool IsTrue) const
{
    size_t toReturn = npos;

    if (Pos == npos)
        Pos = _strLen - 1;
    for (; Pos < _strLen; ++Pos)
    {
        if (_findOfCompare(Other, Len, Pos, IsTrue))
        {
            toReturn = Pos;
            break;
        }
    }
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::_findLastOf(const char* Other, size_t Len, sizeType Pos,
    bool IsTrue) const
{
    size_t toReturn = npos;

    if (Pos == npos)
        Pos = _strLen - 1;
    for (int i = (int)Pos; 0 <= i; --i)
    {
        if (_findOfCompare(Other, Len, i, IsTrue))
        {
            toReturn = i;
            break;
        }
    }
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
bool TString::_findOfCompare(const char* Other, size_t Len, sizeType Pos,
    bool IsTrue) const
{
    for (sizeType i = 0; i < Len; ++i)
    {
        if (operator[](Pos) == *(Other + i))
        {
            return (IsTrue);
        }
    }
    return (!IsTrue);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Length(void) const
{
    return (_strLen);
}

///////////////////////////////////////////////////////////////////////////////
inline size_t TString::Size(void) const
{
    return (Length());
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::MaxSize(void) const
{
    return (npos - 1);
}

///////////////////////////////////////////////////////////////////////////////
void TString::Resize(size_t n)
{
    if (_strLen == n) return;
    if (_strLen < n)
    {
        _setLength(n);
        return;
    }
    _clearStr(n);
}

///////////////////////////////////////////////////////////////////////////////
void TString::Resize(size_t n, char Filler)
{
    if (_strLen == n) return;
    if (_strLen < n)
    {
        sizeType Pos = _strLen;
        _setLength(n);
        _fillStr(_str, _strLen, Pos, Filler);
        return;
    }
    _clearStr(n);
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::Capacity(void) const
{
    return (_strCap);
}

///////////////////////////////////////////////////////////////////////////////
void TString::Reserve(size_t n)
{
    if (_strCap == n) return;
    if (_strCap < n)
    {
        _setCapacity(n);
        return;
    }
    if (_strCap / 2 > _strLen)
    {
        _decreaseCapacity(_strCap / 2);
    }
}

///////////////////////////////////////////////////////////////////////////////
void TString::Clear(void)
{
    _clearStr(0);
}

///////////////////////////////////////////////////////////////////////////////
void TString::ShrinkToFit(void)
{
    _decreaseCapacity(_strLen);
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::ToLowerCase(void)
{
    return (_transform(::towlower));
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::ToUpperCase(void)
{
    return (_transform(::toupper));
}

///////////////////////////////////////////////////////////////////////////////
TString& TString::Trim(void)
{
    size_t start = 0;
    size_t end = _strLen - 1;

    for (; start < _strLen && ::isspace(_str[start]); start++);
    for (; end && ::isspace(_str[end]); end--);

    if (start > 0 && end < _strLen - 1)
    {
        size_t newSize = end - start + 1;
        char* newStr = new char[newSize + 1];
        for (size_t i = 0; i < newSize; i++)
        {
            newStr[i] = _str[start + i];
        }
        newStr[newSize] = '\0';
        delete[] _str;
        _str = newStr;
        _strLen = newSize;
    }

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
bool TString::IsEmpty(void) const
{
    return (_strLen ? false : true);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_setLength(const size_t Len)
{
    if (_strLen > Len)
        _clearStr(Len);
    else if (_strCap < Len)
        _increaseCapacity(Len);
    _strLen = Len;
}

///////////////////////////////////////////////////////////////////////////////
void TString::_setCapacity(const size_t Cap)
{
    if (_strCap == Cap && _str)
        return;
    if (Cap < _strLen)
        return;
    _strCap = Cap;

    char* Buffer = _str;
    _str = nullptr;
    _allocCString(_str, _strCap);
    if (Buffer)
    {
        for (sizeType i = 0; i < _strLen; ++i)
        {
            operator[](i) = Buffer[i];
        }
        operator[](_strLen) = '\0';
    }
    SafeDelete(Buffer);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_increaseCapacity(const size_t Cap)
{
    if (_strCap > Cap && _str)
        return;
    size_t n = _strCap;
    while (n <= Cap)
        n += _increaseBy;
    _increaseBy++;
    _setCapacity(n);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_decreaseCapacity(const size_t Cap)
{
    if (_strCap < Cap)
        return;
    if (_increaseBy > 15)
        _increaseBy--;
    _setCapacity(Cap);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_allocCString(char*& Buffer, const size_t n) const
{
    if (Buffer)
        throw;
    Buffer = new char[n + 1]();
}

///////////////////////////////////////////////////////////////////////////////
void TString::_allocCString(char*& Buffer, const size_t n, char Filler) const
{
    _allocCString(Buffer, n);
    _fillStr(Buffer, n, 0, Filler);
}

///////////////////////////////////////////////////////////////////////////////
void TString::_allocCString(char*& Buffer, const size_t n,
    const ConstIterator It1, const ConstIterator It2) const
{
    _allocCString(Buffer, n);
    ConstIterator Begin = It1;
    for (sizeType i = 0; i < n; i++)
        Buffer[i] = *Begin++;
}

///////////////////////////////////////////////////////////////////////////////
void TString::_fillStr(char* Other, const size_t Len, const sizeType Pos,
    char Ch) const
{
    sizeType Begin = Pos;

    while (Begin != Len)
        Other[Begin++] = Ch;
    Other[Begin] = '\0';
}

///////////////////////////////////////////////////////////////////////////////
void TString::_clearStr(const sizeType Pos)
{
    _fillStr(_str, _strLen, Pos, '\0');
    _strLen = Pos;
}

///////////////////////////////////////////////////////////////////////////////
template <typename F>
TString& TString::_transform(F Func)
{
    for (size_t i = 0; i < _strLen; i++)
    {
        _str[i] = Func(_str[i]);
    }
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Begin(void)
{
    return (_iBegin());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::Begin(void) const
{
    return (_iBegin());
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::End(void)
{
    return (_end());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::End(void) const
{
    return (_end());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::CBegin(void) const
{
    return (_iBegin());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::CEnd(void) const
{
    return (_end());
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator TString::RBegin(void)
{
    return (_rBegin());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::RBegin(void) const
{
    return (_rBegin());
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator TString::REnd(void)
{
    return (_end());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::REnd(void) const
{
    return (_end());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::CRBegin(void) const
{
    return (_rBegin());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::CREnd(void) const
{
    return (_end());
}

///////////////////////////////////////////////////////////////////////////////
inline TString::StringIteratorType TString::_iBegin(void) const
{
    return (StringIteratorType(_str, &_strLen, 0));
}

///////////////////////////////////////////////////////////////////////////////
inline TString::StringIteratorType TString::_end(void) const
{
    return (StringIteratorType(_str, &_strLen, _strLen));
}

///////////////////////////////////////////////////////////////////////////////
inline TString::StringIteratorType TString::_rBegin(void) const
{
    return (StringIteratorType(_str, &_strLen, _strLen - 1));
}

///////////////////////////////////////////////////////////////////////////////
size_t TString::_getLength(const ConstIterator First,
    const ConstIterator Second)
{
    if (First.current.first != Second.current.first)
        throw;
    if (First.current.pos > Second.current.pos)
        throw;
    return (Second.current.pos - First.current.pos);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator::ConstIterator() {}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator::ConstIterator(StringIteratorType p) : current(p) {}

///////////////////////////////////////////////////////////////////////////////
const char& TString::ConstIterator::operator[](int Index) const
{
    return (_randomAccess(Index));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::ConstIterator::operator*(void) const
{
    return (retrieve());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator& TString::ConstIterator::operator++(void)
{
    _increment();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::ConstIterator::operator++(int)
{
    ConstIterator old = *this;
    _increment();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator& TString::ConstIterator::operator--(void)
{
    _decrement();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::ConstIterator::operator--(int)
{
    ConstIterator old = *this;
    _decrement();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
bool TString::ConstIterator::operator==(const ConstIterator& Rhs) const
{
    return (current.first == Rhs.current.first &&
        current.pos == Rhs.current.pos);
}

///////////////////////////////////////////////////////////////////////////////
bool TString::ConstIterator::operator!=(const ConstIterator& Rhs) const
{
    return (!(*this == Rhs));
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::ConstIterator::operator+(sizeType i)
{
    ConstIterator toReturn;
    _add(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstIterator TString::ConstIterator::operator-(sizeType i)
{
    ConstIterator toReturn;
    _subtract(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
char& TString::ConstIterator::retrieve(void) const
{
    return (*(current.first + current.pos));
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstIterator::_increment(void)
{
    if (current.pos > *current.size)
        throw;
    current.pos++;
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstIterator::_decrement(void)
{
    if (current.pos == 0)
        throw;
    current.pos--;
}

///////////////////////////////////////////////////////////////////////////////
char& TString::ConstIterator::_randomAccess(sizeType i) const
{
    if (i >= *current.size)
        throw;
    return (*(current.first + i));
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstIterator::_add(ConstIterator& toReturn, sizeType i)
{
    if (current.pos + i > *current.size)
        throw;
    toReturn = *this;
    toReturn.current.pos += i;
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstIterator::_subtract(ConstIterator& toReturn, sizeType i)
{
    if (current.pos < i)
        throw;
    toReturn = *this;
    toReturn.current.pos -= i;
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator::Iterator(void) : _myBase() {}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator::Iterator(StringIteratorType p) : _myBase(p) {}

///////////////////////////////////////////////////////////////////////////////
char& TString::Iterator::operator[](int Index)
{
    return (_myBase::_randomAccess(Index));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::Iterator::operator[](int Index) const
{
    return (_myBase::operator[](Index));
}

///////////////////////////////////////////////////////////////////////////////
char& TString::Iterator::operator*(void)
{
    return (_myBase::retrieve());
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::Iterator::operator*(void) const
{
    return (_myBase::operator*());
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator& TString::Iterator::operator++(void)
{
    _myBase::_increment();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Iterator::operator++(int)
{
    Iterator old = *this;
    _myBase::_increment();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator& TString::Iterator::operator--(void)
{
    _myBase::_decrement();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Iterator::operator--(int)
{
    Iterator old = *this;
    _myBase::_decrement();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Iterator::operator+(sizeType i)
{
    Iterator toReturn;
    _add(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString::Iterator TString::Iterator::operator-(sizeType i)
{
    Iterator toReturn;
    _subtract(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator::ConstReverseIterator(void) : _myBase() {}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator::ConstReverseIterator(StringIteratorType p) :
    _myBase(p) {}

///////////////////////////////////////////////////////////////////////////////
const char& TString::ConstReverseIterator::operator[](int Index) const
{
    return (_myBase::operator[](Index));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::ConstReverseIterator::operator*(void) const
{
    return (_myBase::operator*());
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator& TString::ConstReverseIterator::operator++()
{
    _increment();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::ConstReverseIterator::operator++(int)
{
    ConstReverseIterator old = *this;
    _increment();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator& TString::ConstReverseIterator::operator--()
{
    _decrement();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::ConstReverseIterator::operator--(int)
{
    ConstReverseIterator old = *this;
    _decrement();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::ConstReverseIterator::operator+(
    sizeType i)
{
    ConstReverseIterator toReturn;
    _add(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString::ConstReverseIterator TString::ConstReverseIterator::operator-(
    sizeType i)
{
    ConstReverseIterator toReturn;
    _subtract(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstReverseIterator::_increment(void)
{
    if (_myBase::current.pos == 0)
    {
        _myBase::current.pos = *_myBase::current.size;
        return;
    }
    if (current.pos == *current.size)
        throw;
    _myBase::_decrement();
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstReverseIterator::_decrement(void)
{
    if (current.pos == *_myBase::current.size)
    {
        _myBase::current.pos = 0;
        return;
    }
    if (current.pos + 1 == *_myBase::current.size)
        throw;
    _myBase::_increment();
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstReverseIterator::_add(ConstReverseIterator& toReturn,
    sizeType i)
{
    if (current.pos == *_myBase::current.size && !i)
        throw;
    if (current.pos == i - 1)
    {
        _myBase::current.pos = *_myBase::current.size;
        return;
    }
    if (current.pos < i - 1)
        throw;
    toReturn = *this;
    toReturn.current.pos -= i;
}

///////////////////////////////////////////////////////////////////////////////
void TString::ConstReverseIterator::_subtract(ConstReverseIterator& toReturn,
    sizeType i)
{
    if (*_myBase::current.size <= _myBase::current.pos + i)
        throw;
    toReturn = *this;
    toReturn.current.pos += i;
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator::ReverseIterator(void) : _myBase() {}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator::ReverseIterator(StringIteratorType p) : _myBase(p) {}

///////////////////////////////////////////////////////////////////////////////
char& TString::ReverseIterator::operator[](int Index)
{
    return (_myBase::_randomAccess(Index));
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::ReverseIterator::operator[](int Index) const
{
    return (_myBase::operator[](Index));
}

///////////////////////////////////////////////////////////////////////////////
char& TString::ReverseIterator::operator*(void)
{
    return (_myBase::retrieve());
}

///////////////////////////////////////////////////////////////////////////////
const char& TString::ReverseIterator::operator*(void) const
{
    return (_myBase::operator*());
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator& TString::ReverseIterator::operator++(void)
{
    _myBase::_increment();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator TString::ReverseIterator::operator++(int)
{
    ReverseIterator old = *this;
    _myBase::_increment();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator& TString::ReverseIterator::operator--(void)
{
    _myBase::_decrement();
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator TString::ReverseIterator::operator--(int)
{
    ReverseIterator old = *this;
    _myBase::_decrement();
    return (old);
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator TString::ReverseIterator::operator+(sizeType i)
{
    ReverseIterator toReturn;
    _myBase::_add(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString::ReverseIterator TString::ReverseIterator::operator-(sizeType i)
{
    ReverseIterator toReturn;
    _myBase::_subtract(toReturn, i);
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
void Swap(TString& A, TString& B)
{
    A.Swap(B);
}

///////////////////////////////////////////////////////////////////////////////
TString operator+(const TString& Lhs, const TString& Rhs)
{
    TString toReturn = Lhs;
    toReturn += Rhs;
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString operator+(const TString& Lhs, const char* Rhs)
{
    TString toReturn = Lhs;
    toReturn += Rhs;
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString operator+(const char* Lhs, const TString& Rhs)
{
    TString toReturn = Lhs;
    toReturn += Rhs;
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString operator+(const TString& Lhs, char Rhs)
{
    TString toReturn = Lhs;
    toReturn += Rhs;
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
TString operator+(char Lhs, const TString& Rhs)
{
    TString toReturn = Rhs;
    toReturn += Lhs;
    return (toReturn);
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const TString& Str)
{
    for (size_t i = 0; i < Str.Length(); i++)
        os << Str[i];
    return (os);
}

///////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& is, TString& Str)
{
    Str.Clear();
    char Ch;

    do {
        Ch = is.get();
        if (Ch == '\n' || Ch == ' ')
            break;
        Str.PushBack(Ch);
    } while (true);
    return (is);
}

///////////////////////////////////////////////////////////////////////////////
std::istream& getline(std::istream& is, TString& Str, char Delim)
{
    Str.Clear();
    char Ch;

    do {
        Ch = is.get();
        if (Ch == Delim)
            break;
        Str.PushBack(Ch);
    } while (true);
    return (is);
}

TString::operator std::string(void) const
{
    return (std::string(this->_str));
}

} // namespace Ax
