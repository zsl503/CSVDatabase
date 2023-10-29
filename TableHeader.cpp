#include "TableHeader.h"
#include "csvdbexceptions.h"
#include "CSVType.h"

using namespace csvdb;
using namespace table;
Field::Field(std::wstring name, csvop::DATATYPE type, bool isPrimaryKey, int index, std::wstring foreignKey, bool selfIncrease, int increaseNum)
    :name(name), type(type), isPrimaryKey(isPrimaryKey),index(index), selfIncrease(selfIncrease), increaseNum(increaseNum)
{
    if (foreignKey.size() == 0)
    {
        this->foreignKey[0] = L"";
        return;
    }
        
    size_t point = foreignKey.find(L'.');
    if (point == std::wstring::npos)
        throw InvalidValueException(foreignKey);
    this->foreignKey[0] = foreignKey.substr(0, point);
    this->foreignKey[1] = foreignKey.substr(point + 1, foreignKey.size() - point);
}

const std::wstring Field::getForeignKeyString() const
{
    if (foreignKey[0].size() == 0)
        return L"";
    return foreignKey[0] + L'.' + foreignKey[1];
}

bool Field::isSelfIncrease() const
{
    return selfIncrease;
}

const std::wstring csvdb::table::Field::getForeignTable() const
{
    return foreignKey[0];
}

const std::wstring csvdb::table::Field::getForeignField() const
{
    return foreignKey[1];
}

const std::wstring Field::getTypeString() const
{
    return dataTypeToStr(type);
}

const csvop::DATATYPE table::Field::getType() const
{
    return type;
}

const std::wstring Field::getNameString() const
{
    return name;
}

const std::wstring Field::getIsPrimaryString() const
{
    return isPrimaryKey ? L"TRUE" : L"FALSE";
}


const std::wstring Field::getSelfIncreaseString() const
{
    return selfIncrease ? L"TRUE" : L"FALSE";
}

const std::wstring csvdb::table::Field::getIncreaseString() const
{
    if (increaseNum == INFINITY)
        return L"";
    else
        return std::to_wstring(increaseNum);
}

const int csvdb::table::Field::getIncrease() const
{
        return increaseNum;
}

void csvdb::table::Field::increase()
{
    increaseNum++;
}

bool Field::isPrimary()
{
    return isPrimaryKey;
}

std::vector<csvop::DATATYPE> table::TableHeader::getTypes()
{
    std::vector<csvop::DATATYPE> types;
    bool isFirst = fields[0].getNameString() == L"index"?true:false;
    for (auto i : fields)
        isFirst = isFirst ? false : (types.push_back(i.getType()),false);
    return types;
}

const std::vector<Field> csvdb::table::TableHeader::getFields() const
{
    return fields;
}

std::vector<int> csvdb::table::TableHeader::getPrimaryIndexs()
{
    size_t t = 0;
    std::vector<int> res;
    for (Field& item : fields)
    {
        if (item.isPrimary())
            res.push_back(t);
        t++;
    }
    return res;
}

Field& TableHeader::operator[](const std::wstring name)
{
    for (Field& item : fields)
    {
        if (item.getNameString() == name)
            return item;
    }
    throw InvalidValueException(L"´íÎó×Ö¶ÎÃû" + name);
}

size_t TableHeader::getIndex(const std::wstring& name) const
{
    size_t t = 0;
    for (const Field& item : fields)
    {
        if (item.getNameString() == name)
            return t;
        t++;
    }
    throw InvalidValueException(L"´íÎó×Ö¶ÎÃû" + name);
}

bool TableHeader::isFieldExsist(std::wstring name)
{
    for (Field item : fields)
    {
        if (item.getNameString() == name)
            return true;
    }
    return false;
}

bool table::Field::operator==(const Field& fd) const
{
    return name == fd.name && type == fd.type &&
        isPrimaryKey == fd.isPrimaryKey && index == fd.index&&
        foreignKey[0] == fd.foreignKey[0] && foreignKey[1] == fd.foreignKey[1];
}


bool table::Field::operator!=(const Field& fd) const
{
    return !((*this) == fd);
}


bool table::TableHeader::operator==(const TableHeader& hd) const
{
    if (hd.fields.size() != fields.size())
        return false;
    for (size_t i = 0; i < fields.size(); i++)
        if ( hd.fields[i] != fields[i])
            return false;
    return true;
}

bool table::TableHeader::operator!=(const TableHeader& hd) const
{
    return !((*this) == hd);
}
