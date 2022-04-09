#include "CSVType.h"
#include <stdexcept>
#include <stdexcept>
using namespace CSVOperate;
CSVData::CSVData(std::wstring data, DATATYPE type):type(type)
{
	try
	{
		switch (type)
		{
		case DATATYPE::STR:dStr = data; break;
		case DATATYPE::INT:dInt = std::stoi(data); break;
		case DATATYPE::DOUBLE:dDouble = std::stod(data); break;
		case DATATYPE::BOOL:dBool = data == L"TRUE" ? true:false; break;
		default:
			dStr = data;
		}
	}
	catch (std::invalid_argument& e)
	{
		std::wstring error;
		switch (type)
		{
		case DATATYPE::STR:error = L"STR"; break;
		case DATATYPE::INT:error = L"INT"; break;
		case DATATYPE::DOUBLE:error = L"DOUBLE"; break;
		case DATATYPE::BOOL:error = L"BOOL"; break;
		default:
			error = L"OTHER";
		}
		throw InvalidValueException(L"原值：'" + data + L"' 无法转换为 '" + error + L"' 类型");
	}
}

int CSVData::getInt()
{
	return dInt;
}


double CSVData::getDouble()
{
	return dDouble;
}

std::wstring CSVData::getStr()
{
	return dStr;
}


bool CSVData::getBool()
{
	return dBool;
}

std::wstring CSVData::toStrng()
{
	switch (type)
	{
	case DATATYPE::STR:return dStr;
	case DATATYPE::INT:return std::to_wstring(dInt);
	case DATATYPE::DOUBLE:return std::to_wstring(dDouble);
	case DATATYPE::BOOL:return dBool?L"TRUE":L"FALSE";
	default:
		return dStr;
	}
}

std::wstring CSVOperate::dataTypeToStr(DATATYPE type)
{
	switch (type)
	{
	case DATATYPE::STR:return L"STR";
	case DATATYPE::INT:return L"INT";
	case DATATYPE::DOUBLE:return L"DOUBLE";
	case DATATYPE::BOOL:return L"BOOL";
	case DATATYPE::DATE:return L"DATE";
	case DATATYPE::TIME:return L"TIME";
	default:return NULL;
	}
}

DATATYPE CSVOperate::stringToDataType(std::wstring dataString)
{
	if (dataString == L"STR") return DATATYPE::STR;
	else if (dataString == L"INT") return DATATYPE::DOUBLE;
	else if (dataString == L"DOUBLE") return DATATYPE::DOUBLE;
	else if (dataString == L"BOOL") return DATATYPE::DOUBLE;
	else if (dataString == L"DATE") return DATATYPE::DOUBLE;
	else if (dataString == L"TIME") return DATATYPE::DOUBLE;
	else if (dataString == L"INT") return DATATYPE::DOUBLE;
	else
		throw InvalidValueException(L"非法类型值 " + dataString);
}

bool CSVData::operator==(const CSVData& ps) const
{

	if (type == ps.type)
		switch (type)
		{
		case DATATYPE::STR:return dStr==ps.dStr;
		case DATATYPE::INT:return dInt == ps.dInt;
		case DATATYPE::DOUBLE:return dDouble == ps.dDouble;
		case DATATYPE::BOOL:return dBool == ps.dBool;
		}
	return false;
}

DATATYPE CSVData::getType()
{
	return type;
}
