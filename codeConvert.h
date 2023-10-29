#pragma once
#include <iostream>
#include <codecvt>
using namespace std;
/*
* UTF��ANSI��Unicode��ת
* wstring Ĭ�ϱ���Ϊ Unicode
* Excel��CSV Ĭ�ϱ���Ϊ ANSI
*/

/**
 * @brief UnicodeתUTF8
 * @param wstr Unicode ����� wstring
 * @return UTF-8 ����� string
*/
string UnicodeToUTF8(const wstring& wstr);

/**
 * @brief UTF8תUnicode
 * @param str UTF8 ����� string
 * @return Unicode ����� wstring
*/
wstring UTF8ToUnicode(const string& str);

/**
 * @brief UnicodeתANSI
 * @param wstr Unicode ����� wstring
 * @return ANSI ����� string
*/
string UnicodeToANSI(const wstring& wstr);

/**
 * @brief UTF8תANSI
 * @param str UTF8 ����� string
 * @return ANSI ����� string
*/
string UTF8ToANSI(const string& str);

/**
 * @brief ANSIתUnicode
 * @param str ANSI ����� string
 * @return Unicode ����� wstring
*/
wstring ANSIToUnicode(const string& str);

/**
 * @brief ANSIתUTF8
 * @param str ANSI ����� string
 * @return UTF8 ����� string
*/
string ANSIToUTF8(const string& str); 