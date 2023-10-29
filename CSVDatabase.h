#pragma once
#include "csvop.h"
#include "TableData.h"
#include <set>
namespace csvdb
{
	/**
	 * @brief 基于CSV的可视微型数据库
	*/
	class Database
	{
	public:
		/**
		 * @brief 初始化数据库
		 * @param dbPath 数据库路径
		 * @exception DatabaseBrokenException 数据库路径错误/数据库不存在/数据损坏
		*/
		Database(std::wstring dbPath);

		/**
		 * @brief 创建数据库，会在dbPath路径下创建文件夹 dbPath/headers、dbPath/tables 和文件 dbPath/tables.csv
		 * @param dbPath 表路径
		 * TODO:未存在tables.csv时，创建Database后创建表会出错
		*/
		static void createDatabase(std::wstring dbPath);

		/**
		 * @brief 创建一张表
		 * @param tableName 表名
		 * @param fields 所创建表的字段列表
		 * @exception TableHasExsistExption 重复建表，
		 * InvalidHeaderException 未指定主键或主键数量过多，
		 * InvalidValueException 非法外键值
		*/
		void createTable(std::wstring tableName, std::vector<table::Field> fields);

		//TableData select(std::wstring, std::wstring,);

		void removeTable(std::wstring tableName);	// 删除表

		std::set<std::wstring> getTableList();	// 获取表列表

		bool tableExist(std::wstring tableName);	// 数据表是否存在

		/**
		 * @brief 获取表头类
		 * @param tableName 表名 
		 * @return 表头类
		*/
		table::TableHeader getTableHeader(std::wstring tableName, bool isIndex = true);

		bool insert(std::wstring tableName, std::vector<std::wstring> row);	// 插入一行数据

		bool insert(std::wstring tableName, std::vector<csvop::CSVData> row);	// 插入多行数据

		bool insert(std::wstring tableName,const std::vector<std::wstring>& fieldName, const std::vector<csvop::CSVData>& data, std::vector<csvop::CSVData>* = NULL);	// 更新数据

		bool drop(std::wstring tableName,const table::DataFilter& );	// 删除指定条件数据

		int update(std::wstring tableName, std::vector<std::wstring> fieldName, std::vector<csvop::CSVData> data , const table::DataFilter&);	// 更新数据
		//std::vector <bool> insert(std::wstring tableName, std::vector <std::vector<std::wstring>> rows);
		
		table::TableData operator[](const std::wstring);


	private:
		std::wstring dbPath;

		std::wstring tablesPath;

		std::wstring headersPath;

		std::set<std::wstring> tableList;

		csvop::CSV dbCSV;

		bool checkForeignKey(std::wstring tableName, std::wstring fieldName, csvop::CSVData data);

		bool updateHeader(std::wstring tableName, std::wstring headerName, std::vector<std::wstring> fieldName, std::vector<csvop::CSVData> data);	// 更新数据
	};
}