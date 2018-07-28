#include <vector>
#include "B+T.h"

using namespace std;

void error(string e){
    throw runtime_error(e);
}

L_NODE* L_NODE::GetNextL_NODE(){
    return next;
}

L_NODE* L_NODE::GetLastL_NODE(){
    return last;
}

void L_NODE::SetLastL_NODE(L_NODE* Node){
    last = Node;
}

void L_NODE::SetNextL_NODE(L_NODE* Node){
    next = Node;
}

void InsertL_NODE(L_NODE* last,L_NODE* next,L_NODE* newNode){
    if(last!=NULL)  last->SetNextL_NODE(newNode);
    if(next!=NULL)next->SetLastL_NODE(newNode);
    newNode->SetLastL_NODE(last);
    newNode->SetNextL_NODE(next);
}

void RemoveL_NODE(L_NODE* node){
    L_NODE *NewNextL_NODE, *NewLastL_NODE;
    NewNextL_NODE = node->GetNextL_NODE();
    NewLastL_NODE = node->GetLastL_NODE();
    if(NewLastL_NODE!=NULL)NewLastL_NODE->SetNextL_NODE(NewNextL_NODE);
    if(NewNextL_NODE!=NULL)NewNextL_NODE->SetLastL_NODE(NewLastL_NODE);
    NewLastL_NODE = NULL;
    NewNextL_NODE = NULL;
}

string L_NODE::GetValue(){
    return value;
}

int L_NODE::GetKey(){
    return key;
}

Node* Node::GetParent(){
    return parent;
}

void Node::SetParent(Node* newparent){
    parent = newparent;
}
    
std::vector<Node*> Node::GetBranches() const{
    return branches;
}

std::vector<Node *>& Node::GetcBranches(){
    return branches;
}

std::vector<int>& Node::GetcKeys(){
    return keys;
}


void Node::DeleteBranch(Node* branch){
    for (int i = 0; i < branches.size();++i)
    {
        if(branches[i]==branch){
            branches[i]->SetParent(NULL);
            branches.erase(branches.begin()+i);
        }
    }
}

std::vector<int> Node::GetKeys() const{
    return keys;
}
bool AddChildren(int key);
bool AddBranch();
bool branchcheck();
bool keycheck();

int keyPosition(int key,  vector<int> const & keys ){
    int length = keys.size();
    for (int i = 0; i < length;++i){
        if(key<keys[i])
            return i;
    }
    return length;
}

std::vector<pair<int, L_NODE*>> Node::GetKVPs(){
    return KeyValuePairs;
};
std::vector<pair<int, L_NODE*>>& Node::GetcKVPs(){
    return KeyValuePairs;
};
Node* BPTree::GetLeaf(int key){
    Node *reader=root;
    cout << "Branches Number"
         << " " << reader->GetcBranches().size() << endl;
    int NumberOfKeys;
    bool flag;
    while(!reader->GetBranches().empty()){
        flag = false;
        vector<int> keys=reader->GetKeys();
        if(!keys.empty())
            cout << keys[0] << endl;
        NumberOfKeys = keys.size();
        for (int i = 0; i < NumberOfKeys;++i){
            if(key<=keys[i]){
                reader = reader->GetBranches()[i];
                flag = true;
                break;
            }
        }
        if(!flag){
            reader = reader->GetBranches()[NumberOfKeys];
        }
    }
    return reader;
}

string BPTree::GetValue(int key){
    Node *leaf = GetLeaf(key);
    vector<pair<int,L_NODE*>> KeyValues = leaf->GetKVPs();
    cout << "THis"
         << " " << KeyValues.size()<<endl;
    cout << KeyValues[0].first << endl;
    for (int i = 0; i < KeyValues.size();++i){
        if(KeyValues[i].first==key){
            return KeyValues[i].second->GetValue();
        }
    }
    error("Key has no value");
    return "";
}

void BPTree::SetRoot(Node* newroot){
    root = newroot;
}















void BPTree::Insert(int key, string value){
    Node *leaf = GetLeaf(key);
    vector<pair<int, L_NODE*>>& KeyValues = leaf->GetcKVPs();
    vector<int>& Keys = leaf->GetcKeys();
    if(KeyValues.size()==0){
        L_NODE *newLnode = new L_NODE(key, value);
        KeyValues.push_back(make_pair(key, newLnode));
        Keys.push_back(key);
        return;
    }
    int position = KeyValues.size();
    for (int i = 0; i < KeyValues.size();++i){
        if(KeyValues[i].first==key)
            return;
        if(key<KeyValues[i].first){
            position = i;
            break;
        }
    }

    L_NODE *newLnode = new L_NODE(key,value);
    L_NODE * NewNextL_NODE, *NewLastL_NODE;
    if(position<KeyValues.size()){
        NewNextL_NODE = KeyValues[position].second;
    }
    else{
        NewNextL_NODE = KeyValues[position - 1].second->GetNextL_NODE();
    }
    if(NewNextL_NODE==NULL){
        NewLastL_NODE = KeyValues[position - 1].second;
    }
    else{
        NewLastL_NODE = NewNextL_NODE->GetLastL_NODE();
    }
    InsertL_NODE(NewLastL_NODE, NewNextL_NODE, newLnode);
    NewNextL_NODE = NULL;
    NewLastL_NODE = NULL;

    if(KeyValues.size()<=L-1){
        KeyValues.insert(KeyValues.begin()+position, make_pair(key,newLnode));
        Keys.insert(Keys.begin() + position, key);
    }

    else {
        Node *parent;
        Node *Self;
        KeyValues.insert(KeyValues.begin()+position, make_pair(key,newLnode));
        Keys.insert(Keys.begin() + position, key);
        parent = leaf->GetParent();
        bool overflow=true;
        Node* newleaf = new Node(M, L);
        vector<pair<int,L_NODE*>>& NewKeyValues=newleaf->GetcKVPs();
        vector<int>& NewKeys = newleaf->GetcKeys();
        for (int i = (L + 1) / 2; i < L + 1;++i){
            NewKeyValues.push_back(KeyValues[i]);
            NewKeys.push_back(Keys[i]);  //新的叶节点插入元素对以及key
        }
        KeyValues.erase(KeyValues.begin() +(L + 1) / 2, KeyValues.end());
        Keys.erase(Keys.begin() + (L + 1) / 2, Keys.end());//删除旧的叶节点里的元素对和keys
        int Okey = NewKeys[NewKeys.size()-1];
        if(parent==NULL){
            parent = new Node(M, L);
            leaf->SetParent(parent);
            parent->GetcBranches().push_back(leaf);
            parent->GetcKeys().push_back(parent->GetcKeys().size() - 1);
            SetRoot(parent);
        }
        vector<int>& Pkeys=parent->GetcKeys();
        vector<Node *>& Pbranches = parent->GetcBranches();
        int OkeyPosition = keyPosition(Okey, Pkeys);
        Pkeys.insert(Pkeys.begin()+OkeyPosition,Okey);
        Pkeys[OkeyPosition - 1] = Keys[Keys.size() - 1];
        Pbranches.insert(Pbranches.begin() + OkeyPosition , newleaf);
        newleaf->SetParent(parent);
        if (Pkeys.size()<=M){
            overflow = false;
        }
        while (overflow){
            Self= parent;
            parent = parent->GetParent();
            
            if(parent==NULL){
                Node *NewRoot = new Node(M, L);
                SetRoot(NewRoot);
                Node *NewNeighbor = new Node(M, L);
                vector<int>& NeighborKeys = NewNeighbor->GetcKeys();
                vector<int>& SelfKeys = Self->GetcKeys();
                vector<Node *>& NeighborBranches = NewNeighbor->GetcBranches();
                vector<Node *>& SelfBranches = Self->GetcBranches();
                Self->SetParent(NewRoot);
                NewNeighbor->SetParent(NewRoot);
                for (int i = (M+1) / 2; i < M+1 ;++i){
                    NeighborKeys.push_back(SelfKeys[i]);
                    NeighborBranches.push_back(SelfBranches[i]);
                    NeighborBranches[i-(1+M)/2]->SetParent(NewNeighbor);
                }
                SelfBranches.erase(SelfBranches.begin() +(M + 1) / 2, SelfBranches.end());
                SelfKeys.erase(SelfKeys.begin() +(M + 1) / 2), SelfKeys.end();
                
                Okey = NeighborKeys[NeighborKeys.size()-1];
                
                NewRoot->GetcBranches().push_back(Self);
                NewRoot->GetcBranches().push_back(NewNeighbor);
                NewRoot->GetcKeys().push_back(SelfKeys[SelfKeys.size() - 1]);
                NewRoot->GetcKeys().push_back(Okey);
                overflow = false;
            }
            
            else{
                Node *NewNeighbor = new Node(M, L);
                NewNeighbor->SetParent(parent);
                vector<int>& NeighborKeys = NewNeighbor->GetcKeys();
                vector<int>& SelfKeys = Self->GetcKeys();
                vector<Node *>& NeighborBranches = NewNeighbor->GetcBranches();
                vector<Node *>& SelfBranches = Self->GetcBranches();
                for (int i = (1+M) / 2; i < M;++i){
                    NeighborKeys.push_back(SelfKeys[i]);
                    NeighborBranches.push_back(SelfBranches[i]);
                    SelfBranches[i]->SetParent(NewNeighbor);
                }
                NeighborBranches.push_back(SelfBranches[M]);
                SelfBranches[M]->SetParent(NewNeighbor);
                SelfBranches.erase(SelfBranches.begin() +(M + 1) / 2, SelfBranches.end());
                SelfKeys.erase(SelfKeys.begin() +(M + 1) / 2), SelfKeys.end();

                Okey = NeighborKeys[NeighborKeys.size()-1];

                
                int OkeyPosition = keyPosition(Okey, parent->GetcKeys());
                parent->GetcKeys()[OkeyPosition - 1] = SelfKeys[SelfKeys.size() - 1];
                parent->GetcKeys().insert(parent->GetcKeys().begin() + OkeyPosition, Okey);
                parent->GetcBranches().insert(parent->GetcBranches().begin() + OkeyPosition + 1, NewNeighbor);
                if(parent->GetcKeys().size()<=M){
                    overflow = false;
                }
            }
        }
        Self = NULL;
        parent = NULL;
    }
}











void BPTree::Delete(int key){
    int NumberOfKeys;
    Node *Leaf = GetLeaf(key);
    vector<int> &LeafKeys = Leaf->GetcKeys();
    vector<pair<int, L_NODE *>> &LeafKeyValues = Leaf->GetcKVPs();
    NumberOfKeys = LeafKeys.size();
    int DeletePosition=-1;
    for (int i = 0; i < NumberOfKeys;++i){
        if(LeafKeys[i]==key){
            DeletePosition = i;
            break;
        }
    }
    if(DeletePosition==-1)
        error("Invalid key");

    RemoveL_NODE(LeafKeyValues[DeletePosition].second);

    LeafKeys.erase(LeafKeys.begin() + DeletePosition);
    LeafKeyValues.erase(LeafKeyValues.begin() + DeletePosition);

    if (LeafKeys.size() >= L / 2)
    {
        return;
    }
    else{
        Node *Parent = Leaf->GetParent();
        if(Parent==NULL){
            return;
        }
        vector<Node *>& PBranches = Parent->GetcBranches();
        vector<int>& PKeys = Parent->GetcKeys();
        int NOB = PBranches.size();
        int ReaderPosition=-1;
        for (int i = 0; i < NOB;++i){
            if(PBranches[i]==Leaf){
                ReaderPosition=i;
                break;
            }
        }
        if(ReaderPosition==-1){
            error("Parent has no such child");
        }
        bool morecheck = true;

        if(ReaderPosition!=NOB-1){                           //解决第一层是借邻居还是合并,先查右边的兄弟
            vector<int>& RBrotherKeys = PBranches[ReaderPosition + 1]->GetcKeys();
            vector<pair<int, L_NODE *>>& RBrotherKeyValues = PBranches[ReaderPosition + 1]->GetcKVPs();
            int RBNumberOfKeys = RBrotherKeys.size();
            if(RBNumberOfKeys>L/2){
                PKeys[ReaderPosition] = RBrotherKeys[0];
                LeafKeys.push_back(RBrotherKeys[0]);
                LeafKeyValues.push_back(RBrotherKeyValues[0]);
                RBrotherKeys.erase(RBrotherKeys.begin());
                RBrotherKeyValues.erase(RBrotherKeyValues.begin());
                morecheck = false;
            }
        }
        if(morecheck){                                    //再查左边的兄弟
            if(ReaderPosition!=0) {
                vector<int>& LBrotherKeys = PBranches[ReaderPosition - 1]->GetcKeys();
                vector<pair<int, L_NODE *>>& LBrotherKeyValues = PBranches[ReaderPosition - 1]->GetcKVPs();
                int LBNumberOfKeys = LBrotherKeys.size();
                if (ReaderPosition != 0){
                    if(LBNumberOfKeys>L/2){
                    LeafKeys.insert(LeafKeys.begin(), LBrotherKeys[LBNumberOfKeys-1]);
                    LeafKeyValues.insert(LeafKeyValues.begin(), LBrotherKeyValues[LBNumberOfKeys-1]);
                    LBrotherKeys.pop_back();
                    LBrotherKeyValues.pop_back();
                    PKeys[ReaderPosition - 1] = LBrotherKeys[LBrotherKeys.size() - 1];
                    morecheck = false;
                    }
                }
            }
            if(morecheck){                                //还是不行，就合并，优先合并右兄弟
                if(ReaderPosition!=NOB-1){
                    vector<int>& RBrotherKeys = PBranches[ReaderPosition + 1]->GetcKeys();
                    vector<pair<int, L_NODE *>>& RBrotherKeyValues = PBranches[ReaderPosition + 1]->GetcKVPs();
                    int RBNumberOfKeys = RBrotherKeys.size();
                    Node *MergeNode = PBranches[ReaderPosition + 1];
                    for (int i = 0; i < RBNumberOfKeys; ++i)
                    {
                        LeafKeys.push_back(RBrotherKeys[i]);
                        LeafKeyValues.push_back(RBrotherKeyValues[i]);
                    }
                    delete MergeNode;
                    PBranches.erase(PBranches.begin() + ReaderPosition + 1);
                    PKeys.erase(PKeys.begin() + ReaderPosition + 1);
                    PKeys[ReaderPosition] = LeafKeys[LeafKeys.size() - 1];
                }
                else{
                    vector<int>& LBrotherKeys = PBranches[ReaderPosition - 1]->GetcKeys();
                    vector<pair<int, L_NODE *>>& LBrotherKeyValues = PBranches[ReaderPosition - 1]->GetcKVPs();
                    for (int i = 0; i < LeafKeys.size();++i){
                        LBrotherKeys.push_back(LeafKeys[i]);
                        LBrotherKeyValues.push_back(LeafKeyValues[i]);
                    }
                    delete Leaf;
                    PBranches.erase(PBranches.begin() + ReaderPosition-1);
                    PKeys.erase(PKeys.begin() + ReaderPosition - 1);
                    PKeys[ReaderPosition - 1] = LBrotherKeys[LBrotherKeys.size() - 1];
                }
            }
        }
        
        bool underflow = false;
        Node *Self;
        vector<int> &Keys = Parent->GetcKeys();
        vector<Node *> &Branches=Parent->GetcBranches();
        int SelfPosition=-1;
        if(Branches.size()<(1+M)/2){
            underflow = true;
        }
        while(underflow){
            Self = Parent;
            Parent = Self->GetParent();
            Keys = Self->GetcKeys();
            Branches = Self->GetcBranches();
            if(Parent==NULL){
                if(Branches.size()>=2)
                    break;
                else{
                    if(Branches.size()==1){
                        Self = Branches[0];
                        root = Self;
                        underflow = false;
                        break;
                    }
                    else{
                        break;
                    }
                }
            }
            else{
                PBranches = Parent->GetcBranches();
                PKeys = Parent->GetcKeys();
                NOB = PBranches.size();
                for (int i = 0; i < NOB;++i){
                    if(PBranches[i]==Self){
                        SelfPosition = i;
                        break;
                    }
                }
                if(SelfPosition==-1)
                    error("Node Not found");
                    

                if(SelfPosition<NOB-1){
                    vector<Node*>& RBBranches = PBranches[SelfPosition + 1]->GetcBranches();
                    vector<int>& RBKeys = PBranches[SelfPosition + 1]->GetcKeys();
                    if(RBBranches.size()>(M+1)/2){
                        PKeys[SelfPosition] = RBKeys[0];
                        RBBranches[0]->SetParent(Self);
                        Branches.push_back(RBBranches[0]);
                        Keys.push_back(RBKeys[0]);
                        RBBranches.erase(RBBranches.begin());
                        RBKeys.erase(RBKeys.begin());
                        underflow = false;
                        continue;
                    }
                }
                if(SelfPosition!=0){
                    vector<Node *>& LBBranches = PBranches[SelfPosition - 1]->GetcBranches();
                    vector<int>& LBKeys = PBranches[SelfPosition - 1]->GetcKeys();
                    if(LBBranches.size()>(1+M)/2){
                        LBBranches[LBBranches.size()-1]->SetParent(Self);
                        Branches.insert(Branches.begin(), LBBranches[-1]);
                        Keys.insert(Keys.begin(), LBKeys[-1]);
                        LBBranches.pop_back();
                        LBKeys.pop_back();
                        Keys[SelfPosition - 1] = Keys[0];
                        underflow = false;
                        continue;
                    }
                }
                if(SelfPosition!=NOB-1){
                    vector<Node*>& RBBranches = PBranches[SelfPosition + 1]->GetcBranches();
                    vector<int>& RBKeys = PBranches[SelfPosition + 1]->GetcKeys();
                    Node *MergeNode = PBranches[SelfPosition + 1];
                    int RBNumberOfKeys = RBKeys.size();
                    for (int i = 0; i < RBNumberOfKeys;++i){
                        Keys.push_back(RBKeys[i]);
                        Branches.push_back(RBBranches[i]);
                        RBBranches[i]->SetParent(Self);
                    }
                    PBranches.erase(PBranches.begin() + SelfPosition +1);
                    PKeys.erase(PKeys.begin() + SelfPosition+1);
                    PKeys[SelfPosition] = Keys[Keys.size() - 1];
                    delete MergeNode;
                }
                else{
                    vector<Node *>& LBBranches = PBranches[SelfPosition - 1]->GetcBranches();
                    vector<int>& LBKeys = PBranches[SelfPosition - 1]->GetcKeys();
                    Node *MergeNode = PBranches[SelfPosition - 1];
                    int SelfNOK = Keys.size();
                    for (int i = 0; i < SelfNOK;++i){
                        LBBranches.push_back(Branches[i]);
                        Branches[i]->SetParent(MergeNode);
                        LBKeys.push_back(Keys[i]);
                    }
                    PBranches.erase(PBranches.begin() + SelfPosition);
                    PKeys.erase(PKeys.begin() + SelfPosition);
                    PKeys[SelfPosition-1] = LBKeys[LBKeys.size()]
                    delete Self;
                    Self = MergeNode;
                }

            }
            if(PBranches.size()>=(M+1)/2)
                underflow = false;
        }
        Self = NULL;
        Parent = NULL;
    }
}


