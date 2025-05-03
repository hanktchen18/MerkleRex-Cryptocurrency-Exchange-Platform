// input/output stream library
#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "MerkleMain.h"
#include "CSVReader.h"
#include "Wallet.h"

int main()
{ 
    MerkleMain app{};
    app.init();
    CSVReader::readCSV("20200317.csv");
    return 0;
}