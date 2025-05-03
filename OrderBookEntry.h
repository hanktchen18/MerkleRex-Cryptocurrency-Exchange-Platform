#pragma once

#include <string>

enum class OrderBookType{ask, bid, unknown, asksale, bidsale};
class OrderBookEntry
{
    public:
        // Constructor
        // Parameters that are sending in
        OrderBookEntry( double _price,
                        double _amount,
                        std::string _timestamp,
                        std::string _product,
                        OrderBookType _orderType,
                        std::string userName = "dataset");
        static OrderBookType stringToOrderBookType(std::string s);
        static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2);
        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2);
        static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2);        

        // Define member types
        double price;
        double amount;
        std::string timestamp; 
        std::string product;
        OrderBookType orderType;
        std::string userName;
};