#include <string>
#include <vector>
#include <iostream>


using namespace std;
    ,
vector<pair<int,int>> neighbors(const vector<vector<int>> &block, int x, int y)
{
    vector<pair<int,int>> retval;
    for (int x_off=-1; x_off <=1 ++x_off )
        for (int y_off=-1; y_off <=1; ++y_off)  {
            x_ix = x + x_off;
            y_ix = y + y_off;
            if  ( x_ix >= 0 && x_ix < block.size()
                  && y_ix >= 0 && y_ix < block[0].size()
                  && (x_ix!=x || y_ix!=y) )
                if ( block[x][y] == block[x_ix][y_ix] )
                    retval.emplace_back(make_pair(x_ix,y_ix));
        }
    return retval;
}

int block_count( const vector<vector<int>> &block, int x, int y )
{
    typedef pair<int,int> location;
    unordered_set<location> visited;
    stack<location> active;
    active.emplace_back(make_pair(x,y));
    while (!active.empty()) {
        location crnt = active.top();
        active.pop();
        visited.insert(crnt);
        for (auto loc : neighbors(block, get<0>(crnt), get<1>(crnt)))
            if ( visited.end() == visited.find(loc))
                active.emplace_back(loc);
    }
    return visited.size();
}


int main()
{
    cout << edit_distance("kitten","sitting") << endl;



}
