#include "others.h"
#include <fstream>
void replace_all(std::wstring& str, const std::wstring& old_value, const std::wstring& new_value)
{
    for (std::wstring::size_type pos(0); pos != std::wstring::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != std::wstring::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
    }
}