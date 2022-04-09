#include "exceptions.h"

const std::wstring CSVDBException::what(void) const
{
     std::wstring text = L"´íÎó´úÂë£º" + std::to_wstring(code) + L"\r\n´íÎóĞÅÏ¢£º" + what_arg;
     return text;
}
