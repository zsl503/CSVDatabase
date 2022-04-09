#pragma once
#include <vector>
#include "CSV.h"
#include "TableHeader.h"
namespace CSVDatabase
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
		void createTable(std::wstring tableName, std::vector<Table::Field> fields);

		void removeTable(std::wstring tableName);

		void showTables();

		/**
		 * @brief 获取表头类
		 * @param tableName 表名 
		 * @return 表头类
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


