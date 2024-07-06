#pragma once

#include "WeatherAnalyzer.h"
#include <iostream>
#include <string>
#include <vector>

class MessageHandler {
public:
    MessageHandler();

    static void displayMenu();
    static void selectCountryMessage();
    static void showLoadingMessage();
    static void loadData(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData);
    static void calculateData(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData);
    static void filterByDateRange(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData);
    static void filterByYear(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData);
    static void filterByTemperatureRange(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData);
    static void predictFutureTemperatures(WeatherAnalyzer& analyzer);
    static void timeFrameMessage(int& currentFrame, std::vector<CandleStick>& candlestickData); 
    static void nextTimeFrame(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData, int& currentFrame);
    static void previousTimeFrame(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData, int& currentFrame);
};

