#include "csvdbexceptions.h"
#include "codeConvert.h"
#include <fstream>
using std::wstring;
using csvdb::CSVDbException;

CSVDbException::CSVDbException(unsigned int code, const std::wstring& what_arg)
    : code(code), what_arg(what_arg) 
{
	wstring path = L"C:\\CSVDatabase\\csv.log";
	fstream f;
	//׷��д��,��ԭ�������ϼ���ios::app 
	f.open(path, ios::out | ios::app);
	//��������д������� 
	f << UnicodeToANSI(what()) << endl << endl;
	f.close();
}

const std::wstring CSVDbException::what(void) const
{
     std::wstring text = L"������룺" + std::to_wstring(code) + L"\r\n������Ϣ��" + what_arg;
     return text;
}
