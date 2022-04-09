#include "CSVDatabase.h"
#include <fstream>
#include "CSV.h"
#include <direct.h>
#include <io.h>
#include "global.h"
#include <cstdio>
#include "codeConvert.h"
using namespace CSVDatabase;
using namespace CSVOperate;

CSVDatabase::Database::Database(std::wstring dbPath)
	:dbPath(dbPath), tablesPath(dbPath + L"tables\\"), headersPath(dbPath + L"headers\\"),
    dbCSV(CSV(dbPath + L"tables.csv", { DATATYPE::STR}))
{
    try {
        vector<vector<CSVData>> tbs = dbCSV.readCSV(1);
        for (vector<CSVData> i:tbs) {
            tableList.push_back(i[0].getStr());
            std::ifstream ftb(tablesPath + i[0].getStr() + L".csv");
            std::ifstream fhd(headersPath + i[0].getStr() + L".csv");
            wcout << headersPath + i[0].getStr() + L".csv";
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

void CSVDatabase::Database::createDatabase(std::wstring dbPath)
{
    remove(UnicodeToANSI(dbPath + L"tables.csv").c_str());
    const string dbFolder = UnicodeToANSI(dbPath);
    const string tbFolder = UnicodeToANSI(dbPath + L"tables\\");
    const string hdFolder = UnicodeToANSI(dbPath + L"headers\\");
    if (_access(dbFolder.c_str(), 0) == -1) _mkdir(dbFolder.c_str());
    if (_access(tbFolder.c_str(), 0) == -1) _mkdir(tbFolder.c_str());
    if (_access(hdFolder.c_str(), 0) == -1) _mkdir(hdFolder.c_str());
    std::ofstream fdb(dbPath + L"tables.csv");
    fdb.close();
    CSV dbCSV(dbPath + L"tables.csv", { DATATYPE::STR, DATATYPE::STR });
    dbCSV.insertLineToCSV({ CSVData(L"表名")},0);
}

void Database::createTable(std::wstring tableName, std::vector<Table::Field> fields)
{
    if (find(tableList.begin(), tableList.end(), tableName) != tableList.end())
    {
        throw TableHasExsistExption(dbPath, tableName);
    }
    size_t count = 0;
    for(auto i : fields)
    {
        if (i.isPrimary())
        {
            count++;
            if (count > 1)
                throw InvalidHeaderException(L"主键数量过多");
        }
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

    dbCSV.insertLineToCSV({ CSVData(tableName)});
    std::ofstream ftb(dbPath + L"tables\\" + tableName + L".csv");
    std::ofstream fhd(dbPath + L"headers\\" + tableName + L".csv");
    ftb.close(); fhd.close();

    std::vector<CSVData> fieldNames;
    std::vector<DATATYPE> dataTypes;
    CSV hdCSV(dbPath + L"headers\\" + tableName + L".csv",
        { DATATYPE::STR, DATATYPE::STR, DATATYPE::STR, DATATYPE::STR });
    hdCSV.insertLineToCSV({CSVData(L"字段名"), CSVData(L"类型"), CSVData(L"主键"), CSVData(L"外键")});
    for (auto i : fields)
    {
        fieldNames.push_back(i.getNameString());
        dataTypes.push_back(DATATYPE::STR);
        hdCSV.insertLineToCSV({
            CSVData(i.getNameString()),
            CSVData(i.getTypeString()) ,
            CSVData(i.getIsPrimaryString()),
            CSVData(i.getForeignKeyString())
            });
    }
    CSV tbCSV(dbPath + L"tables\\" + tableName + L".csv", dataTypes);
    tbCSV.insertLineToCSV(fieldNames);
}

Table::TableHeader CSVDatabase::Database::getTableHeader(std::wstring tableName)
{
    CSV hdCSV(dbPath + L"headers\\" + tableName + L".csv",
        { DATATYPE::STR, DATATYPE::STR, DATATYPE::BOOL, DATATYPE::STR });
    vector<vector<CSVData>> datas = hdCSV.readCSV(1);
    vector<Table::Field> fields;
    for(vector<CSVData> item : datas)
    {
        fields.push_back(Table::Field(
            item[0].getStr(), 
            stringToDataType(item[1].getStr()), 
            item[2].getBool(),
            item[3].getStr()));
     }
    return Table::TableHeader(fields);
}
