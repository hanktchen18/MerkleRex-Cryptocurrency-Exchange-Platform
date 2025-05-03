#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <iostream>

OrderBook::OrderBook(std::string fileName)
{
    orders = CSVReader::readCSV(fileName);
};
/** vectors of known products in the dataset */
std::vector<std::string> OrderBook::getKnownProducts()
{

    std::vector<std::string> products;
    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e: orders)
    {
        prodMap[e.product] = true;
    }
    // now flatten the map to a vector of strings
    // Iterate through the prodMap - pullout each element
    for (auto const& e: prodMap)
    {
        // Add key(first) to a vector of strings
        products.push_back(e.first);
    }
    return products;
};
/** return vectors of orders according to the sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type &&
        e.product == product &&
        e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
};

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
};

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
};

std::string OrderBook::getPreviousTime(std::string timestamp)
{  
    std::string prev_timestamp = "";
    std::string earliestTime = getEarliestTime();

    if (timestamp == earliestTime)
    {
        return "";
    }
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp < timestamp && (prev_timestamp == "" || e.timestamp > prev_timestamp))
        {
            prev_timestamp = e.timestamp;
        }
    }
    return prev_timestamp;
}



double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{   
    if (orders.empty()) return 0.0;
    
    double max = orders[0].price;
    for (OrderBookEntry e : orders)
    {
        if (e.price > max) max = e.price;
    }
    return max;
};

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{   
    if (orders.empty()) return 0.0;
    
    double min = orders[0].price;
    for (OrderBookEntry e : orders)
    {
        if (e.price < min) min = e.price;
    }
    return min;
};

double OrderBook::getPriceChange(OrderBookType type, 
                                std::string product, 
                                std::string currentTime,
                                std::string previousTime)
{
    // Get orders for current time
    std::vector<OrderBookEntry> currentOrders = getOrders(type, product, currentTime);
    // Get orders for previous time
    std::vector<OrderBookEntry> previousOrders = getOrders(type, product, previousTime);
    
    // Only calculate price change if both time periods have orders
    if (!currentOrders.empty() && !previousOrders.empty())
    {
        double currentPrice = getHighPrice(currentOrders);
        double previousPrice = getHighPrice(previousOrders);
        return currentPrice - previousPrice;
    }
    
    return 0.0; // Return 0 if either time period has no orders
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    // Sort the whole data
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                  product,
                                                  timestamp);
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                  product,
                                                  timestamp);
    // sales = []
    std::vector<OrderBookEntry> sales;

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    std::cout << "Max bid: " << bids[0].price << std::endl;
    std::cout << "min bid: " << bids[bids.size()-1].price << std::endl;

    // for ask in asks:
    for (OrderBookEntry& ask : asks)
    {
        // for bid in bids:
        for (OrderBookEntry& bid : bids)
        {
            // if bid.price >= ask.price:
            if (bid.price >= ask.price) // transaction start
            {
                // sale = new order()
                // sale.price = ask.price
                OrderBookEntry sale{ask.price,
                    0,
                    timestamp,
                    product,
                    OrderBookType::asksale};
                // Check if it is a transaction made by user
                if (bid.userName == "simuser")
                {
                    sale.userName = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.userName == "simuser")
                {
                    sale.userName = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }
                              
                // Scenario 1: if bid.amount == ask.amount:
                if (bid.amount == ask.amount)
                {
                    // sale.amount = (ask/bid) amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // bid.amount = 0
                    bid.amount = 0;
                    // break
                    break;
                }
                // Scenario 2: if bid.amount > ask.amount:
                if (bid.amount > ask.amount)
                {
                    // sale.amount = ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // bid.amount  = bid.amount - ask.amount
                    bid.amount = bid.amount - ask.amount;
                    // break
                    break;
                }
                // Scenario 3: if ask.amount > bid.amount:
                if (ask.amount > bid.amount && bid.amount > 0)
                {
                    // sale.amount = bid.amount
                    sale.amount = bid.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
                    // bid.amount = 0
                    bid.amount = 0;
                    // continue 
                    continue;
                }
            }
        }
    }
    return sales;
}