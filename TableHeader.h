#pragma once
#include <vector>
#include "global.h"
namespace CSVDatabase
{
	namespace Table
	{
		/**
		 * @brief 字段类，指定表的某一字段属性和配置
		*/
		class Field
		{
		public:
			Field(std::wstring name, CSVOperate::DATATYPE type, bool isPrimary, int index, std::wstring foreignKey = L"");
			const std::wstring getForeignKeyString() const;
			const std::wstring getTypeString() const;
			const std::wstring getNameString() const;
			const std::wstring getIsPrimaryString() const;
			bool isPrimary();
			const size_t getIndex();
		private:
			std::wstring name;
			CSVOperate::DATATYPE type;
			bool isPrimaryKey;
			std::wstring foreignKey[2];
			std::size_t index;
		};

		class TableHeader
		{
		public:
			TableHeader() :fields(){}
			TableHeader(std::vector<Field> fields) : fields(fields) {}
			TableHeader(const TableHeader& header) : fields(header.fields) {}
			Field& operator[] (const std::wstring);

			bool isFieldExsist(std::wstring name);
		private:
			std::vector<Field> fields;
		};
	}
}


