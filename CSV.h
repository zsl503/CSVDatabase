#pragma once
#include "global.h"
#include <vector>
using namespace std;

/**
 * @brief 有关csv操作的命名空间
*/
namespace CSVOperate
{
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
		CSV(wstring path, vector<DATATYPE> types):path(path), types(types){}
		
		/**
		 * @brief 插入一行数据到csv文件
		 * @param splits 一行csv数据，其中每一列的数据必须与csv表类型一致
		 * @param index 插入的行号，从0开始数，若为-1，则插入最后一行。默认-1
		 * @param primaryIndex 主键列号，表中主键值必须唯一才能插入，若为-1，则不进行主键检查。默认-1
		 * @exception InvalidValueException 字段类型不一致, OpenFileException 打开文件出错, PrimaryKeyOverlapException 主键值重复
		*/
		void insertLineToCSV(vector<CSVData> splits, int index = -1, int primaryIndex = -1);

		/**
		 * @brief 删除一行数据
		 * @param index 删除数据的行号
		 * @return 被删除的数据
		 * @exception OpenFileException 打开文件错误
		*/
		vector<CSVData> deleteLineFromCSV(size_t index);

		/**
		 * @brief 读取全部csv数据表
		 * @param skipLine 跳过的行数，默认为0，即不跳过
		 * @return 全部csv数据
		*/
		vector<vector<CSVData>> readCSV(size_t skipLine = 0);

	private:
		wstring wchars2wstring(vector<wchar_t> source);

		vector<wstring> csvSplit(const wstring& source);

		/**
		 * @brief 一行csv数据转为ANSI编码的csv文本串
		 * @param splits 一行csv数据
		 * @return csv文本串
		*/
		string splits2CSVWstring(const vector<CSVData> splits);

		/**
		 * @brief csv文件路径
		*/
		const wstring path;

		/**
		 * @brief csv每一列类型
		*/
		vector<DATATYPE> types;

	};
}
