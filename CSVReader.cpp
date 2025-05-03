#include "CSVReader.h"
#include <iostream>
#include <fstream>
CSVReader::CSVReader()
{

}
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFileName)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFileName};
    std::string line;

    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            try
            {   
                OrderBookEntry obe = stringsToOBE(tokenize(line, ','));
                entries.push_back(obe);       
            }catch(std::exception& e)
            {
                std::cout << "CSVReader::readCSV bad data " << std::endl;
            }
        } // end of while
    }
    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenize(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do{
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    }while(end != std::string::npos);
    
    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;
    if (tokens.size() != 5) // Bad
            {
                std::cout << "Bad line" << std::endl;
                throw std::exception{};
            }
            // We have 5 tokens each line now
            // Use a try catch block to handle exceptions
            try {
                // Convert price & amount from string to double
                price = std::stod(tokens[3]);
                amount = std::stod(tokens[4]);
            }catch(const std::exception& e){
                // We don't know which one is wrong, so print both
                std::cout << "CSVReader::stringsToOBE Bad Float! " << tokens[3] << std::endl;
                std::cout << "CSVReader::stringsToOBE Bad Float! " << tokens[4] << std::endl;
                // Function that called this function will crashed out (The outer function will know)
                throw;
            }
    OrderBookEntry obe{price,
                        amount,
                        tokens[0],
                        tokens[1],
                        OrderBookEntry::stringToOrderBookType(tokens[2])};
    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
                                             std::string amountString,
                                             std::string timestamp,
                                             std::string product,
                                             OrderBookType OrderType)
{
    double price, amount;
     try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad Float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad Float! " << amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{price,
                        amount,
                        timestamp,
                        product,
                        OrderType};
    return obe;
}