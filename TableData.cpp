#include "TableData.h"
#include "csvdbexceptions.h"
#include <set>
using namespace csvdb::table;
const FilterGetter TableData::operator[] (const std::wstring name) const
{
    return FilterGetter(header, name);
}

TableData csvdb::table::operator+(const TableData& td1, const TableData& td2)
{
    if (td1.header != td2.header)
        throw UnexpectOperationExcetion();
    TableData td(td1.header, td1.datas);
    std::set<int> rowSet;
    for (auto i : td.datas)
        rowSet.insert(i[0].getInt());
    for (auto i : td2.datas)
    {
        if (rowSet.find(i[0].getInt()) != rowSet.end())
        {
            td.datas.push_back(i);
            rowSet.insert(i[0].getInt());
        }
    }
    return td;
}

TableData TableData::operator[](const FieldGetter& getter)
{
    FilterGetter* p = (FilterGetter*)&getter;
    std::vector<std::vector<csvop::CSVData>> res;
    if ((p->type == FilterGetter::Type::Union))
        for (auto col : datas)
        {
            res.push_back(p->getUnion(col));
        }
    else
        for (auto col : datas)
        {
            res.push_back({ getter.verify(col) });
        }
    return TableData(p->getNewHeader(), res);
}

TableData csvdb::table::TableData::operator[](const DataFilter& filter)
{
    std::vector<std::vector<csvop::CSVData>> res;
    for (auto col : datas)
    {
        if(filter.verify(col))
            res.push_back(col);
    }
    return TableData(filter.getHeader(), res);
}

size_t csvdb::table::TableData::length() const
{
    return datas.size();
}

const TableHeader csvdb::table::TableData::getHeader() const
{
    return header;
}

void csvdb::table::TableData::get(const DataFilter& filter)
{
    std::vector<std::vector<csvop::CSVData>> res;
    for (auto col : datas)
    {
        if (filter.verify(col))
            res.push_back(col);
    }
    datas = res;
}


void csvdb::table::TableData::clear()
{
    datas = std::vector<std::vector<csvop::CSVData>>{};
}


bool csvdb::table::TableData::push(std::vector<csvop::CSVData> row)
{
    std::vector<csvop::DATATYPE> types = header.getTypes();

    if (row.size() != types.size())
        return false;
    for (size_t i = 0; i < row.size(); i++)
    {
        if (row[i].isNull())
            row[i] = csvop::CSVData(L"", types[i]);
        else if (row[i].getType() != types[i])
            return false;
    }
    datas.push_back(row);
    return true;
}

std::vector<csvop::CSVData>& TableData::operator[](int index)
{
    if (index < datas.size())
        return datas[index];
    else
        throw OutOfIndexException();
}
