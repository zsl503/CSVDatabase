#pragma once
#include "global.h"
#include <vector>
#include "TableHeader.h"
#include "DataFilter.h"
namespace CSVDatabase
{
	namespace Table
	{
		class TableData
		{
		public:
			TableData(TableHeader header, std::vector<std::vector<std::wstring>> datas) :header(header), datas(datas) {}

			TableData select(const std::vector<std::wstring>& fields);

			static TableData select(const std::vector<std::wstring>& fields, TableData data);

			TableData where();

			TableData groupby();

			const TableData& operator[] (const std::wstring) const;

			const TableData& operator&& (const TableData) const;

			const TableData& operator|| (const TableData) const;

			const TableData& operator< (const TableData) const;

			const TableData& operator> (const TableData) const;

			const TableData& operator== (const TableData) const;

			const TableData& operator!= (const TableData) const;


		private:
			TableHeader header;
			std::vector<std::vector<std::wstring>> datas;
			std::wstring currentField = nullptr;
		};

	}

}
