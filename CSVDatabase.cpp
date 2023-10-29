#include "CSVDatabase.h"
#include <fstream>
#include <direct.h>
#include <io.h>
#include "csvdbexceptions.h"
#include <cstdio>
#include "codeConvert.h"
using namespace csvdb;
using namespace csvop;

Database::Database(std::wstring dbPath)
	:dbPath(dbPath), tablesPath(dbPath + L"tables\\"), headersPath(dbPath + L"headers\\"),
    dbCSV(CSV(dbPath + L"tables.csv", { DATATYPE::STR}))
{
    try {
        vector<vector<CSVData>> tbs = dbCSV.read(1);
        for (vector<CSVData> i:tbs) {
            tableList.insert(i[1].getStr());
            std::ifstream ftb(tablesPath + i[1].getStr() + L".csv");
            std::ifstream fhd(headersPath + i[1].getStr() + L".csv");
            //wcout << headersPath + i[0].getStr() + L".csv";
            if (!ftb.good()||!fhd.good())
            {
                ftb.close(); fhd.close();
                throw DatabaseBrokenException(dbPath);
            }
            ftb.close(); fhd.close();
        }
    }
    catch (OpenFileException e) {
        throw DatabaseBrokenException(dbPath);
    }

}

void Database::createDatabase(std::wstring dbPath)
{
    remove(UnicodeToANSI(dbPath + L"tables.csv").c_str());
    const string dbFolder = UnicodeToANSI(dbPath);
    const string tbFolder = UnicodeToANSI(dbPath + L"tables\\");
    const string hdFolder = UnicodeToANSI(dbPath + L"headers\\");
    const string table = UnicodeToANSI(dbPath + L"tables.csv");
    if (_access(dbFolder.c_str(), 0) == -1) _mkdir(dbFolder.c_str());
    if (_access(tbFolder.c_str(), 0) == -1) _mkdir(tbFolder.c_str());
    if (_access(hdFolder.c_str(), 0) == -1) _mkdir(hdFolder.c_str());

    if (_access(table.c_str(), 0) == -1)
    {
        std::ofstream fdb(dbPath + L"tables.csv");
        fdb.close();
        CSV dbCSV(dbPath + L"tables.csv", { DATATYPE::STR, DATATYPE::STR });
        dbCSV.insertLine({ CSVData(L"tableName",DATATYPE::STR) }, 0);
    }
}

void Database::createTable(std::wstring tableName, std::vector<table::Field> fields)
{
    if (find(tableList.begin(), tableList.end(), tableName) != tableList.end())
    {
        throw TableHasExsistExption(dbPath, tableName);
    }
    size_t count = 0;
    for(auto i : fields)
    {
        if (i.isPrimary())
            count++;
        wstring tmp;
        if ((tmp = i.getForeignKeyString()) != L"")
        {
            size_t pos = tmp.find(L".");
            if (pos == wstring::npos)
                throw InvalidValueException(L"非法外键"+tmp);
            wstring tn=tmp.substr(0, pos),key= tmp.substr(pos + 1);
            if(find(tableList.begin(), tableList.end(), tn) == tableList.end()||!getTableHeader(tn).isFieldExsist(key))
                throw InvalidValueException(L"非法外键" + tmp);
        }
    }
    if(count == 0)
        throw InvalidHeaderException(L"未指定主键");

    dbCSV.insertLine({ CSVData(tableName)});
    std::ofstream ftb(dbPath + L"tables\\" + tableName + L".csv");
    std::ofstream fhd(dbPath + L"headers\\" + tableName + L".csv");
    ftb.close(); fhd.close();

    std::vector<CSVData> fieldNames;
    std::vector<DATATYPE> dataTypes;
    CSV hdCSV(dbPath + L"headers\\" + tableName + L".csv",
        { DATATYPE::STR, DATATYPE::STR, DATATYPE::STR, DATATYPE::STR, DATATYPE::STR, DATATYPE::STR });
    hdCSV.insertLine({CSVData(L"字段名",DATATYPE::STR), CSVData(L"类型",DATATYPE::STR), CSVData(L"主键",DATATYPE::STR), CSVData(L"外键",DATATYPE::STR), CSVData(L"自增",DATATYPE::STR), CSVData(L"自增值",DATATYPE::STR) });
    for (auto i : fields)
    {
        fieldNames.push_back(i.getNameString());
        dataTypes.push_back(DATATYPE::STR);
        hdCSV.insertLine({
            CSVData(i.getNameString(),DATATYPE::STR),
            CSVData(i.getTypeString(),DATATYPE::STR) ,
            CSVData(i.getIsPrimaryString(),DATATYPE::STR),
            CSVData(i.getForeignKeyString(),DATATYPE::STR),
            CSVData(i.getSelfIncreaseString(),DATATYPE::STR),
            CSVData(i.getIncreaseString(),DATATYPE::STR),
            });
    }
    CSV tbCSV(dbPath + L"tables\\" + tableName + L".csv", dataTypes);
    tbCSV.insertLine(fieldNames);
    tableList.insert(tableName);
}

std::set<std::wstring> Database::getTableList()
{
    return tableList;
}

bool Database::tableExist(std::wstring tableName)
{
    return tableList.count(tableName) == 1;
}

table::TableHeader Database::getTableHeader(std::wstring tableName,bool isIndex)
{
    CSV hdCSV(dbPath + L"headers\\" + tableName + L".csv",
        { DATATYPE::STR, DATATYPE::STR, DATATYPE::BOOL, DATATYPE::STR, DATATYPE::BOOL, DATATYPE::INT });
    vector<vector<CSVData>> datas = hdCSV.read(1);
    vector<table::Field> fields;
    int i = 0;
    if (isIndex)
    {
        fields.push_back(table::Field(
            L"index",
            DATATYPE::INT,
            false, 0));
        i = 1;
    }
    
    for(vector<CSVData> item : datas)
    {
        fields.push_back(table::Field(
            item[1].getStr(), 
            stringToDataType(item[2].getStr()), 
            item[3].getBool(),i,
            item[4].getStr(), item[5].getBool(),
            item[6].getInt()
        ));
        i++;
     }
    return table::TableHeader(fields);
}

bool csvdb::Database::insert(std::wstring tableName, std::vector<std::wstring> row)
{
    try
    {
        table::TableHeader h = getTableHeader(tableName,false);
        std::vector<CSVData> datas;
        for (size_t i = row.size(); i < h.getTypes().size(); i++)
        {
            row.push_back(L"");
        }
        std::vector < DATATYPE> types = h.getTypes();
        std::vector < csvdb::table::Field> fields = h.getFields();
        for (size_t i = 0; i < types.size(); i++)
        {
            datas.push_back(CSVData(row[i], types[i]));
            if (!checkForeignKey(fields[i].getForeignTable(), fields[i].getForeignField(), CSVData(row[i], types[i])))
                return false;
        }
        CSV tdCSV(dbPath + L"tables\\" + tableName + L".csv", h.getTypes());
        tdCSV.insertLine(datas, -1, h.getPrimaryIndexs(),1);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool csvdb::Database::insert(std::wstring tableName, std::vector<CSVData> row)
{
    try
    {
        table::TableHeader h = getTableHeader(tableName,false);
        std::vector < DATATYPE> types = h.getTypes();
        std::vector < csvdb::table::Field> fields = h.getFields();
        if (row.size() != types.size())
            return false;
        for (size_t i = 0; i < types.size(); i++)
        {
            if (!checkForeignKey(fields[i].getForeignTable(), fields[i].getForeignField(), row[i]))
                return false;
        }
        CSV tdCSV(dbPath + L"tables\\" + tableName + L".csv", h.getTypes());
        tdCSV.insertLine(row, -1, h.getPrimaryIndexs(),1);
        return true;
    }
    catch (...)
    {
        return false;
    }
}


bool csvdb::Database::insert(std::wstring tableName, const std::vector<std::wstring>& fieldName, const  std::vector<csvop::CSVData>& data, std::vector<csvop::CSVData>* increasing)
{
    if (fieldName.size() != data.size())
        return false;
    table::TableHeader h = getTableHeader(tableName, false);
    vector<CSVData> insertData(h.getTypes().size());

    for (size_t i = 0; i < fieldName.size(); i++)
    {
        if (!checkForeignKey(h[fieldName[i]].getForeignTable(), h[fieldName[i]].getForeignField(), data[i]))
            throw InvalidForeignKeyException(h[fieldName[i]].getForeignKeyString(), data[i].toString());
        insertData[h.getIndex(fieldName[i])] = data[i];
    }
    vector<table::Field> changeField;
    std::vector<table::Field> fs = h.getFields();
    for (auto& i : fs)
    {
        if (i.isSelfIncrease())
        {
            i.increase();
            insertData[h.getIndex(i.getNameString())] = CSVData(i.getIncrease());
            changeField.push_back(i);
        }
        if (i.isPrimary() && increasing != NULL)
        {
            increasing->push_back(i.getNameString());
            increasing->push_back(insertData[h.getIndex(i.getNameString())]);
        }
    }
    try
    {
        CSV tdCSV(dbPath + L"tables\\" + tableName + L".csv", h.getTypes());
        tdCSV.insertLine(insertData, -1, h.getPrimaryIndexs(), 1);
    }
    catch (...)
    {
        return false;
    }

    bool res = true;
    for (auto i : changeField)
    {
        if (!updateHeader(tableName, i.getNameString(), { L"自增值" }, { CSVData(i.getIncrease()) }))
            res = false;
    }
    return res;
}


bool csvdb::Database::updateHeader(std::wstring tableName, std::wstring headerName, std::vector<std::wstring> fieldName, std::vector<csvop::CSVData> data)
{
    table::TableHeader h = table::TableHeader(std::vector<table::Field>{
        table::Field(L"index", DATATYPE::INT, false, 0),
        table::Field(L"字段名", DATATYPE::STR, true, 1),
        table::Field(L"类型", DATATYPE::STR, false, 2),
        table::Field(L"主键", DATATYPE::BOOL, false, 3),
        table::Field(L"外键", DATATYPE::STR, false, 4),
        table::Field(L"自增", DATATYPE::STR, false, 5),
        table::Field(L"自增值", DATATYPE::INT, false, 6),
    });
    CSV tdCSV(dbPath + L"headers\\" + tableName + L".csv", h.getTypes());
    vector<vector<CSVData>> datas = tdCSV.read(1);
    table::TableData td = table::TableData(h, datas);
    td = td[td[L"字段名"] == headerName];
    if (td.length() < 1)
        return false;
    try
    {
        for (size_t i = 0; i < fieldName.size(); i++)
        {
            td[0][h.getIndex(fieldName[i])] = data[i];
        }
        tdCSV.updateLine(td[0], td[0][0].getInt(), h.getPrimaryIndexs(),1);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

int csvdb::Database::update(std::wstring tableName, std::vector<std::wstring> fieldName, std::vector<CSVData> data, const table::DataFilter& filter)
{
    if (fieldName.size() != data.size())
        return -1;
    table::TableHeader h = getTableHeader(tableName);
    table::TableData td = (*this)[tableName][filter];
    int updatLen = td.length();
    if (updatLen == 0)
        return 0;
    vector<size_t> fieldIndex;
    for (size_t i = 0; i < fieldName.size(); i++)
    {
        if (!checkForeignKey(h[fieldName[i]].getForeignTable(), h[fieldName[i]].getForeignField(), data[i]))
            return -1;
        fieldIndex.push_back(h.getIndex(fieldName[i]));
    }
    try
    {
        CSV tdCSV(dbPath + L"tables\\" + tableName + L".csv", h.getTypes());
        for (size_t i = 0; i < updatLen; i++)
        {
            for (size_t j = 0; j < fieldIndex.size(); j++)
            {
                td[i][fieldIndex[j]] = data[j];
            }
            tdCSV.updateLine(td[i], td[i][0].getInt(), h.getPrimaryIndexs(), 1);
        }
        return updatLen;
    }
    catch (...)
    {
        return -1;
    }
}

bool csvdb::Database::drop(std::wstring tableName, const table::DataFilter& filter)
{
    table::TableHeader h = getTableHeader(tableName);
    table::TableData td = (*this)[tableName][filter];
    td = td[td[L"index"]];
    CSV tdCSV(dbPath + L"tables\\" + tableName + L".csv", h.getTypes());
    for (size_t i = 0; i < td.length(); i++)
    {
        tdCSV.deleteLine(td[i][0].getInt()-i);
    }
    return true;
}

table::TableData Database::operator[](const std::wstring tableName)
{
    table::TableHeader h = getTableHeader(tableName);
    CSV tdCSV(dbPath + L"tables\\" + tableName + L".csv", h.getTypes());
    vector<vector<CSVData>> datas = tdCSV.read(1);
    return table::TableData(h, datas);
}

bool Database::checkForeignKey(std::wstring tableName, std::wstring fieldName, CSVData data)
{
    try {
        if (tableName == L"" || fieldName == L"")
            return true;
        table::TableData tb = (*this)[tableName];
        if (tb[tb[fieldName] == data].length() > 0)
            return true;
        else
            return false;
    }
    catch (...)
    {
        return false;
    }
}

