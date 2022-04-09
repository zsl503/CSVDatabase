#pragma once
#include "global.h"
#include <vector>
using namespace std;

/**
 * @brief �й�csv�����������ռ�
*/
namespace CSVOperate
{
	/**
	 * @brief ����CSV����
	*/
	class CSV
	{
	public:
		/**
		 * @brief ��ʼ��CSV��������ANSI������ļ�
		 * @param path csv�ļ���·��
		 * @param types ÿһ�е�����
		*/
		CSV(wstring path, vector<DATATYPE> types):path(path), types(types){}
		
		/**
		 * @brief ����һ�����ݵ�csv�ļ�
		 * @param splits һ��csv���ݣ�����ÿһ�е����ݱ�����csv������һ��
		 * @param index ������кţ���0��ʼ������Ϊ-1����������һ�С�Ĭ��-1
		 * @param primaryIndex �����кţ���������ֵ����Ψһ���ܲ��룬��Ϊ-1���򲻽���������顣Ĭ��-1
		 * @exception InvalidValueException �ֶ����Ͳ�һ��, OpenFileException ���ļ�����, PrimaryKeyOverlapException ����ֵ�ظ�
		*/
		void insertLineToCSV(vector<CSVData> splits, int index = -1, int primaryIndex = -1);

		/**
		 * @brief ɾ��һ������
		 * @param index ɾ�����ݵ��к�
		 * @return ��ɾ��������
		 * @exception OpenFileException ���ļ�����
		*/
		vector<CSVData> deleteLineFromCSV(size_t index);

		/**
		 * @brief ��ȡȫ��csv���ݱ�
		 * @param skipLine ������������Ĭ��Ϊ0����������
		 * @return ȫ��csv����
		*/
		vector<vector<CSVData>> readCSV(size_t skipLine = 0);

	private:
		wstring wchars2wstring(vector<wchar_t> source);

		vector<wstring> csvSplit(const wstring& source);

		/**
		 * @brief һ��csv����תΪANSI�����csv�ı���
		 * @param splits һ��csv����
		 * @return csv�ı���
		*/
		string splits2CSVWstring(const vector<CSVData> splits);

		/**
		 * @brief csv�ļ�·��
		*/
		const wstring path;

		/**
		 * @brief csvÿһ������
		*/
		vector<DATATYPE> types;

	};
}
