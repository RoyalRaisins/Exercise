#ifndef B_PLUS
#define B_PLUS
// M=3, L=2
#include <iostream>
#include <vector>
using namespace std;
class L_NODE{
private:
    int key;
    std::string value;
    L_NODE *next = NULL;
    L_NODE *last = NULL;
public:
    L_NODE(int akey, std::string val) : key(akey),value(val){};
    std::string GetValue();
    int GetKey();
    L_NODE *GetNextL_NODE();
    L_NODE *GetLastL_NODE();
    void SetNextL_NODE(L_NODE *node);
    void SetLastL_NODE(L_NODE *node);
};


class LinkList{
private:
    L_NODE root;
public:
    int NextValue();
    int LastValue();
    int Search(int value);
};


class Node{
private:
    Node *parent=NULL;
    std::vector<Node*> branches;
    std::vector<int> keys;
    int M=3;
    int L=2;
    L_NODE* ThisNode;
    std::vector<std::pair<int, L_NODE*>> KeyValuePairs;
  public:
    Node* GetParent();
    Node(int m, int l) : M(m), L(l){};
    void SetParent(Node *newparent);
    void DeleteBranch(Node* branch);
    std::vector<std::pair<int, L_NODE*>> GetKVPs();
    std::vector<std::pair<int, L_NODE*>>& GetcKVPs();
    std::vector<Node*> GetBranches() const;
    std::vector<Node *>& GetcBranches();
    std::vector<int> GetKeys() const;
    std::vector<int>& GetcKeys();
    bool AddChildren(int key);
    bool AddBranch();
    bool branchcheck();
    bool keycheck();
};



class BPTree{
private:
    Node* root;
    int M = 3;
    int L = 2;
public:
    BPTree(int m, int l) : M(m), L(l), root(new Node(m, l)){};
    string GetValue(int key);
    Node *GetLeaf(int key);
    void Insert(int key, std::string value);
    void Delete(int key);
    void SetRoot(Node* root);
};

#endif 