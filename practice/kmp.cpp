#include <string>
#include <vector>
#include <iostream>


using namespace std;


vector<int> build_table(string target)
{
    vector<int> retval;
    retval.push_back(-1);
    retval.push_back(0);
    int base = 2;
    int cand = 0;

    while ( base < target.size() )  {
        if (target[base-1]==target[cand]) {
            retval[base] = 1 + retval[base-1];
            ++base;
            ++cand;
        } else if (cand>0) {
            cand = retval[cand];
        } else {
            retval[base] = 0;
            ++base;
        }
    }
}


int kmp(string corpus, string target)
{
    int n=corpus.size();
    int k=target.size();

    vector<int> jump_table = build_table(target);

    int start = 0;
    int length = 0;
    
    while (start+length < n)  {
        if (target[length]==corpus[start+length]) {
            ++length;
            if (length >= k)
                return start;
        } else {
            if (jump_table[length]==-1)  {
                start += 1;
                length = 0;
            } else {
                start += length - jump_table[length];
                length = jump_table[length];
            }
        }
    }
    return n;
}
