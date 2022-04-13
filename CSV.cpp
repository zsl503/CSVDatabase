#pragma once
#include "CSV.h"
#include "codeConvert.h"
#include "others.h"
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

using namespace CSVOperate;

wstring CSV::wchars2wstring(vector<wchar_t> source)
{
    return wstring(source.begin(),source.end());
}

vector<wstring> CSV::csvSplit(const wstring &source)
{
    vector<wstring> res;
    vector<wchar_t> word;
    bool isStrict = false;
    for (size_t i = 0; i < source.size()&&!(source[i] == '"' && i + 1 >= source.size()); i++)
    {
        if(source[i] == L'"')
        {
            if (source[i + 1] != '"')
                isStrict = !isStrict;
            else
            {
                word.push_back(L'"');
                i++;
            }
        }
        else if (source[i] == L','&&!isStrict)
        {
            res.push_back(wchars2wstring(word));
            word.clear();
        }
        else
            word.push_back(source[i]);

    }
    if(!word.empty())
        res.push_back(wchars2wstring(word));
    if (source[source.size() - 1]==L',')
        res.push_back(L"");
    return res;
}

string CSV::splits2CSVWstring(const vector<CSVData> splits)
{
    wstring ret;
    wstring word;
    for (CSVData item : splits)
    {
        word = item.toString();
        replace_all(word, L"\"", L"\"\"");
        if (word.find(',') != wstring::npos)
            ret += (L'"' + word + L'"' + L',');
        else
            ret += (word + L',');
    }
    ret.pop_back();
    return UnicodeToANSI(ret);
}

void CSV::insertLineToCSV(vector<CSVData> splits, int index, int primaryIndex)
{
    for (size_t i = 0; i < splits.size(); i++)
    {
        if (splits[i].getType() != types[i])
            throw InvalidValueException(
                L"非法字段类型，应为 " + dataTypeToStr(types[i]) + L" 输入 " + dataTypeToStr(splits[i].getType()));
    }
    ifstream inFile(path, ios::in);
    if (!inFile)
    {
        inFile.close();
        throw OpenFileException(path);
    }
    ofstream outFile(path + L".edit", ios::out);
    string csvString = splits2CSVWstring(splits);
    string line;
    for (size_t i = 0; getline(inFile, line) || i == index; i++)
    {
        if (primaryIndex != -1 && 
            splits[primaryIndex] == CSVData(csvSplit(ANSIToUnicode(line))[primaryIndex], types[primaryIndex]))
        {
            outFile.close();
            inFile.close();
            remove(UnicodeToANSI(path + L".edit").c_str());
            throw PrimaryKeyOverlapException(splits[i].toString());
        }
        if (i == index) outFile << csvString << endl;
        outFile << line << endl;
    }
    if (index == -1)
        outFile << csvString << endl;

    outFile.close();
    inFile.close();
    remove(UnicodeToANSI(path).c_str());
    int _ = rename(UnicodeToANSI(path + L".edit").c_str(), UnicodeToANSI(path).c_str());
}

vector<CSVData> CSV::deleteLineFromCSV(size_t index)
{
    ifstream inFile(path, ios::in);
    ofstream outFile;
    outFile.open(path + L".edit", ios::out);
    if (!inFile)
    {
        throw OpenFileException(path);
    }
    vector<wstring> split;
    string line;
    vector<CSVData> data;
    for (size_t i = 0; getline(inFile, line); i++)
    {
        if (i == index)
        {
            split = csvSplit(ANSIToUnicode(line));
            for (size_t i = 0; i < types.size(); i++)
                data.push_back(CSVData(split[i], types[i]));
        } 
        else
            outFile << line << endl;
    }

    outFile.close();
    inFile.close();
    remove(UnicodeToANSI(path).c_str());
    rename(UnicodeToANSI(path + L".edit").c_str(), UnicodeToANSI(path).c_str());
    return data;
}

vector<vector<CSVData>> CSV::readCSV(size_t skipLine)
{
    ifstream inFile(path, ios::in);
    if (!inFile)
    {
        throw OpenFileException(path);
    }
    string line;

    vector<vector<CSVData>> res;
    vector<wstring> words;
    vector<CSVData> data;
    while (skipLine--)getline(inFile, line);
    while (getline(inFile, line))//getline(inFile, line)表示按行读取CSV文件中的数据
    {
        wstring wline = ANSIToUnicode(line);
        words = csvSplit(wline);
        if (words.size() == 0)
            continue;
        else if (words.size() < types.size())
            throw CSVFormatException(L"表字段数少于指定字段数，表路径：" + path);
        for (size_t i = 0; i < types.size(); i++)
        {
            data.push_back(CSVData(words[i], types[i]));
        }
        res.push_back(data);
        data.clear();
    }

    inFile.close();
    return res;
}
