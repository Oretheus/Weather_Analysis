#include "WeatherAnalyzer.h"
#include "MessageHandler.h"
#include <iostream>
#include <string>
#include <vector>

int getUserOption() {
    int userOption = 0;
    std::string line;
    std::cout << "┌───────────────────┐\n";
    std::cout << "│ Choose an option: │\n";
    std::cout << "└───────────────────┘\n";
    std::getline(std::cin, line);
    try {
        userOption = std::stoi(line);
    } catch (const std::exception& e) {
        // Handle invalid input by setting an invalid option number
        userOption = -1;
    }
    return userOption;
}

int main() {
    WeatherAnalyzer analyzer;
    MessageHandler message;
    std::vector<CandleStick> candlestickData;
    int choice = -1;
    int currentFrame = 0;

    message.loadData(analyzer, candlestickData);
    message.calculateData(analyzer, candlestickData);

    while (true) {
        message.displayMenu();
        choice = getUserOption();

        switch (choice) {
            case 1:
                analyzer.plotCandlestickData(candlestickData);
                break;

            case 2:
                message.filterByDateRange(analyzer, candlestickData);
                break;

            case 3:
                message.filterByYear(analyzer, candlestickData);
                break;

            case 4:
                message.filterByTemperatureRange(analyzer, candlestickData);
                break;

            case 5:
                message.predictFutureTemperatures(analyzer);
                break;

            case 6:
                message.nextTimeFrame(analyzer, candlestickData, currentFrame);
                break;

            case 7:
                message.previousTimeFrame(analyzer, candlestickData, currentFrame);
                break;

            case 8:
                std::cout << "┌────────────┐\n";
                std::cout << "│ Exiting... │\n";
                std::cout << "└────────────┘\n";
                return 0;

            default:
                std::cout << "┌───────────────────────────────────────┐\n";
                std::cout << "│ Invalid choice. Please enter a number │\n";
                std::cout << "│ between 1 and 8.                      │\n";
                std::cout << "└───────────────────────────────────────┘\n";
        }
    }
    return 0;
}