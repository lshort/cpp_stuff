#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>

using namespace std;

class SuffixTree;
class SuffixTree {
 public:
    SuffixTree(const string &data );
    bool find(const string &target) const;
 private:
    struct Node {
        typedef unordered_map<char, Node*> child_list;
        child_list children;
        void insert(const string &data, int offset);
    };
    Node root;
};

SuffixTree::SuffixTree(const string &data)
{
    for (int i=0; i<data.size(); ++i)  {
        root.insert(data, i);
    }
}

void SuffixTree::Node::insert(const string &data, int offset)
{
    if (offset+1 > data.size())
        return;
    if (children.end() == children.find(data[offset]))  { 
        children.insert(child_list::value_type(data[offset], new Node ));
        children.at(data[offset])->insert(data, offset+1);
    } else {
        children[data[offset]]->insert(data, offset+1);
    }
};

bool SuffixTree::find(const string &target) const
{
    const Node *node = &root;
    for (auto c : target) {
        if (node->children.end() == node->children.find(c))
            return false;
        node = node->children.at(c);
    }
    return true;
}


int main()
{
    string a("foosball");
    string b("moneymone");
    string c("oup");
    string d("one");
    string e("all");
    string f("foo");
SuffixTree t1(a);
    SuffixTree t2(b);
    cout << t1.find(c) << endl;
    cout << t2.find(c) << endl;
    cout << t1.find(d) << endl;
    cout << t2.find(d) << endl;
    cout << t1.find(e) << endl;
    cout << t2.find(e) << endl;
    cout << t1.find(f) << endl;
    cout << t2.find(f) << endl;

}

