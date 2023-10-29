#pragma once
#include <vector>
#include "TableHeader.h"
namespace csvdb
{
	using std::vector;
	using std::wstring;
	namespace table
	{
		class FilterGetter;
		class VerifyFilter;
		class DataFilter;

		class DataFilter
		{
		public:
			virtual DataFilter* copy() const = 0;	// 拷贝
			virtual const bool verify(const vector<csvop::CSVData>& datas) const = 0;	// 验证数据
			virtual const TableHeader getHeader() const = 0;	// 获取筛选后的新表头
			virtual void updateHeader(TableHeader header) = 0;	// 更新表头
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
			void updateHeader(TableHeader header);
			const bool verify(const vector<csvop::CSVData>& datas) const;
			~UnionFilter();
			UnionFilter(const DataFilter& filter, const DataFilter& filter2, Type type);
			UnionFilter(const DataFilter& filter, Type type);
			UnionFilter(const UnionFilter&);
			UnionFilter(const VerifyFilter&);

			UnionFilter(const FilterGetter& getter);
			UnionFilter(csvop::CSVData);
			UnionFilter(int);
			UnionFilter(double);
			UnionFilter(wstring);
			UnionFilter(bool);
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
				NOTEQUAL,
				SINGLE
			}type;
			DataFilter* copy() const;
			const TableHeader getHeader() const;
			void updateHeader(TableHeader header);
			const bool verify(const vector<csvop::CSVData>& datas) const;
			~VerifyFilter();
			VerifyFilter(const VerifyFilter&);
			VerifyFilter(const FilterGetter& getter);
			VerifyFilter(const FilterGetter* getter, const FilterGetter* getter2, Type type);

		private:
			VerifyFilter() {}
			FilterGetter* getter = nullptr;
			FilterGetter* getter2 = nullptr;
		};

		class FieldGetter
		{
		public:
			virtual const csvop::CSVData verify(const vector<csvop::CSVData>& datas) const = 0;

			virtual const TableHeader getHeader() const = 0;

			virtual const TableHeader getNewHeader()const = 0;

			virtual void updateHeader(TableHeader header) = 0;
		};

		class FilterGetter:public FieldGetter
		{
		public:
			enum class Type
			{
				ADD,
				SUB,
				MUL,
				DIV,
				Union,
				SINGLE
			}type;
			const TableHeader getHeader() const;
			const TableHeader getNewHeader() const;
			void updateHeader(TableHeader header);
			const vector<csvop::CSVData> getUnion(const vector<csvop::CSVData>& datas) const;
			const csvop::CSVData verify(const vector<csvop::CSVData>& datas) const;
			FilterGetter(TableHeader header, const std::wstring& name)
				:name(name), type(Type::SINGLE),header(header), FieldGetter() {}
			
			FilterGetter(csvop::CSVData data)
				:data(data), type(Type::SINGLE),isValue(true), FieldGetter() {}

			FilterGetter(const std::wstring& data) :FilterGetter(csvop::CSVData(data)) {}

			FilterGetter(int data) :FilterGetter(csvop::CSVData(data)) {}

			FilterGetter(double data) :FilterGetter(csvop::CSVData(data)) {}
			
			FilterGetter(bool data) :FilterGetter(csvop::CSVData(data)) {}
			
			~FilterGetter();

			friend const FilterGetter operator+(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator-(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator*(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator/(const FilterGetter& getter, const FilterGetter& getter2);
			friend const FilterGetter operator&&(const FilterGetter& getter, const FilterGetter& getter2);
		private:

			FilterGetter(const FilterGetter& getter, FilterGetter* getter2,Type type)
				: name(getter.name), header(getter.header), data(getter.data),
				isValue(getter.isValue), isTmp(true), getter2(getter2), FieldGetter(),type(type) {}

			const csvop::CSVData& getValue(const vector<csvop::CSVData>& datas) const;

			std::wstring name;
			csvop::CSVData data;
			TableHeader header;
			FilterGetter* getter2 = nullptr;
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
//typedef class CSVDatabase::Table::FilterGetter FG;

