#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class goofus {
public:
    int getRotTime(vector<string> prices, vector<string> orders);
private:
    class vendingMachine {
    public:
        vendingMachine(vector<string> price_data);
        int processOrder(string order);
    private:
        vector<vector<int>> prices;
        vector<int> column_costs;
        int elapsed_minutes;
        int rotation_seconds;
        int current_column;
        int columns;
    };

};

int goofus::getRotTime(vector<string> prices, vector<string> orders)
{
    vendingMachine myvend(prices);
    return 1;
};

int goofus::getBestColumn(int rank)
{

}

goofus::vendingMachine::processOrder(string order)
{
    stringstream ss(order);
    int row,column,time;
    char colon,dash;
    ss >> row >> colon >> column >> dash >> dash >> time;
    for (i=0; i<(time-elapsed_minutes)/5;++i)  {
        rotateToNext();
    }
}


goofus::vendingMachine::vendingMachine(vector<string> price_data)
{
    elapsed_minutes = 0;
    rotation_seconds = 0;
    stringstream ss;

    for (string price_set : price_data) {
        ss << price_set;
        prices.push_back(vector<int>());
        auto ref = prices.back();
        while (ss.good())  {
            int tmp;
            ss >> tmp;
            ref.push_back(tmp);
        }
    }
    columns = prices[0].size();
    for (int i=0; i<columns; ++i)
        column_costs[i] = accumulate(prices[i].begin(),prices[i].end(),0);
    current_column = findBestColumn(1);
}
