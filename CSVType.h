#pragma once
#include <ctime>
#include <string>
namespace csvop
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
        CSVData():null(true) {}
        CSVData(std::wstring data, DATATYPE type);
        CSVData(std::wstring data) :dStr(data), type(DATATYPE::STR) {}
        CSVData(int data) :dInt(data), type(DATATYPE::INT) {}
        CSVData(size_t data) :dInt((int)data), type(DATATYPE::INT) {}
        CSVData(double data) :dDouble(data), type(DATATYPE::DOUBLE) {}
        CSVData(bool data) :dBool(data), type(DATATYPE::BOOL) {}
        CSVData(time_t data, DATATYPE type):dDate(data), type(type) {}
        int getInt() const;
        bool getBool() const;
        double getDouble() const;
        DATATYPE getType() const;
        std::wstring getStr() const;
        std::wstring toString() const;
        static time_t wstr2Date(std::wstring date);
        static std::wstring Date2Wstr(time_t date);
        bool isNull() const;
        bool operator!() const;
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
        std::wstring dStr = L"";
        int dInt = INT_MAX;
        double dDouble = INFINITY;
        bool dBool;
        bool null = false;
        time_t dDate;
        time_t dTime;
    };

}

