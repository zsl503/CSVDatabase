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
	//追加写入,在原来基础上加了ios::app 
	f.open(path, ios::out | ios::app);
	//输入你想写入的内容 
	f << UnicodeToANSI(what()) << endl << endl;
	f.close();
}

const std::wstring CSVDbException::what(void) const
{
     std::wstring text = L"错误代码：" + std::to_wstring(code) + L"\r\n错误信息：" + what_arg;
     return text;
}
