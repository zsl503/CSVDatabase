#include "DataFilter.h"
using namespace CSVDatabase::Table;

UnionFilter::UnionFilter(const DataFilter& filter, const DataFilter& filter2, Type type)
	:type(type), filter(filter.copy()), filter2(filter2.copy()) {}

UnionFilter::UnionFilter(const DataFilter& filter, Type type)
	: type(type), filter(filter.copy()) {}

UnionFilter::UnionFilter(const UnionFilter& filter)
	: type(filter.type), filter(filter.filter->copy()),filter2(filter.filter2->copy())
{}

UnionFilter::UnionFilter(const VerifyFilter& filter)
	: type(Type::SINGLE), filter(filter.copy()){}

DataFilter* UnionFilter::copy() const
{
	UnionFilter* f = new UnionFilter();
	if(filter!=nullptr)
		f->filter = filter->copy();
	if (filter2 != nullptr)
		f->filter2 = filter2->copy();
	f->type = type;
	return f;
}

UnionFilter::~UnionFilter()
{
	delete filter;
	delete filter2;
}

const bool UnionFilter::verify(const vector<CSVOperate::CSVData>& datas) const
{
	//CSVOperate::CSVData tmp = getValue(datas);
	switch (type)
	{
	case Type::AND:
		return filter->verify(datas) && filter2->verify(datas);
	case Type::OR:
		return filter->verify(datas) || filter2->verify(datas);
	case Type::NOT:
		return !filter->verify(datas);
	case Type::SINGLE:
		return filter->verify(datas);
	default:
		return false;
	}
}

const TableHeader UnionFilter::getHeader() const
{
	return filter->getHeader();
}
DataFilter* VerifyFilter::copy() const
{
	VerifyFilter* f = new VerifyFilter();
	if (getter != nullptr)
		f->getter = new FilterGetter(*getter);
	if (getter2 != nullptr)
		f->getter2 = new FilterGetter(*getter2);
	f->type = type;
	return f;
}

VerifyFilter::VerifyFilter(const FilterGetter* getter, const FilterGetter* getter2, Type type)
	:type(type), getter(new FilterGetter(*getter)), getter2(new FilterGetter(*getter2)) {}

VerifyFilter::VerifyFilter(const VerifyFilter& filter)
	:type(filter.type), getter(new FilterGetter(*filter.getter)),
	getter2(new FilterGetter(*filter.getter2)) {}

VerifyFilter::~VerifyFilter()
{
	delete getter;
	delete getter2;
}

const bool VerifyFilter::verify(const vector<CSVOperate::CSVData>& datas) const
{
	//CSVOperate::CSVData tmp = getValue(datas);
	switch (type)
	{
	case Type::EQUAL:
		return getter->verify(datas) == getter2->verify(datas);
	case Type::GREATER:
		return getter->verify(datas) > getter2->verify(datas);
	case Type::LESS:
		return getter->verify(datas) < getter2->verify(datas);
	case Type::GREATEREQUAL:
		return getter->verify(datas) >= getter2->verify(datas);
	case Type::LESSEQUAL:
		return getter->verify(datas) <= getter2->verify(datas);
	case Type::NOTEQUAL:
		return getter->verify(datas) != getter2->verify(datas);
	default:
		return false;
	}
}

const TableHeader VerifyFilter::getHeader() const
{
	return getter->getHeader();
}

FilterGetter::~FilterGetter()
{
	if(!isTmp)
		delete filter2;
}

const TableHeader FilterGetter::getNewHeader() const
{
	return TableHeader({const_cast<TableHeader&>(header)[name]});
}

const TableHeader FilterGetter::getHeader() const
{
	return header;
}

const CSVOperate::CSVData& FilterGetter::getValue(const vector<CSVOperate::CSVData>& datas) const
{
	if(isValue)
		return data;
	return datas[const_cast<TableHeader&>(header)[name].getIndex()];
}

const CSVOperate::CSVData FilterGetter::verify(const vector<CSVOperate::CSVData> &datas) const
{
	CSVOperate::CSVData tmp = getValue(datas);
	switch (type)
	{
	case Type::ADD:
		return getValue(datas) + filter2->verify(datas);
	case Type::SUB:
		return getValue(datas) - filter2->verify(datas);
	case Type::MUL:
		return getValue(datas) * filter2->verify(datas);
	case Type::DIV:
		return getValue(datas) / filter2->verify(datas);
	case Type::SINGLE:
		return getValue(datas);
	default:
		return CSVOperate::CSVData();
	}
}

const UnionFilter CSVDatabase::Table::operator&&(const DataFilter& filter, const DataFilter& filter2)
{
	return UnionFilter(filter, filter2, UnionFilter::Type::AND);
}

const UnionFilter CSVDatabase::Table::operator||(const DataFilter& filter, const DataFilter& filter2)
{
	return UnionFilter(filter, filter2, UnionFilter::Type::OR);
}

const UnionFilter CSVDatabase::Table::operator!(const DataFilter& filter)
{
	return UnionFilter(filter, UnionFilter::Type::NOT);
}

const VerifyFilter CSVDatabase::Table::operator==(const FilterGetter& getter, const FilterGetter& getter2)
{
	return VerifyFilter(&getter, &getter2, VerifyFilter::Type::EQUAL);
}
const VerifyFilter CSVDatabase::Table::operator>(const FilterGetter& getter, const FilterGetter& getter2)
{
	return VerifyFilter(&getter, &getter2, VerifyFilter::Type::GREATER);
}
const VerifyFilter CSVDatabase::Table::operator<(const FilterGetter& getter, const FilterGetter& getter2)
{
	return VerifyFilter(&getter, &getter2, VerifyFilter::Type::EQUAL);
}
const VerifyFilter CSVDatabase::Table::operator>=(const FilterGetter& getter, const FilterGetter& getter2)
{
	return VerifyFilter(&getter, &getter2, VerifyFilter::Type::GREATEREQUAL);
}
const VerifyFilter CSVDatabase::Table::operator<=(const FilterGetter& getter, const FilterGetter& getter2)
{
	return VerifyFilter(&getter, &getter2, VerifyFilter::Type::LESSEQUAL);
}
const VerifyFilter CSVDatabase::Table::operator!=(const FilterGetter& getter, const FilterGetter& getter2)
{
	return VerifyFilter(&getter, &getter2, VerifyFilter::Type::NOTEQUAL);
}

const FilterGetter CSVDatabase::Table::operator+(const FilterGetter& getter, const FilterGetter& getter2)
{
	return FilterGetter(getter, new FilterGetter(getter2), FilterGetter::Type::ADD);
}
const FilterGetter CSVDatabase::Table::operator-(const FilterGetter& getter, const FilterGetter& getter2)
{
	return FilterGetter(getter, new FilterGetter(getter2), FilterGetter::Type::SUB);
}
const FilterGetter CSVDatabase::Table::operator*(const FilterGetter& getter, const FilterGetter& getter2)
{
	return FilterGetter(getter, new FilterGetter(getter2), FilterGetter::Type::MUL);
}
const FilterGetter CSVDatabase::Table::operator/(const FilterGetter& getter, const FilterGetter& getter2)
{
	return FilterGetter(getter, new FilterGetter(getter2), FilterGetter::Type::DIV);
}