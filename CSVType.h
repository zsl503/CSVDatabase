#pragma once
#include <ctime>
#include <string>
#include "global.h"
namespace CSVOperate
{
    enum class DATATYPE {
        STR, INT, DOUBLE, DATE, TIME, BOOL
    };
    std::wstring dataTypeToStr(DATATYPE type);

    /**
     * @brief 文本形式的DataType转为DataType
     * @param dataString DataType文本
     * @return DataType
    */
    DATATYPE stringToDataType(std::wstring dataString);

    class CSVData
    {
    public:
        CSVData(std::wstring data,DATATYPE type = DATATYPE::STR);
        int getInt();
        bool getBool();
        double getDouble();
        DATATYPE getType();
        std::wstring getStr();
        std::wstring toStrng();
        bool operator==(const CSVData& ps) const;
    private:
        DATATYPE type;
        std::wstring dStr;
        int dInt;
        double dDouble;
        bool dBool;
        tm dDate;
        tm dTime;
    };

}

