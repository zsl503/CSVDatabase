#include "CSVType.h"
#include <stdexcept>
#include "csvdbexceptions.h"

#pragma warning( disable : 4996)
using namespace csvop;
using namespace csvdb;
CSVData::CSVData(std::wstring data, DATATYPE type):type(type)
{
	try
	{
		if (data == L"")
		{
			null = true;
			return;
		}
		switch (type)
		{
		case DATATYPE::STR:dStr = data; break;
		case DATATYPE::INT:dInt = (data == L"" ? 0 : std::stoi(data)); break;
		case DATATYPE::DOUBLE:dDouble = (data == L""?0:std::stod(data)); break;
		case DATATYPE::BOOL:dBool = data == L"TRUE" ? true : false; break;
		case DATATYPE::DATE:dDate = wstr2Date(data); break;
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
		case DATATYPE::DATE:error = L"DATE"; break;
		default:
			error = L"OTHER";
		}
		throw InvalidValueException(L"原值：'" + data + L"' 无法转换为 '" + error + L"' 类型");
	}
}

int CSVData::getInt() const
{
	return dInt;
}


double CSVData::getDouble() const
{
	return dDouble;
}

std::wstring CSVData::getStr() const
{
	return dStr;
}


bool CSVData::getBool() const
{
	return dBool;
}

std::wstring CSVData::toString()  const
{
	if(isNull())
		return L"";
	switch (type)
	{
	case DATATYPE::STR:return dStr;
	case DATATYPE::INT:return dInt==INT_MAX?L"":std::to_wstring(dInt);
	case DATATYPE::DOUBLE:return std::to_wstring(dDouble);
	case DATATYPE::BOOL:return dBool ? L"TRUE" : L"FALSE";
	case DATATYPE::DATE:return Date2Wstr(dDate); break;
	default:
		return dStr;
	}
}

time_t csvop::CSVData::wstr2Date(std::wstring date)
{
	tm tmp = tm();
	int year, month, day;
	swscanf(date.c_str(), L"%d/%d/%d", &month, &day, &year);
	tmp.tm_year = year - 1900;
	tmp.tm_mon = month - 1;
	tmp.tm_mday = day;
	return mktime(&tmp);
}

std::wstring csvop::CSVData::Date2Wstr(time_t date)
{
	tm tminfo;
	tminfo = *localtime(&date);
	return std::to_wstring(tminfo.tm_mon + 1) + L"/" + std::to_wstring(tminfo.tm_mday) + L"/" + std::to_wstring(tminfo.tm_year + 1900);
}

bool csvop::CSVData::isNull() const
{
	return null;
}

std::wstring csvop::dataTypeToStr(DATATYPE type) 
{
	switch (type)
	{
	case DATATYPE::STR:return L"STR";
	case DATATYPE::INT:return L"INT";
	case DATATYPE::DOUBLE:return L"DOUBLE";
	case DATATYPE::BOOL:return L"BOOL";
	case DATATYPE::DATE:return L"DATE";
	case DATATYPE::TIME:return L"TIME";
	default:return L"";
	}
}

DATATYPE csvop::stringToDataType(std::wstring dataString)
{
	if (dataString == L"STR") return DATATYPE::STR;
	else if (dataString == L"INT") return DATATYPE::INT;
	else if (dataString == L"DOUBLE") return DATATYPE::DOUBLE;
	else if (dataString == L"BOOL") return DATATYPE::BOOL;
	else if (dataString == L"DATE") return DATATYPE::DATE;
	else if (dataString == L"TIME") return DATATYPE::TIME;
	else if (dataString == L"INT") return DATATYPE::INT;
	else
		throw InvalidValueException(L"非法类型值 " + dataString);
}

DATATYPE CSVData::getType() const
{
	return type;
}

bool CSVData::operator!() const
{
	switch (type)
	{
	case DATATYPE::STR:return dStr != L"";
	case DATATYPE::INT:return dInt;
	case DATATYPE::DOUBLE:return dDouble;
	case DATATYPE::BOOL:return dBool;
	}
	return false;
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
		case DATATYPE::DATE:return dDate == ps.dDate;
		}
	return false;
}

bool CSVData::operator<(const CSVData& ps) const
{

	if (type == ps.type)
		switch (type)
		{
		case DATATYPE::STR:return dStr < ps.dStr;
		case DATATYPE::INT:return dInt < ps.dInt;
		case DATATYPE::DOUBLE:return dDouble < ps.dDouble;
		case DATATYPE::BOOL:return dBool < ps.dBool;
		case DATATYPE::DATE:return dDate < ps.dDate;
		}
	return false;
}

bool CSVData::operator>(const CSVData& ps) const
{

	if (type == ps.type)
		switch (type)
		{
		case DATATYPE::STR:return dStr > ps.dStr;
		case DATATYPE::INT:return dInt > ps.dInt;
		case DATATYPE::DOUBLE:return dDouble > ps.dDouble;
		case DATATYPE::BOOL:return dBool > ps.dBool;
		case DATATYPE::DATE:return dDate > ps.dDate;
		}
	return false;
}

bool CSVData::operator<=(const CSVData& ps) const
{

	if (type == ps.type)
		switch (type)
		{
		case DATATYPE::STR:return dStr <= ps.dStr;
		case DATATYPE::INT:return dInt <= ps.dInt;
		case DATATYPE::DOUBLE:return dDouble <= ps.dDouble;
		case DATATYPE::BOOL:return dBool <= ps.dBool;
		case DATATYPE::DATE:return dDate <= ps.dDate;
		}
	return false;
}

bool CSVData::operator>=(const CSVData& ps) const
{

	if (type == ps.type)
		switch (type)
		{
		case DATATYPE::STR:return dStr >= ps.dStr;
		case DATATYPE::INT:return dInt >= ps.dInt;
		case DATATYPE::DOUBLE:return dDouble >= ps.dDouble;
		case DATATYPE::BOOL:return dBool >= ps.dBool;
		case DATATYPE::DATE:return dDate >= ps.dDate;
		}
	return false;
}

bool CSVData::operator!=(const CSVData& ps) const
{

	if (type == ps.type)
		switch (type)
		{
		case DATATYPE::STR:return dStr != ps.dStr;
		case DATATYPE::INT:return dInt != ps.dInt;
		case DATATYPE::DOUBLE:return dDouble != ps.dDouble;
		case DATATYPE::BOOL:return dBool != ps.dBool;
		case DATATYPE::DATE:return dDate != ps.dDate;
		}
	return false;
}


const CSVData CSVData::operator+(const CSVData& data) const
{
	if (type == data.type)
		switch (type)
		{
		case DATATYPE::STR:return CSVData(dStr + data.dStr);
		case DATATYPE::INT:return dInt + data.dInt;
		case DATATYPE::DOUBLE:return dDouble + data.dDouble;
		case DATATYPE::BOOL:throw UnexpectOperationExcetion();
		}
	else if(type == DATATYPE::INT && data.type == DATATYPE::DOUBLE)
		return dInt + data.dDouble;
	else if (data.type == DATATYPE::INT && type == DATATYPE::DOUBLE)
		return data.dInt + dDouble;
	throw UnexpectOperationExcetion();
}

const CSVData CSVData::operator-(const CSVData& data) const
{
	if (type == data.type)
		switch (type)
		{
		case DATATYPE::STR:throw UnexpectOperationExcetion();
		case DATATYPE::INT:return dInt - data.dInt;
		case DATATYPE::DOUBLE:return dDouble - data.dDouble;
		case DATATYPE::BOOL:throw UnexpectOperationExcetion();
		}
	else if (type == DATATYPE::INT && data.type == DATATYPE::DOUBLE)
		return dInt - data.dDouble;
	else if (data.type == DATATYPE::INT && type == DATATYPE::DOUBLE)
		return data.dInt - dDouble;
	throw UnexpectOperationExcetion();
}

const CSVData CSVData::operator*(const CSVData& data) const
{
	if (type == data.type)
		switch (type)
		{
		case DATATYPE::STR:throw UnexpectOperationExcetion();
		case DATATYPE::INT:return dInt * data.dInt;
		case DATATYPE::DOUBLE:return dDouble * data.dDouble;
		case DATATYPE::BOOL:throw UnexpectOperationExcetion();
		}
	else if (type == DATATYPE::INT && data.type == DATATYPE::DOUBLE)
		return dInt * data.dDouble;
	else if (data.type == DATATYPE::INT && type == DATATYPE::DOUBLE)
		return data.dInt * dDouble;
	throw UnexpectOperationExcetion();
}


const CSVData CSVData::operator/(const CSVData& data) const
{
	if (type == data.type)
		switch (type)
		{
		case DATATYPE::STR:throw UnexpectOperationExcetion();
		case DATATYPE::INT:return dInt / data.dInt;
		case DATATYPE::DOUBLE:return dDouble / data.dDouble;
		case DATATYPE::BOOL:throw UnexpectOperationExcetion();
		}
	else if (type == DATATYPE::INT && data.type == DATATYPE::DOUBLE)
		return dInt / data.dDouble;
	else if (data.type == DATATYPE::INT && type == DATATYPE::DOUBLE)
		return data.dInt / dDouble;
	throw UnexpectOperationExcetion();
}