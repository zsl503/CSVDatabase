#pragma once
#include <vector>
#include "CSV.h"
#include "TableHeader.h"
namespace CSVDatabase
{
	/**
	 * @brief ����CSV�Ŀ���΢�����ݿ�
	*/
	class Database
	{
	public:
		/**
		 * @brief ��ʼ�����ݿ�
		 * @param dbPath ���ݿ�·��
		 * @exception DatabaseBrokenException ���ݿ�·������/���ݿⲻ����/������
		*/
		Database(std::wstring dbPath);

		/**
		 * @brief �������ݿ⣬����dbPath·���´����ļ��� dbPath/headers��dbPath/tables ���ļ� dbPath/tables.csv
		 * @param dbPath ��·��
		 * TODO:δ����tables.csvʱ������Database�󴴽�������
		*/
		static void createDatabase(std::wstring dbPath);

		/**
		 * @brief ����һ�ű�
		 * @param tableName ����
		 * @param fields ����������ֶ��б�
		 * @exception TableHasExsistExption �ظ�����
		 * InvalidHeaderException δָ�������������������࣬
		 * InvalidValueException �Ƿ����ֵ
		*/
		void createTable(std::wstring tableName, std::vector<Table::Field> fields);

		void removeTable(std::wstring tableName);

		void showTables();

		/**
		 * @brief ��ȡ��ͷ��
		 * @param tableName ���� 
		 * @return ��ͷ��
		*/
		Table::TableHeader getTableHeader(std::wstring tableName);
		

	private:
		std::wstring dbPath;

		std::wstring tablesPath;

		std::wstring headersPath;

		std::vector<std::wstring> tableList;

		CSVOperate::CSV dbCSV;
	};
}


