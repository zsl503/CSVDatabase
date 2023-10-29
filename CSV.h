#pragma once
#include <vector>
#include "CSVType.h"

using namespace std;

/**
 * @brief 有关csv操作的命名空间
*/
namespace csvop
{
	// CSV 编码类型
	enum class CSVCode
	{
		UTF8,
		UNIC,
		ANSI
	};
	/**
	 * @brief 操作CSV的类
	*/
	class CSV
	{
	public:
		/**
		 * @brief 初始化CSV，必须是ANSI编码的文件
		 * @param path csv文件的路径
		 * @param types 每一列的类型
		*/
		CSV(wstring path, vector<DATATYPE> types, CSVCode code = CSVCode::UTF8);
		
		/**
		 * @brief 插入一行数据到csv文件
		 * @param splits 一行csv数据，其中每一列的数据必须与csv表类型一致
		 * @param index 插入的行号，从0开始数，若为-1，则插入最后一行。默认-1
		 * @param primaryIndex 主键列号，表中主键值必须唯一才能插入，若为-1，则不进行主键检查。默认-1
		 * @exception InvalidValueException 字段类型不一致, OpenFileException 打开文件出错, PrimaryKeyOverlapException 主键值重复
		*/
		void insertLine(vector<CSVData> splits, int index = -1, vector<int> primaryIndexs = {}, int skipLine = 0);

		/**
		 * @brief 更新一行数据
		 * @param index 更新数据的行号
		 * @exception OpenFileException 打开文件错误
		*/
		void updateLine(vector<CSVData> splits, int index, vector<int> primaryIndexs = {}, int skipLine = 0);

		/**
		 * @brief 删除一行数据
		 * @param index 删除数据的行号
		 * @return 被删除的数据
		 * @exception OpenFileException 打开文件错误
		*/
		vector<CSVData> deleteLine(size_t index);

		/**
		 * @brief 读取全部csv数据表
		 * @param skipLine 跳过的行数，默认为0，即不跳过
		 * @return 全部csv数据
		*/
		vector<vector<CSVData>> read(size_t skipLine = 0);

	private:
		wstring wchars2wstring(vector<wchar_t> source);

		vector<wstring> csvSplit(const wstring& source);

		/**
		 * @brief 一行csv数据转为ANSI编码的csv文本串
		 * @param splits 一行csv数据
		 * @return csv文本串
		*/
		wstring splits2CSVWstring(const vector<CSVData> splits);

		/**
		 * @brief csv文件路径
		*/
		const wstring path;

		/**
		 * @brief csv每一列类型
		*/
		vector<DATATYPE> types;

		CSVCode code;

		wstring(*convert2Unicode)(const string& wstr);

		/**
		 * @brief 更新一行数据
		 * @param mode mdoe == 1为更新，mode == 0为插入
		 * @param index 更新数据的行号
		 * @exception OpenFileException 打开文件错误
		*/
		void _updateLine(vector<CSVData> splits,int mode, int index = -1, vector<int> primaryIndexs = {}, int skipLine = 0);

	};
}
