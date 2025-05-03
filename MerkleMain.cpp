#include "MerkleMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include "Wallet.h"

MerkleMain::MerkleMain()
{
}

void MerkleMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    // Insert 10 BTC into the wallet
    wallet.insertCurrency("BTC", 10);
    
    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkleMain::printMenu() 
{
    // 1 print help
    std::cout << "1: Print help" << std::endl;

    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;

    // 3 make an offer
    std::cout << "3: Make an offer" << std::endl;

    // 4 make a bid
    std::cout << "4: Make a bid" << std::endl;

    // 5 print wallet
    std::cout << "5: Print wallet" << std::endl;

    // 6 continue
    std::cout << "6: Continue" << std::endl;

    std::cout << "==========" << std::endl;
    
    std::cout << "Current time is: " << currentTime << std::endl; 
}

void MerkleMain::invalidInput()
{
    std::cout << "Invalid choice. Choose 1-6!" << std::endl;
}
void MerkleMain::printHelp()
{
    std::cout << "Help - Your aim is to make money. Analyze the market and make bids offers." << std::endl;
}
void MerkleMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "\nProduct: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p,
                                                                  currentTime);
        if (entries.empty())
        {
            std::cout << "No asks found for this product" << std::endl;
            continue;
        }
        
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        
        // Get previous timestamp
        std::string previousTime = orderBook.getPreviousTime(currentTime);
        if (previousTime != "")
        {
            double priceChange = orderBook.getPriceChange(OrderBookType::ask,
                                                          p,
                                                          currentTime,
                                                          previousTime);
            std::cout << "Price change since last timestamp: " << priceChange << std::endl;
        }
        else 
        {
            std::cout << "No previous timestamp available" << std::endl;
        }
        std::cout << "-------------------" << std::endl;
    }
}
void MerkleMain::enterAsk()
{
    std::cout << "Make and offer - enter the amount: product, price, amount (e.g. ETH/BTC,200,0.5)" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    // Tokenize the input
    std::vector<std::string> tokens = CSVReader::tokenize(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkleMain::enterAsk Bad input!"<< input << std::endl;
    }
    else {
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], 
                                                         tokens[2], 
                                                         currentTime, 
                                                         tokens[0], 
                                                         OrderBookType::ask);
            obe.userName = "simuser";
            if (wallet.canFulfullOrder(obe))
            {
                std::cout << "Wallet looks good!" << std::endl;
                orderBook.insertOrder(obe);                                 
            }
            else std::cout << "Wallet has insufficient funds" << std::endl;
        }catch (const std::exception& e)
        {
            std::cout << "MerkleMain::enterAsk Bad input" << std::endl;
        }              
    }
}
void MerkleMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product, price, amount (e.g. ETH/BTC,200,0.5)" << std::endl;

    std::string input;
    std::getline(std::cin, input);

    // Tokenize the input
    std::vector<std::string> tokens = CSVReader::tokenize(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkleMain::enterBid Bad input!"<< input << std::endl;
    }
    else {
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], 
                                                         tokens[2], 
                                                         currentTime, 
                                                         tokens[0], 
                                                         OrderBookType::bid);
            obe.userName = "simuser";
            if (wallet.canFulfullOrder(obe))
            {
                std::cout << "Wallet looks good!" << std::endl;
                orderBook.insertOrder(obe);                                 
            }
            else std::cout << "Wallet has insufficient funds" << std::endl;
        }catch (const std::exception& e)
        {
            std::cout << "MerkleMain::enterBid Bad input" << std::endl;
        }              
    }
}
void MerkleMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkleMain::goToNextTimeFrame()
{
    std::cout << "Going to next time frame" << std::endl;
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& sale : sales)
    {
        std::cout << "Sale price: " << sale.price << ", amount: " << sale.amount << std::endl;
        if (sale.userName == "simuser")
        {
            // Update the wallet
            wallet.processSale(sale);
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}

int MerkleMain::getUserOption()
{
    int userOption = 0; 
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line); 
    try 
    {
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl; // Display the input back to the user
    return userOption;
}

void MerkleMain::processUserOption(int userOption) 
{
    if (userOption <= 0 || userOption >= 7) // Lower bad input
    {
        invalidInput();
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        goToNextTimeFrame();
    }
}