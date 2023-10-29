#pragma once
#include <vector>
#include <string>
#include "csvop.h"

namespace csvdb
{
	namespace table
	{
		/**
		 * @brief �ֶ��ָ࣬�����ĳһ�ֶ����Ժ�����
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

			// ��TableHeader��indexʱ�����᷵��index��type
			std::vector<csvop::DATATYPE> getTypes();

			// ����ȫ��Field������indexʱ���᷵��index��Field
			const std::vector<Field> getFields() const;

			// ������header��Primarykey����ţ���header��indexʱ�����к�0Ϊindex
			std::vector<int> getPrimaryIndexs();

			// ������header�е����
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


