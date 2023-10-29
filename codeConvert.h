#pragma once
#include <iostream>
#include <codecvt>
using namespace std;
/*
* UTF、ANSI、Unicode互转
* wstring 默认编码为 Unicode
* Excel的CSV 默认编码为 ANSI
*/

/**
 * @brief Unicode转UTF8
 * @param wstr Unicode 编码的 wstring
 * @return UTF-8 编码的 string
*/
string UnicodeToUTF8(const wstring& wstr);

/**
 * @brief UTF8转Unicode
 * @param str UTF8 编码的 string
 * @return Unicode 编码的 wstring
*/
wstring UTF8ToUnicode(const string& str);

/**
 * @brief Unicode转ANSI
 * @param wstr Unicode 编码的 wstring
 * @return ANSI 编码的 string
*/
string UnicodeToANSI(const wstring& wstr);

/**
 * @brief UTF8转ANSI
 * @param str UTF8 编码的 string
 * @return ANSI 编码的 string
*/
string UTF8ToANSI(const string& str);

/**
 * @brief ANSI转Unicode
 * @param str ANSI 编码的 string
 * @return Unicode 编码的 wstring
*/
wstring ANSIToUnicode(const string& str);

/**
 * @brief ANSI转UTF8
 * @param str ANSI 编码的 string
 * @return UTF8 编码的 string
*/
string ANSIToUTF8(const string& str); 