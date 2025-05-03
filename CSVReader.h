#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
    public:
        
        CSVReader();
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);
        static std::vector<std::string> tokenize(std::string csvLine, char separator);
        static OrderBookEntry stringsToOBE(std::string priceString,
                                           std::string amountString,
                                           std::string timestamp,
                                           std::string product,
                                           OrderBookType OrderType);

    private:
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};