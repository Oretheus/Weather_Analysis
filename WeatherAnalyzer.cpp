#include "WeatherAnalyzer.h"
#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <iomanip>
#include <ctime>

// Load weather data from CSV file
bool WeatherAnalyzer::loadData(const std::string& filePath) {
    // Create an instance of CSVReader to read the CSV file
    CSVReader reader(filePath);
    // Get the data from the CSV file
    auto data = reader.getData();

    // Check if the data is empty
    if (data.empty()) return false;

    // Assume the first row contains headers (country codes) and skip it
    const auto& headers = data[0];
    // Loop through each row of data starting from the second row
    for (size_t rowIdx = 1; rowIdx < data.size(); ++rowIdx) {
        const auto& row = data[rowIdx];
        std::string date = row[0];

        // Read the temperatures for each country in the row
        for (size_t colIdx = 1; colIdx < row.size(); ++colIdx) {
            if (!row[colIdx].empty()) {
                try {
                    // Convert the string to a double representing temperature
                    double temperature = std::stod(row[colIdx]);
                    // Get the country code from the headers
                    std::string country = headers[colIdx];
                    // Store the date, country, and temperature as a tuple in weatherData
                    weatherData.emplace_back(date, country, temperature);
                } catch (const std::invalid_argument&) {
                    // Ignore invalid data entries
                } catch (const std::out_of_range&) {
                    // Ignore out of range data entries
                }
            }
        }
    }

    return true;
}

// Compute candlestick data for all countries
std::vector<CandleStick> WeatherAnalyzer::computeCandlestickData() {
    // Map to store temperature data grouped by date
    std::map<std::string, std::vector<double>> dataByDate;

    // Organize temperature data by date
    for (const auto& entry : weatherData) {
        const auto& [date, country, temp] = entry;
        dataByDate[date].push_back(temp);
    }

    double previousClose = 0.0; // Initialize previous close temperature

    // Compute candlestick values for each date
    for (const auto& [date, temps] : dataByDate) {
        double high = *std::max_element(temps.begin(), temps.end());
        double low = *std::min_element(temps.begin(), temps.end());
        double open = (previousClose == 0.0) ? temps.front() : previousClose; // Use first temperature if no previous close
        double close = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();

        // Create a candlestick object and set its values
        CandleStick candlestick;
        candlestick.setDate(date);
        candlestick.setOpen(open);
        candlestick.setHigh(high);
        candlestick.setLow(low);
        candlestick.setClose(close);

        // Store the candlestick in candlestickData vector
        candlestickData.push_back(candlestick);
        previousClose = close; // Update previous close
    }

    return candlestickData;
}

// Compute candlestick data for a specific country
std::vector<CandleStick> WeatherAnalyzer::computeCandlestickData(const std::string& country) {
    // Map to store temperature data grouped by date for the specific country
    std::map<std::string, std::vector<double>> dataByDate;
    std::vector<CandleStick> countryCandlestickData;

    // Filter data for the specific country and organize by date
    for (const auto& entry : weatherData) {
        auto& [date, entryCountry, temp] = entry;
        // Check if the country matches the given country code (first two characters)
        if (entryCountry.substr(0,2) == country) {
            dataByDate[date].push_back(temp);
        }
    }

    double previousClose = 0.0; // Initialize previous close temperature

    // Compute candlestick values for each date
    for (const auto& [date, temps] : dataByDate) {
        double high = *std::max_element(temps.begin(), temps.end());
        double low = *std::min_element(temps.begin(), temps.end());
        double open = (previousClose == 0.0) ? temps.front() : previousClose; // Use first temperature if no previous close
        double close = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();

        // Create a candlestick object and set its values
        CandleStick candlestick;
        candlestick.setDate(date);
        candlestick.setOpen(open);
        candlestick.setHigh(high);
        candlestick.setLow(low);
        candlestick.setClose(close);

        // Store the candlestick in countryCandlestickData vector
        countryCandlestickData.push_back(candlestick);
        previousClose = close; // Update previous close
    }

    return countryCandlestickData;
}

// Filter candlestick data by date range
std::vector<CandleStick> WeatherAnalyzer::filterDataByDateRange(const std::vector<CandleStick>& data, const std::string& startDate, const std::string& endDate) {
    std::vector<CandleStick> filteredData;
    // Loop through each candlestick and filter by date range
    for (const auto& candle : data) {
        if (candle.getDate() >= startDate && candle.getDate() <= endDate) {
            filteredData.push_back(candle);
        }
    }
    return filteredData;
}

// Filter candlestick data by year
std::vector<CandleStick> WeatherAnalyzer::filterDataByYear(const std::vector<CandleStick>& data, const int& year) {
    std::vector<CandleStick> filteredData;
    // Loop through each candlestick and filter by year
    for (const auto& candle : data) {
        std::string date = candle.getDate();
        date = date.substr(0, 4); // Extract the year from the date

        int entryYear;
        try {
            entryYear = std::stoi(date);
        } catch (const std::exception&) {
            // Ignore invalid date entries
            continue;
        }

        if (entryYear == year) {
            filteredData.push_back(candle);
        }
    }
    return filteredData;
}

// Filter candlestick data by temperature range
std::vector<CandleStick> WeatherAnalyzer::filterDataByTemperatureRange(const std::vector<CandleStick>& data, double minTemp, double maxTemp) {
    std::vector<CandleStick> filteredData;
    // Loop through each candlestick and filter by temperature range
    for (const auto& candle : data) {
        if (candle.getLow() >= minTemp && candle.getHigh() <= maxTemp) {
            filteredData.push_back(candle);
        }
    }
    return filteredData;
}

// Predict temperature using a simple moving average
std::vector<CandleStick> WeatherAnalyzer::predictTemperature(const std::string& country, const std::string& startDate, const std::string& endDate) {
    // Use historical data for the specified country
    auto historicalData = computeCandlestickData(country);
    if (historicalData.empty()) {
        return {};
    }

    // Parse the start and end dates
    std::tm tm_start = {};
    std::tm tm_end = {};
    std::istringstream ss_start(startDate);
    std::istringstream ss_end(endDate);
    ss_start >> std::get_time(&tm_start, "%Y-%m-%d");
    ss_end >> std::get_time(&tm_end, "%Y-%m-%d");

    std::time_t start_time = std::mktime(&tm_start);
    std::time_t end_time = std::mktime(&tm_end);
    if (start_time == -1 || end_time == -1) {
        throw std::invalid_argument("Invalid date format");
    }

    std::vector<CandleStick> predictions;
    std::tm current_tm = tm_start;

    size_t historicalSize = historicalData.size();
    size_t windowSize = 3; // Window size for moving average

    // Predict temperatures using a simple moving average
    while (std::mktime(&current_tm) <= end_time) {
        double sumOpen = 0, sumClose = 0, sumLow = 0, sumHigh = 0;
        size_t count = 0;

        // Calculate the moving average for the current date
        for (size_t j = historicalSize - windowSize; j < historicalSize; ++j) {
            sumOpen += historicalData[j].getOpen();
            sumClose += historicalData[j].getClose();
            sumLow += historicalData[j].getLow();
            sumHigh += historicalData[j].getHigh();
            count++;
        }

        if (count == 0) count = 1; // Prevent division by zero

        // Create a candlestick object and set its predicted values
        CandleStick prediction;
        prediction.setOpen(sumOpen / count);
        prediction.setClose(sumClose / count);
        prediction.setLow(sumLow / count);
        prediction.setHigh(sumHigh / count);

        // Convert current_tm to string date format and set the date
        std::ostringstream oss;
        oss << std::put_time(&current_tm, "%Y-%m-%d");
        prediction.setDate(oss.str());

        predictions.push_back(prediction);
        current_tm.tm_mday++; // Move to the next day
        std::mktime(&current_tm); // Normalize the date structure
    }

    return predictions;
}

// Plot the candlestick data
void WeatherAnalyzer::plotCandlestickData(const std::vector<CandleStick>& candlestickData) {
    double maxTemp = -std::numeric_limits<double>::infinity();
    double minTemp = std::numeric_limits<double>::infinity();

    // Determine the temperature range for scaling the plot
    for (const auto& candle : candlestickData) {
        if (candle.getHigh() > maxTemp) {
            maxTemp = candle.getHigh();
        }
        if (candle.getLow() < minTemp) {
            minTemp = candle.getLow();
        }
    }

    double plotHeight = 20.0;
    double tempRange = maxTemp - minTemp;
    double scaleFactor = plotHeight / tempRange;

    // Print the candlestick plot line by line
    for (const auto& candle : candlestickData) {
        double scaledOpen = (candle.getOpen() - minTemp) * scaleFactor;
        double scaledClose = (candle.getClose() - minTemp) * scaleFactor;
        double scaledHigh = (candle.getHigh() - minTemp) * scaleFactor;
        double scaledLow = (candle.getLow() - minTemp) * scaleFactor;

        for (int i = plotHeight; i >= 0; --i) {
            if (i == static_cast<int>(scaledHigh)) {
                std::cout << "|";
            } else if (i == static_cast<int>(scaledLow)) {
                std::cout << "|";
            } else if (i <= static_cast<int>(scaledOpen) && i >= static_cast<int>(scaledClose)) {
                std::cout << "|";
            } else {
                std::cout << " ";
            }
        }
        std::cout << " " << candle.getDate() << std::endl;
    }
}
