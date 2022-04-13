#pragma once
#include <vector>
#include "CSV.h"
#include "TableHeader.h"

namespace CSVDatabase
{
	namespace Table
	{
		class FilterGetter;
		class VerifyFilter;
		class DataFilter;

		class DataFilter
		{
		public:
			virtual DataFilter* copy() const = 0;
			virtual const bool verify(const vector<CSVOperate::CSVData>& datas) const = 0;
			virtual const TableHeader getHeader() const = 0;
		};

		class UnionFilter :public DataFilter
		{
		private:
			UnionFilter() {}
			DataFilter* filter = nullptr;
			DataFilter* filter2 = nullptr;
		public:
			enum class Type
			{
				AND,
				OR,
				NOT,
				SINGLE
			}type;
			DataFilter* copy() const;
			const TableHeader getHeader() const;
			const bool verify(const vector<CSVOperate::CSVData>& datas) const;
			~UnionFilter();
			UnionFilter(const DataFilter& filter, const DataFilter& filter2, Type type);
			UnionFilter(const DataFilter& filter, Type type);
			UnionFilter(const UnionFilter&);
			UnionFilter(const VerifyFilter&);
		};

		class VerifyFilter :public DataFilter
		{
		public:
			enum class Type
			{
				GREATER,
				EQUAL,
				LESS,
				GREATEREQUAL,
				LESSEQUAL,
				NOTEQUAL
			}type;
			DataFilter* copy() const;
			const TableHeader getHeader() const;
			const bool verify(const vector<CSVOperate::CSVData>& datas) const;
			~VerifyFilter();
			VerifyFilter(const VerifyFilter&);
			VerifyFilter(const FilterGetter* getter, const FilterGetter* getter2, Type type);

		private:
			VerifyFilter() {}
			const FilterGetter* getter = nullptr;
			const FilterGetter* getter2 = nullptr;
		};

		class FieldGetter
		{
		public:
			virtual const CSVOperate::CSVData verify(const vector<CSVOperate::CSVData>& datas) const = 0;

			virtual const TableHeader getHeader() const = 0;

			virtual const TableHeader getNewHeader()const = 0;
		};

		class FilterGetter:public FieldGetter
		{
		public:
			const TableHeader getHeader() const;
			const TableHeader getNewHeader() const;
			const CSVOperate::CSVData verify(const vector<CSVOperate::CSVData>& datas) const;
			FilterGetter(TableHeader header, const std::wstring& name)
				:name(name), type(Type::SINGLE),header(header), FieldGetter() {}
			
			FilterGetter(CSVOperate::CSVData data)
				:data(data), type(Type::SINGLE),isValue(true), FieldGetter() {}

			FilterGetter(int data) :FilterGetter(CSVOperate::CSVData(data)) {}

			FilterGetter(double data) :FilterGetter(CSVOperate::CSVData(data)) {}
			
			~FilterGetter();

			friend const FilterGetter operator+(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator-(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator*(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator/(const FilterGetter& getter, const FilterGetter& getter2);
		private:
			enum class Type
			{
				ADD,
				SUB,
				MUL,
				DIV,
				SINGLE
			}type;

			FilterGetter(const FilterGetter& getter, FilterGetter* filter2,Type type)
				: name(getter.name), header(getter.header), data(getter.data),
				isValue(getter.isValue), isTmp(true), filter2(filter2), FieldGetter(),type(type) {}

			const CSVOperate::CSVData& getValue(const vector<CSVOperate::CSVData>& datas) const;
			
			std::wstring name;
			
			CSVOperate::CSVData data;
			
			TableHeader header;
			
			FilterGetter* filter2 = nullptr;
			
			bool isTmp = false;
			
			bool isValue = false;

		};

		const UnionFilter operator&&(const DataFilter& filter, const DataFilter& filter2);
		const UnionFilter operator||(const DataFilter& filter, const DataFilter& filter2);
		const UnionFilter operator!(const DataFilter& filter);

		const VerifyFilter operator==(const FilterGetter& getter, const FilterGetter& getter2);
		const VerifyFilter operator>(const FilterGetter& getter, const FilterGetter& getter2);
		const VerifyFilter operator<(const FilterGetter& getter, const FilterGetter& getter2);
		const VerifyFilter operator>=(const FilterGetter& getter, const FilterGetter& getter2);
		const VerifyFilter operator<=(const FilterGetter& getter, const FilterGetter& getter2);
		const VerifyFilter operator!=(const FilterGetter& getter, const FilterGetter& getter2);

	}
}