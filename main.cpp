#include <vector>
#include "global.h"
#include "CSVDatabase.h"
using namespace CSVDatabase::Table;
using namespace CSVOperate;
using namespace CSVDatabase;
using namespace std;
void main()
{
    //this->foreignKey[0] = foreignKey.substr(0, point);
    //this->foreignKey[1] = foreignKey.substr(point + 1, foreignKey.size() - point);
       //wcout.imbue(locale("chs"));			//更改区域设置 只为控制台输出显示
    setlocale(LC_CTYPE, "");
    //Database::createDatabase(L"E:\\Documents\\CSVDatabase\\");
    try {
    Database db(L"E:\\Documents\\CSVDatabase\\");
    Field f1(L"test", DATATYPE::STR, true);
    Field f2(L"test2", DATATYPE::INT, false,L"testTb1.test");
    Field f3(L"test3", DATATYPE::BOOL, false);
    db.createTable(L"testTb4",{f1,f2,f3});
    //wstring s = L"test.py";
        //wcout << s.substr(0,s.find(L".")) << "  " << s.substr(s.find(L".")+1);
    }
    catch (CSVDBException e) {
        wcout << e.what();
    }
}