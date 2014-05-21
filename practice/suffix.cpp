#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>

using namespace std;

class SuffixTree;
class SuffixTree {
 public:
    SuffixTree(string &data );
    bool find(const string &target) const;
 private:
    struct Node {
        typedef unordered_map<char, Node*> child_list;
        child_list children;
        void insert(string &data, int offset);
    };
    Node root;
};

SuffixTree::SuffixTree(string * data)
{
    for (int = 0; i<data.size(); ++i)
        root.insert(data, i);
}

void SuffixTree::Node::insert(string &data, int offset)
{
    if (children.end() == children.find(data[i]))
        children.insert(child_list::value_type(data[i],
                                               new SuffixTree(data,i+1)));
    else
        children[data[i]]->insert(data, i+1);
};

bool SuffixTree::find(const string &target) const
{
    const SuffixTree *node = this;
    for (auto c : target) {
        if (node->children.end() == node->children.find(c))
            return false;
        node = node->children.at(c);
    }
    return true;
}


int main()
{
    string a("foos");
    string b("moneymone");
    string c("oup");
    SuffixTree t1(a);
    SuffixTree t2(b);
    cout << t1.find(c) << endl;
    cout << t2.find(c) << endl;

}

