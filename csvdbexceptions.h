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
        OpenFileException(const std::wstring& path):CSVDbException(101,L"�򲻿��ļ�" + path){}
    };

    class CSVFormatException :public CSVDbException
    {
    public:
        CSVFormatException(const std::wstring& detail):CSVDbException(201, L"CSV Database�ļ���ʽ����\n" + detail) {}
    };

    class InvalidValueException :public CSVDbException
    {
    public:
        InvalidValueException(const std::wstring& detail) :CSVDbException(301, L"CSV ���ݷǷ�ֵ\n" + detail) {}
    };

    class DatabaseBrokenException :public CSVDbException
    {
    public:
        DatabaseBrokenException(const std::wstring& path) :CSVDbException(401, L"���ݿ��ʧ��:\n" + path) {}
    };

    class TableHasExsistExption :public CSVDbException
    {
    public:
        TableHasExsistExption(const std::wstring& path, const std::wstring& tableName)
            :CSVDbException(501, L"���ݱ��Ѵ���\n'" + path + L"'." + tableName) {}
    };

    class InvalidHeaderException :public CSVDbException
    {
    public:
        InvalidHeaderException(const std::wstring& detail) :CSVDbException(601, L"�Ƿ� CSV ��ͷ\n" + detail) {}
    };

    class PrimaryKeyOverlapException :public CSVDbException
    {
    public:
        PrimaryKeyOverlapException(const std::wstring& detail) :CSVDbException(601, L"�ظ�����ֵ\n" + detail) {}
    };

    class UnexpectOperationExcetion :public CSVDbException
    {
    public:
        UnexpectOperationExcetion() :CSVDbException(701, L"�Ƿ�����") {}
    };

    class OutOfIndexException :public CSVDbException
    {
    public:
        OutOfIndexException() :CSVDbException(801, L"�±����") {}
    };

    class UnexpectCodeException :public CSVDbException
    {
    public:
        UnexpectCodeException() :CSVDbException(901, L"�Ƿ��ļ�����") {}
    };

    class InvalidForeignKeyException :public CSVDbException
    {
    public:
        InvalidForeignKeyException(const std::wstring& foreignKey, const std::wstring& data) :CSVDbException(102, L"�Ƿ������" + foreignKey + L" ֵ��" + data) {}
    };
}
