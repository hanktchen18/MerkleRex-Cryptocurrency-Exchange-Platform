#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
class MerkleMain
{
    public:
        MerkleMain();
        /** Call this to start the sim */
        void init();
    
    private:
        void printMenu(); 
        void invalidInput();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void goToNextTimeFrame();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;

        OrderBook orderBook{"20200317.csv"};

        Wallet wallet;
};