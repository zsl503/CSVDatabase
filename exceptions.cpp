#include "exceptions.h"

const std::wstring CSVDBException::what(void) const
{
     std::wstring text = L"������룺" + std::to_wstring(code) + L"\r\n������Ϣ��" + what_arg;
     return text;
}
