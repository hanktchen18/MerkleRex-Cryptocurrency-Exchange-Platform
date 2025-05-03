#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string fileName);
        /** vectors of known products in the dataset */
        std::vector<std::string> getKnownProducts();
        /** return vectors of orders according to the sent filters */
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);
        /** return the earliest time in the orderbook */
        std::string getEarliestTime();

        /** return the next time after the sent time in the orderbook */
        /** if no next timestamp, wraps around to start */
        std::string getNextTime(std::string timestamp);
        
        /** return the previous time before the sent time in the orderbook */
        /** returns empty string if no previous time exists */
        std::string getPreviousTime(std::string timestamp);
        /** calculate price change between two timestamps for a specific product */
        double getPriceChange(OrderBookType type, 
                              std::string product, 
                              std::string currentTime,
                              std::string previousTime);
        void insertOrder(OrderBookEntry& order);
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

    private:
        std::vector<OrderBookEntry> orders;
};