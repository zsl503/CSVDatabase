#include "TableData.h"
using namespace CSVDatabase::Table;
const FilterGetter TableData::operator[] (const std::wstring name) const
{
    return FilterGetter(header, name);
}

//TableData TableData::operator[](const std::wstring& name)
//{
//    return (*this)[(*this)(name)];
//}

TableData TableData::operator[](const FieldGetter& getter)
{
    std::vector<std::vector<CSVOperate::CSVData>> res;
    for (auto col : datas)
    {
        res.push_back({ getter.verify(col) });
    }
    return TableData(const_cast<FieldGetter&>(getter).getNewHeader(), res);
}

TableData CSVDatabase::Table::TableData::operator[](const DataFilter& filter)
{
    std::vector<std::vector<CSVOperate::CSVData>> res;
    for (auto col : datas)
    {
        if(filter.verify(col))
            res.push_back(col);
    }
    return TableData(filter.getHeader(), res);
}

std::vector<CSVOperate::CSVData>& TableData::operator[](int index)
{
    if (index < datas.size())
        return datas[index];
    else
        throw OutOfIndexException();
}
