#pragma once
#include <vector>
#include "CSVType.h"

using namespace std;

/**
 * @brief �й�csv�����������ռ�
*/
namespace csvop
{
	// CSV ��������
	enum class CSVCode
	{
		UTF8,
		UNIC,
		ANSI
	};
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
		CSV(wstring path, vector<DATATYPE> types, CSVCode code = CSVCode::UTF8);
		
		/**
		 * @brief ����һ�����ݵ�csv�ļ�
		 * @param splits һ��csv���ݣ�����ÿһ�е����ݱ�����csv������һ��
		 * @param index ������кţ���0��ʼ������Ϊ-1����������һ�С�Ĭ��-1
		 * @param primaryIndex �����кţ���������ֵ����Ψһ���ܲ��룬��Ϊ-1���򲻽���������顣Ĭ��-1
		 * @exception InvalidValueException �ֶ����Ͳ�һ��, OpenFileException ���ļ�����, PrimaryKeyOverlapException ����ֵ�ظ�
		*/
		void insertLine(vector<CSVData> splits, int index = -1, vector<int> primaryIndexs = {}, int skipLine = 0);

		/**
		 * @brief ����һ������
		 * @param index �������ݵ��к�
		 * @exception OpenFileException ���ļ�����
		*/
		void updateLine(vector<CSVData> splits, int index, vector<int> primaryIndexs = {}, int skipLine = 0);

		/**
		 * @brief ɾ��һ������
		 * @param index ɾ�����ݵ��к�
		 * @return ��ɾ��������
		 * @exception OpenFileException ���ļ�����
		*/
		vector<CSVData> deleteLine(size_t index);

		/**
		 * @brief ��ȡȫ��csv���ݱ�
		 * @param skipLine ������������Ĭ��Ϊ0����������
		 * @return ȫ��csv����
		*/
		vector<vector<CSVData>> read(size_t skipLine = 0);

	private:
		wstring wchars2wstring(vector<wchar_t> source);

		vector<wstring> csvSplit(const wstring& source);

		/**
		 * @brief һ��csv����תΪANSI�����csv�ı���
		 * @param splits һ��csv����
		 * @return csv�ı���
		*/
		wstring splits2CSVWstring(const vector<CSVData> splits);

		/**
		 * @brief csv�ļ�·��
		*/
		const wstring path;

		/**
		 * @brief csvÿһ������
		*/
		vector<DATATYPE> types;

		CSVCode code;

		wstring(*convert2Unicode)(const string& wstr);

		/**
		 * @brief ����һ������
		 * @param mode mdoe == 1Ϊ���£�mode == 0Ϊ����
		 * @param index �������ݵ��к�
		 * @exception OpenFileException ���ļ�����
		*/
		void _updateLine(vector<CSVData> splits,int mode, int index = -1, vector<int> primaryIndexs = {}, int skipLine = 0);

	};
}
