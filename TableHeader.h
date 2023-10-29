#pragma once
#include <vector>
#include <string>
#include "csvop.h"

namespace csvdb
{
	namespace table
	{
		/**
		 * @brief 字段类，指定表的某一字段属性和配置
		*/
		class Field
		{
		public:
			Field(std::wstring name, csvop::DATATYPE type, bool isPrimary, int index, std::wstring foreignKey = L"", bool selfIncrease = false,int increaseNum = INFINITY);
			const std::wstring getForeignKeyString() const;
			const std::wstring getForeignTable() const;
			const std::wstring getForeignField() const;
			const std::wstring getTypeString() const;
			const csvop::DATATYPE getType() const;
			const std::wstring getNameString() const;
			const std::wstring getIsPrimaryString() const;
			const std::wstring getSelfIncreaseString() const;
			const std::wstring getIncreaseString() const;
			const int getIncrease() const;
			void increase();

			bool isSelfIncrease() const;
			bool isPrimary();
			bool operator==(const Field& hd) const;
			bool operator!=(const Field& hd) const;
		private:
			std::wstring name;
			csvop::DATATYPE type;
			bool isPrimaryKey;
			bool selfIncrease;
			int increaseNum;
			std::wstring foreignKey[2];
			std::size_t index;
		};

		class TableHeader
		{
		public:
			TableHeader() :fields(){}
			TableHeader(std::vector<Field> fields) : fields(fields) {}
			TableHeader(const TableHeader& header) : fields(header.fields) {}

			// 当TableHeader含index时，不会返回index的type
			std::vector<csvop::DATATYPE> getTypes();

			// 返回全部Field，当含index时，会返回index的Field
			const std::vector<Field> getFields() const;

			// 返回在header中Primarykey的序号，当header含index时，序列号0为index
			std::vector<int> getPrimaryIndexs();

			// 返回在header中的序号
			size_t getIndex(const std::wstring& name) const;
			Field& operator[] (const std::wstring);
			bool isFieldExsist(std::wstring name);
			bool operator==(const TableHeader& hd) const;
			bool operator!=(const TableHeader& hd) const;
		private:
			std::vector<Field> fields;
		};
	}
}


