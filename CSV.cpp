#pragma once
#include "CSV.h"
#include "codeConvert.h"
#include "others.h"
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include "csvdbexceptions.h"

using namespace csvop;
using namespace csvdb;
wstring CSV::wchars2wstring(vector<wchar_t> source)
{
    return wstring(source.begin(),source.end());
}

vector<wstring> CSV::csvSplit(const wstring &source)
{
    if (source.size() == 0)
        return {};
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

wstring CSV::splits2CSVWstring(const vector<CSVData> splits)
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
    return ret;
    //return UnicodeToUTF8(ret);
}

csvop::CSV::CSV(wstring path, vector<DATATYPE> types, CSVCode code) 
    :path(path), types(types), code(code) 
{
    switch (code)
    {
    case csvop::CSVCode::UTF8:
    {
        convert2Unicode = UTF8ToUnicode;
        break;
    }
    case csvop::CSVCode::ANSI:
    {
        convert2Unicode = ANSIToUnicode;
        break;
    }
    default:
    {
        throw UnexpectCodeException();
    }
    }
}

void CSV::insertLine(vector<CSVData> splits, int index, vector<int> primaryIndexs, int skipLine)
{
    _updateLine(splits, 0, index, primaryIndexs, skipLine);
}

void CSV::updateLine(vector<CSVData> splits, int index, vector<int> primaryIndexs, int skipLine)
{
    _updateLine(splits, 1, index, primaryIndexs, skipLine);
}

void CSV::_updateLine(vector<CSVData> splits, int mode, int index, vector<int> primaryIndexs,int skipLine)
{
    // 如果是更新，则splits[0]为待更新的列序号，不存在于表；如果是插入，splits[0]为正常表第一个值
    bool isUpdate = false;
    if (splits.size() == types.size() + 1)
    {
        splits.erase(splits.begin());
        isUpdate = true;
    }
    if (splits.size() < 1 || splits.size() != types.size())
        return;

    for (size_t i = 1; i < splits.size(); i++)
    {
        if (!splits[i].isNull() && splits[i].getType() != types[i])
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
    wstring wstr = splits2CSVWstring(splits);
    string csvString;

    switch (code)
    {
    case csvop::CSVCode::UTF8:
    {
        char c1 = 0xEF;
        char c2 = 0xBB;
        char c3 = 0xBF;
        outFile << c1 << c2 << c3;
        csvString = UnicodeToUTF8(wstr);
        break;
    }
    case csvop::CSVCode::ANSI:
        csvString = UnicodeToANSI(wstr);
        break;
    default:
        csvString = string(wstr.begin(), wstr.end());
    }
    string line;
    int i = skipLine;
    while (skipLine--)
    {
        getline(inFile, line);
        outFile << line << endl;
    }
    for (i; getline(inFile, line) || i == index; i++)
    {
        if (line.empty())
            continue;
        if (i == 0)
        {
            switch (code)
            {
            case csvop::CSVCode::UTF8:
            {
                char c1 = 0xEF;
                char c2 = 0xBB;
                char c3 = 0xBF;
                if (line.size() >= 3 && line[0] == c1 && line[1] == c2 && line[2] == c3)
                    line = line.substr(3);
                break;
            }
            default:
                break;
            }
        }
        if (primaryIndexs.size() && !(i == index && mode == 1))
        {
            bool isSamePrimary = true;
            for (auto j : primaryIndexs)
            {
                if (isUpdate)
                    j -= 1;
                CSVData d;
                d = CSVData(csvSplit(convert2Unicode(line))[j], types[j]);
                if (splits[j] != d)
                {
                    isSamePrimary = false;
                    break;
                }

            }
            if (isSamePrimary)
            {
                outFile.close();
                inFile.close();
                remove(UnicodeToANSI(path + L".edit").c_str());
                throw PrimaryKeyOverlapException(wstr);
            }
        }

        if (i == index && csvString.size())
        {
            outFile << csvString << endl;
            if (mode == 1)
                continue;
        }
        outFile << line << endl;
    }
    if (index == -1)
        outFile << csvString << endl;

    outFile.close();
    inFile.close();
    remove(UnicodeToANSI(path).c_str());
    int _ = rename(UnicodeToANSI(path + L".edit").c_str(), UnicodeToANSI(path).c_str());
}

vector<CSVData> CSV::deleteLine(size_t index)
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
        if (line.empty())
            continue;
        if (i == index)
        {
            split = csvSplit(convert2Unicode(line));
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

vector<vector<CSVData>> CSV::read(size_t skipLine)
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
    int index = skipLine;
    while (skipLine--)getline(inFile, line);
    while (getline(inFile, line))//getline(inFile, line)表示按行读取CSV文件中的数据
    {
        wstring wline;
        switch (code)
        {
        case csvop::CSVCode::UTF8:
            wline = UTF8ToUnicode(line);
            break;
        case csvop::CSVCode::ANSI:
            wline = ANSIToUnicode(line);
            break;
        }
        words = csvSplit(wline);
        if (words.size() == 0)
            continue;
        else if (words.size() < types.size())
        {
            inFile.close();
            throw CSVFormatException(L"表字段数少于指定字段数，表路径：" + path);
        }
        data.push_back(csvop::CSVData(index++));
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
