#pragma once

#include "CandleStick.h"
#include <vector>
#include <string>
#include <map>

class WeatherAnalyzer {
private:
    std::vector<std::tuple<std::string, std::string, double>> weatherData;
    std::vector<CandleStick> candlestickData;

public:
    bool loadData(const std::string& filePath);
    std::vector<CandleStick> computeCandlestickData();
    std::vector<CandleStick> computeCandlestickData(const std::string& country);
    std::vector<CandleStick> filterDataByDateRange(const std::vector<CandleStick>& data, const std::string& startDate, const std::string& endDate);
    std::vector<CandleStick> filterDataByYear(const std::vector<CandleStick>& data, const int& year);
    std::vector<CandleStick> filterDataByTemperatureRange(const std::vector<CandleStick>& data, double minTemp, double maxTemp);
    std::vector<CandleStick> predictTemperature(const std::string& country, const std::string& startDate, const std::string& endDate);
    void plotCandlestickData(const std::vector<CandleStick>& candlestickData);
};