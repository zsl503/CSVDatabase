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
			// ����TableData
			TableData(TableHeader header, std::vector<std::vector<csvop::CSVData>> datas) 
				:header(header), datas(datas) {}
			TableData() :header(), datas() {}

			TableData groupby();
			const FilterGetter operator[] (const std::wstring) const;	// ��ȡ�ֶ�
			std::vector<csvop::CSVData>& operator[] (int);	// ��ȡĳ������
			TableData operator[] (const FieldGetter&);	// ɸѡ��
			TableData operator[] (const DataFilter&);	// ����ɸѡ
			size_t length() const;	// ��ȡ����
			const TableHeader getHeader() const;	// ��ȡ��ͷ
			friend TableData operator+(const TableData& td1, const TableData& td2);
			bool push(std::vector<csvop::CSVData>);
			void get(const DataFilter&);
			void clear();
		private:
			TableHeader header;	// ��ͷ
			std::vector<std::vector<csvop::CSVData>> datas;	// ������
		};
	}

}

