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
        CSVData(){}
        CSVData(std::wstring data, DATATYPE type);
        CSVData(std::wstring data) :dStr(data), type(DATATYPE::STR) {}
        CSVData(int data) :dInt(data), type(DATATYPE::INT) {}
        CSVData(double data) :dDouble(data), type(DATATYPE::DOUBLE) {}
        CSVData(bool data):dBool(data), type(DATATYPE::BOOL) {}
        int getInt() const;
        bool getBool() const;
        double getDouble() const;
        DATATYPE getType() const;
        std::wstring getStr() const;
        std::wstring toString() const;
        bool operator==(const CSVData& ps) const;
        bool operator<(const CSVData& ps) const;
        bool operator>(const CSVData& ps) const;
        bool operator<=(const CSVData& ps) const;
        bool operator>=(const CSVData& ps) const;
        bool operator!=(const CSVData& ps) const;
        const CSVData operator+(const CSVData& data) const;
        const CSVData operator-(const CSVData& data) const;
        const CSVData operator*(const CSVData& data) const;
        const CSVData operator/(const CSVData& data) const;
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

