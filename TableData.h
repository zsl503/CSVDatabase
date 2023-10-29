#pragma once
#include <vector>
#include "TableHeader.h"
#include "DataFilter.h"
namespace csvdb
{
	namespace table
	{
		class TableData
		{
		public:
			// 构造TableData
			TableData(TableHeader header, std::vector<std::vector<csvop::CSVData>> datas) 
				:header(header), datas(datas) {}
			TableData() :header(), datas() {}

			TableData groupby();
			const FilterGetter operator[] (const std::wstring) const;	// 获取字段
			std::vector<csvop::CSVData>& operator[] (int);	// 获取某行数据
			TableData operator[] (const FieldGetter&);	// 筛选列
			TableData operator[] (const DataFilter&);	// 条件筛选
			size_t length() const;	// 获取长度
			const TableHeader getHeader() const;	// 获取表头
			friend TableData operator+(const TableData& td1, const TableData& td2);
			bool push(std::vector<csvop::CSVData>);
			void get(const DataFilter&);
			void clear();
		private:
			TableHeader header;	// 表头
			std::vector<std::vector<csvop::CSVData>> datas;	// 表数据
		};
	}

}

