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
			Field(std::wstring name, CSVOperate::DATATYPE type, bool isPrimary, std::wstring foreignKey = L"");
			std::wstring getForeignKeyString();
			std::wstring getTypeString();
			std::wstring getNameString();
			std::wstring getIsPrimaryString();
			bool isPrimary();
		private:
			std::wstring name;
			CSVOperate::DATATYPE type;
			bool isPrimaryKey;
			std::wstring foreignKey[2];
		};

		class TableHeader
		{
		public:
			TableHeader(std::vector<Field> fields) : fields(fields) {}
			Field getFieldByName(std::wstring name);
			bool isFieldExsist(std::wstring name);
		private:
			std::vector<Field> fields;
		};
	}
}


