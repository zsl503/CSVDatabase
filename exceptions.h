#pragma once
#include <iostream>
#include <string>

class CSVDBException
{
public:
    CSVDBException(unsigned int code, const std::wstring& what_arg): code(code), what_arg(what_arg){}
    virtual ~CSVDBException() = default;
    virtual const std::wstring what(void) const;
    
private:
    unsigned int code;
    std::wstring what_arg;
};  

class OpenFileException :public CSVDBException
{
public:
    OpenFileException(const std::wstring& path):CSVDBException(101,L"打不开文件" + path){}
};

class CSVFormatException :public CSVDBException
{
public:
    CSVFormatException(const std::wstring& detail):CSVDBException(201, L"CSV Database文件格式错误\n" + detail) {}
};

class InvalidValueException :public CSVDBException
{
public:
    InvalidValueException(const std::wstring& detail) :CSVDBException(301, L"CSV 数据非法值\n" + detail) {}
};

class DatabaseBrokenException :public CSVDBException
{
public:
    DatabaseBrokenException(const std::wstring& path) :CSVDBException(401, L"数据库打开失败:\n" + path) {}
};

class TableHasExsistExption :public CSVDBException
{
public:
    TableHasExsistExption(const std::wstring& path, const std::wstring& tableName)
        :CSVDBException(501, L"数据表已存在\n'" + path + L"'." + tableName) {}
};

class InvalidHeaderException :public CSVDBException
{
public:
    InvalidHeaderException(const std::wstring& detail) :CSVDBException(601, L"非法 CSV 表头\n" + detail) {}
};

class PrimaryKeyOverlapException :public CSVDBException
{
public:
    PrimaryKeyOverlapException(const std::wstring& detail) :CSVDBException(601, L"重复主键值\n" + detail) {}
};

class UnexpectOperationExcetion :public CSVDBException
{
public:
    UnexpectOperationExcetion() :CSVDBException(701, L"非法操作") {}
};

class OutOfIndexException :public CSVDBException
{
public:
    OutOfIndexException() :CSVDBException(801, L"下标溢出") {}
};