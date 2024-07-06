#include "MessageHandler.h"
#include <limits>
#include <iostream>

MessageHandler::MessageHandler() {}

void MessageHandler::displayMenu() {
    std::cout << "┌────────────────────────────────┐\n";
    std::cout << "│      Weather Analysis Tool     │\n";
    std::cout << "├────────────────────────────────┤\n";
    std::cout << "│ 1. Display Candlestick Data    │\n";
    std::cout << "│ 2. Filter by Date Range        │\n";
    std::cout << "│ 3. Filter by Year              │\n";
    std::cout << "│ 4. Filter by Temperature Range │\n";
    std::cout << "│ 5. Predict Future Temperatures │\n";
    std::cout << "│ 6. Next Time Frame             │\n";
    std::cout << "│ 7. Previous Time Frame         │\n";
    std::cout << "│ 8. Exit                        │\n";
    std::cout << "└────────────────────────────────┘\n";
}
 

void MessageHandler::showLoadingMessage() {
    std::cout << "┌──────────────────────────────┐\n";
    std::cout << "│          Loading...          │\n";
    std::cout << "└──────────────────────────────┘\n";
}

void MessageHandler::loadData(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData) {
    std::string filePath;
    while (true) {
        std::cout << "┌──────────────────────────┐\n";
        std::cout << "│ Enter the CSV file path: │\n";
        std::cout << "└──────────────────────────┘\n";
        std::getline(std::cin, filePath);
        showLoadingMessage();
        if (analyzer.loadData(filePath)) break;
        else {
            std::cout << "┌─────────────────────────────┐\n";
            std::cout << "│ Error: Could not find file! │\n";
            std::cout << "└─────────────────────────────┘\n";
        }
    }

    std::cout << "┌───────────────────────────┐\n";
    std::cout << "│ Data loaded successfully. │\n";
    std::cout << "└───────────────────────────┘\n";
}

void MessageHandler::calculateData(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData) {
    std::cout << "┌──────────────────────────┐\n";
    std::cout << "│ Calculating Data...      │\n";
    std::cout << "└──────────────────────────┘\n";
    candlestickData = analyzer.computeCandlestickData();

    if (candlestickData.empty()) {
        std::cout << "┌───────────────────────────────┐\n";
        std::cout << "│ Oops... Something went wrong. │\n";
        std::cout << "└───────────────────────────────┘\n";
        return;
    }

    std::cout << "┌───────────────────────────────┐\n";
    std::cout << "│ Data calculated successfully. │\n";
    std::cout << "└───────────────────────────────┘\n";

}

void MessageHandler::filterByDateRange(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData) {
    std::string startDate, endDate;
    std::cout << "┌────────────────────────────────┐\n";
    std::cout << "│ Enter the start date (YYYY-MM- │\n";
    std::cout << "│ DD):                           │\n";
    std::cout << "└────────────────────────────────┘\n";
    std::getline(std::cin, startDate);
    std::cout << "┌─────────────────────────────────┐\n";
    std::cout << "│ Enter the end date (YYYY-MM-DD) │\n";
    std::cout << "└─────────────────────────────────┘\n";
    std::getline(std::cin, endDate);
    showLoadingMessage();
    auto filteredData = analyzer.filterDataByDateRange(candlestickData, startDate, endDate);
    if (filteredData.empty()) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Error: No data available for │\n";
        std::cout << "│ the specified date range.    │\n";
        std::cout << "└──────────────────────────────┘\n";
        return;
    }
    analyzer.plotCandlestickData(filteredData);
}

void MessageHandler::filterByYear(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData) {
    int year;
    std::string yearStr;
    std::cout << "┌──────────────────────┐\n";
    std::cout << "│ Enter a year (YYYY): │\n";
    std::cout << "└──────────────────────┘\n";
    std::getline(std::cin, yearStr);
    try {
        year = std::stoi(yearStr);
    } catch (const std::exception& e) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Error: Invalid input. Please │\n";
        std::cout << "│ enter a valid year.          │\n";
        std::cout << "└──────────────────────────────┘\n";
        return;
    }
    showLoadingMessage();
    auto filteredData = analyzer.filterDataByYear(candlestickData, year);
    if (filteredData.empty()) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Error: No data available for │\n";
        std::cout << "│ the specified year.          │\n";
        std::cout << "└──────────────────────────────┘\n";
        return;
    }
    analyzer.plotCandlestickData(filteredData);
}

void MessageHandler::filterByTemperatureRange(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData) {
    double minTemp, maxTemp;
    std::string minTempStr, maxTempStr;
    std::cout << "┌───────────────────────────────┐\n";
    std::cout << "│ Enter the minimum temperature │\n";
    std::cout << "└───────────────────────────────┘\n";
    std::getline(std::cin, minTempStr);
    std::cout << "┌───────────────────────────────┐\n";
    std::cout << "│ Enter the maximum temperature │\n";
    std::cout << "└───────────────────────────────┘\n";
    std::getline(std::cin, maxTempStr);
    try {
        minTemp = std::stod(minTempStr);
        maxTemp = std::stod(maxTempStr);
    } catch (const std::exception& e) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Error: Invalid input. Please │\n";
        std::cout << "│ enter valid temperatures.    │\n";
        std::cout << "└──────────────────────────────┘\n";
        return;
    }
    showLoadingMessage();
    auto filteredData = analyzer.filterDataByTemperatureRange(candlestickData, minTemp, maxTemp);
    
    if (filteredData.empty()) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Error: No data available for │\n";
        std::cout << "│ the specified temperature    │\n";
        std::cout << "│ range.                       │\n";
        std::cout << "└──────────────────────────────┘\n";
        return;
    }

    analyzer.plotCandlestickData(filteredData);
}

void MessageHandler::predictFutureTemperatures(WeatherAnalyzer& analyzer) {
    std::string startDate, endDate, country;
    std::cout << "┌────────────────────────────────┐\n";
    std::cout << "│ Enter the desired country to   │\n";
    std::cout << "│ predict:                       │\n";
    std::cout << "└────────────────────────────────┘\n";
    std::getline(std::cin, country);
    std::cout << "┌────────────────────────────────┐\n";
    std::cout << "│ Enter the start date (YYYY-MM- │\n";
    std::cout << "│ DD):                           │\n";
    std::cout << "└────────────────────────────────┘\n";
    std::getline(std::cin, startDate);
    std::cout << "┌─────────────────────────────────┐\n";
    std::cout << "│ Enter the end date (YYYY-MM-DD) │\n";
    std::cout << "└─────────────────────────────────┘\n";
    std::getline(std::cin, endDate);
    std::cout << "┌──────────────────────────────┐\n";
    std::cout << "│          Loading...          │\n";
    std::cout << "└──────────────────────────────┘\n";
    
    auto predictions = analyzer.predictTemperature(country, startDate, endDate);

    if (predictions.empty()) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Error: No data available for │\n";
        std::cout << "│ prediction.                  │\n";
        std::cout << "└──────────────────────────────┘\n";
        return;
    }

    std::cout << "┌──────────────────────────────────────────────┐\n";
    std::cout << "│ Predicted temperatures:                      │\n";
    std::cout << "└──────────────────────────────────────────────┘\n";
    for (const auto& candle : predictions) {
        std::cout << "┌──────────────────────────────┐\n";
        std::cout << "│ Date: " << candle.getDate() << "\n";
        std::cout << "│ Open: " << candle.getOpen() << "\n";
        std::cout << "│ Close: " << candle.getClose() << "\n";
        std::cout << "│ High: " << candle.getHigh() << "\n";
        std::cout << "│ Low: " << candle.getLow() << "\n";
        std::cout << "└──────────────────────────────┘\n";
    }
}

void MessageHandler::timeFrameMessage(int& currentFrame, std::vector<CandleStick>& candlestickData) {
    std::cout << "┌──────────────────────────────┐\n";
    std::cout << "│ Displaying time frame: " << currentFrame + 1 << "\n";
    std::cout << "│ Date: " << candlestickData[currentFrame].getDate() << "\n";
    std::cout << "│ Open: " << candlestickData[currentFrame].getOpen() << "\n";
    std::cout << "│ Close: " << candlestickData[currentFrame].getClose() << "\n";
    std::cout << "│ High: " << candlestickData[currentFrame].getHigh() << "\n";
    std::cout << "│ Low: " << candlestickData[currentFrame].getLow() << "\n";
    std::cout << "└──────────────────────────────┘\n";
}


void MessageHandler::nextTimeFrame(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData, int& currentFrame) {
    if (currentFrame < static_cast<int>(candlestickData.size()) - 1) {
        currentFrame++;
        showLoadingMessage();
        timeFrameMessage(currentFrame, candlestickData);
        std::cout << "┌───────────────────────────────────────────────────────────────────────┐\n";
        std::cout << "  Plot of the single frame: ";
        analyzer.plotCandlestickData({candlestickData[currentFrame]});
        std::cout << "└───────────────────────────────────────────────────────────────────────┘\n";
    } else {
        std::cout << "┌──────────────────────────┐\n";
        std::cout << "│ You are at the last time │\n";
        std::cout << "│ frame.                   │\n";
        std::cout << "└──────────────────────────┘\n";
    }
}

void MessageHandler::previousTimeFrame(WeatherAnalyzer& analyzer, std::vector<CandleStick>& candlestickData, int& currentFrame) {
    if (currentFrame > 0) {
        currentFrame--;
        showLoadingMessage();
        timeFrameMessage(currentFrame, candlestickData);
        std::cout << "┌───────────────────────────────────────────────────────────────────────┐\n";
        std::cout << "  Plot of the single frame: ";
        analyzer.plotCandlestickData({candlestickData[currentFrame]});
        std::cout << "└───────────────────────────────────────────────────────────────────────┘\n";
    } else {
        std::cout << "┌───────────────────────────┐\n";
        std::cout << "│ You are at the first time │\n";
        std::cout << "│ frame.                    │\n";
        std::cout << "└───────────────────────────┘\n";
    }
}

