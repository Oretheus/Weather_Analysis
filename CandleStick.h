#pragma once

#include <string>

class CandleStick {
private:
    std::string date;
    double open;
    double high;
    double low;
    double close;

public:
    // Constructor
    CandleStick() = default;
    CandleStick(std::string _date, double _open, double _high, double _low, double _close);

    // Getters
    std::string getDate() const { return date; }
    double getOpen() const { return open; }
    double getHigh() const { return high; }
    double getLow() const { return low; }
    double getClose() const { return close; }

    // Setters
    void setDate(const std::string& _date) { date = _date; }
    void setOpen(double _open) { open = _open; }
    void setHigh(double _high) { high = _high; }
    void setLow(double _low) { low = _low; }
    void setClose(double _close) { close = _close; }
};

