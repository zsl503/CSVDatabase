#include "TableHeader.h"
#include "global.h"
using namespace CSVDatabase;
using namespace Table;
Field::Field(std::wstring name, CSVOperate::DATATYPE type, bool isPrimaryKey, int index, std::wstring foreignKey)
    :name(name), type(type), isPrimaryKey(isPrimaryKey),index(index)
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

const std::wstring Field::getTypeString() const
{
    return dataTypeToStr(type);
}

const std::wstring Field::getNameString() const
{
    return name;
}

const std::wstring Field::getIsPrimaryString() const
{
    return isPrimaryKey ? L"TRUE" : L"FALSE";
}

bool Field::isPrimary()
{
    return isPrimaryKey;
}

const size_t Field::getIndex()
{
    return index;
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

bool TableHeader::isFieldExsist(std::wstring name)
{
    for (Field item : fields)
    {
        if (item.getNameString() == name)
            return true;
    }
    return false;
}

