#pragma once
#include "csvop.h"
#include "TableData.h"
#include <set>
namespace csvdb
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
		void createTable(std::wstring tableName, std::vector<table::Field> fields);

		//TableData select(std::wstring, std::wstring,);

		void removeTable(std::wstring tableName);	// ɾ����

		std::set<std::wstring> getTableList();	// ��ȡ���б�

		bool tableExist(std::wstring tableName);	// ���ݱ��Ƿ����

		/**
		 * @brief ��ȡ��ͷ��
		 * @param tableName ���� 
		 * @return ��ͷ��
		*/
		table::TableHeader getTableHeader(std::wstring tableName, bool isIndex = true);

		bool insert(std::wstring tableName, std::vector<std::wstring> row);	// ����һ������

		bool insert(std::wstring tableName, std::vector<csvop::CSVData> row);	// �����������

		bool insert(std::wstring tableName,const std::vector<std::wstring>& fieldName, const std::vector<csvop::CSVData>& data, std::vector<csvop::CSVData>* = NULL);	// ��������

		bool drop(std::wstring tableName,const table::DataFilter& );	// ɾ��ָ����������

		int update(std::wstring tableName, std::vector<std::wstring> fieldName, std::vector<csvop::CSVData> data , const table::DataFilter&);	// ��������
		//std::vector <bool> insert(std::wstring tableName, std::vector <std::vector<std::wstring>> rows);
		
		table::TableData operator[](const std::wstring);


	private:
		std::wstring dbPath;

		std::wstring tablesPath;

		std::wstring headersPath;

		std::set<std::wstring> tableList;

		csvop::CSV dbCSV;

		bool checkForeignKey(std::wstring tableName, std::wstring fieldName, csvop::CSVData data);

		bool updateHeader(std::wstring tableName, std::wstring headerName, std::vector<std::wstring> fieldName, std::vector<csvop::CSVData> data);	// ��������
	};
}