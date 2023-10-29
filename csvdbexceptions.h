#pragma once
#include <iostream>
#include <string>

namespace csvdb
{
    class CSVDbException
    {
    public:
        CSVDbException(unsigned int code, const std::wstring& what_arg);
        virtual ~CSVDbException() = default;
        virtual const std::wstring what(void) const;
    
    private:
        unsigned int code;
        std::wstring what_arg;
    };  

    class OpenFileException :public CSVDbException
    {
    public:
        OpenFileException(const std::wstring& path):CSVDbException(101,L"打不开文件" + path){}
    };

    class CSVFormatException :public CSVDbException
    {
    public:
        CSVFormatException(const std::wstring& detail):CSVDbException(201, L"CSV Database文件格式错误\n" + detail) {}
    };

    class InvalidValueException :public CSVDbException
    {
    public:
        InvalidValueException(const std::wstring& detail) :CSVDbException(301, L"CSV 数据非法值\n" + detail) {}
    };

    class DatabaseBrokenException :public CSVDbException
    {
    public:
        DatabaseBrokenException(const std::wstring& path) :CSVDbException(401, L"数据库打开失败:\n" + path) {}
    };

    class TableHasExsistExption :public CSVDbException
    {
    public:
        TableHasExsistExption(const std::wstring& path, const std::wstring& tableName)
            :CSVDbException(501, L"数据表已存在\n'" + path + L"'." + tableName) {}
    };

    class InvalidHeaderException :public CSVDbException
    {
    public:
        InvalidHeaderException(const std::wstring& detail) :CSVDbException(601, L"非法 CSV 表头\n" + detail) {}
    };

    class PrimaryKeyOverlapException :public CSVDbException
    {
    public:
        PrimaryKeyOverlapException(const std::wstring& detail) :CSVDbException(601, L"重复主键值\n" + detail) {}
    };

    class UnexpectOperationExcetion :public CSVDbException
    {
    public:
        UnexpectOperationExcetion() :CSVDbException(701, L"非法操作") {}
    };

    class OutOfIndexException :public CSVDbException
    {
    public:
        OutOfIndexException() :CSVDbException(801, L"下标溢出") {}
    };

    class UnexpectCodeException :public CSVDbException
    {
    public:
        UnexpectCodeException() :CSVDbException(901, L"非法文件编码") {}
    };

    class InvalidForeignKeyException :public CSVDbException
    {
    public:
        InvalidForeignKeyException(const std::wstring& foreignKey, const std::wstring& data) :CSVDbException(102, L"非法外键：" + foreignKey + L" 值：" + data) {}
    };
}
