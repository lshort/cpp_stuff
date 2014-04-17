#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Item {
    int _v;
    unsigned int _w;
};


class Knapsack {
public:
    Knapsack(unsigned int capacity) : _capacity(capacity) {};
    int packZeroOne(const vector<Item> &items);
    vector<Item> packMany(const vector<Item> &items);
private:
    unsigned int _capacity;
};


int Knapsack::packZeroOne(const vector<Item> &items)
{
    vector<vector<int>> mx;
    mx.push_back(vector<int>(_capacity+1,0));  // set mx[0,k]=0 for all k
    for (int i=1; i<=items.size(); ++i)  // 1-indexed since 0th entry is empty slot
    {
        mx.push_back(vector<int>(_capacity+1,0));
        for (int j=0; j<_capacity+1; ++j)
        {
            if (items[i-1]._w > j) {
                mx[i][j] = mx[i-1][j];
            } else {
                mx[i][j] = max(mx[i-1][j],items[i-1]._v+mx[i-1][j-items[i-1]._w]);
            }
        }
    }
    return mx[items.size()][_capacity];
}


int main()
{
    vector<Item> a { {1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7}
    };
    vector<Item> b { {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1}
    };
    vector<Item> c { {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7}
    };
    Knapsack backpack(10);
    cout << backpack.packZeroOne(a) << endl;
    cout << backpack.packZeroOne(b) << endl;
    cout << backpack.packZeroOne(c) << endl;
}
