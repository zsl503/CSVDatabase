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
			TableData(TableHeader header, std::vector<std::vector<CSVOperate::CSVData>> datas) :header(header), datas(datas) {}

			TableData select(const std::vector<std::wstring>& fields);

			static TableData select(const std::vector<std::wstring>& fields, TableData data);

			TableData where();

			TableData groupby();

			const FilterGetter operator[] (const std::wstring) const;

			//std::vector<CSVOperate::CSVData>& operator[] (int);

			std::vector<CSVOperate::CSVData>& operator[] (int);

			//TableData operator[] (const std::wstring&);

			TableData operator[] (const FieldGetter&);

			TableData operator[] (const DataFilter&);


		private:
			TableHeader header;
			std::vector<std::vector<CSVOperate::CSVData>> datas;
		};

	}

}
