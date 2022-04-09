#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"codeConvert.h"

string UnicodeToUTF8(const wstring& wstr)
{
    string ret;
    try {
        wstring_convert< codecvt_utf8<wchar_t> > wcv;
        ret = wcv.to_bytes(wstr);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }
    return ret;
}

wstring UTF8ToUnicode(const string& str)
{
    wstring ret;
    try {
        wstring_convert< codecvt_utf8<wchar_t> > wcv;
        ret = wcv.from_bytes(str);
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }
    return ret;
}


string UnicodeToANSI(const wstring& wstr)
{
    string ret;
    mbstate_t state = {};
    const wchar_t* src = wstr.data();
    size_t len = wcsrtombs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        unique_ptr< char[] > buff(new char[len + 1]);
        len = wcsrtombs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

string UTF8ToANSI(const string& str)
{
    return UnicodeToANSI(UTF8ToUnicode(str));
}

wstring ANSIToUnicode(const string& str)
{
    wstring ret;
    mbstate_t state = {};
    const char* src = str.data();
    size_t len = mbsrtowcs(nullptr, &src, 0, &state);
    if (static_cast<size_t>(-1) != len) {
        unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
        len = mbsrtowcs(buff.get(), &src, len, &state);
        if (static_cast<size_t>(-1) != len) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}


string ANSIToUTF8(const string& str)
{
    return UnicodeToUTF8(ANSIToUnicode(str));
}
