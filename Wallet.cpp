#include <iostream>
#include "CSVReader.h"
#include "Wallet.h"

Wallet::Wallet()
{
};
 
void Wallet::insertCurrency(std::string type, double amount)
{
    double balance;
    // Check if the input amount is valid (positive)
    if (amount < 0)
    {
        throw std::exception{};
    }
    // No existing amount
    if (currencies.count(type) == 0) balance = 0;
    // Already exists
    else balance = currencies[type];

    currencies[type] = balance + amount;
    std::cout << "Inserting " << type << ": " << amount << std::endl;
};

bool Wallet::removeCurrency(std::string type, double amount)
{
    double balance;
    if (amount < 0) // Check if the input amount is valid (positive)
    {
        throw std::exception{};
    }
    if (currencies.count(type) == 0) // if the currency has no value in wallet
    {
        std::cout << "No currency for " << type << std::endl;
        return false;
    }
    else
    {
        if (containsCurrency(type, amount)) // if the wallet contains enough value
        {
            std::cout << "Removing " << type << ": " << amount << std::endl;
            currencies[type] -= amount;
            if (currencies[type] == 0) // remove the key if wallet is empty
            {
                currencies.erase(type);
            }
            return true;
        }
        else return false;
    }
};

bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0) return false;
    else return currencies[type] >= amount;
};

std::string Wallet::toString()
{
    std::cout << "You have:" << std::endl;
    std::string s = "";
    for (std::pair<std::string,double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    if (s.empty())
    {
        s = "nothing in your wallet!\n";
    }
    return s;
};

bool Wallet::canFulfullOrder(const OrderBookEntry& order)
{   
    std::vector<std::string> currs = CSVReader::tokenize(order.product, '/');
    // ask
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfullOrder" << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    // bid
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfullOrder" << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }
    // if something went wrong, return false
    return false;
}

void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenize(sale.product, '/');
    // ask
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    // bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}