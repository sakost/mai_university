//
// Created by sakost on 19.06.2021.
//

#ifndef LAB5_SUFFIXTREE_H
#define LAB5_SUFFIXTREE_H

#include <string>
#include <map>
#include <iostream>


const char TERMINATE_LEAF_SYMBOL = '$';

class TSuffixTree;
class TNode;

class TNode{
public:
    friend class TSuffixTree;
    friend std::ostream& operator<<(std::ostream&, const TSuffixTree&);

    TNode(TNode* link, std::size_t start, std::size_t* end) : suffix_link(link),
                                                                              start(start),
                                                                              end(end)
    {}

    std::size_t size() const {
        if(end == nullptr){
            return 0;
        }
        return (*end) - start + 1;
    }

    ~TNode(){
        if(end != nullptr && *end != -1){
            delete end;
        }
        suffix_link = nullptr;
        for(auto &child: children){
            delete child.second;
        }
    }

private:
    void RecursiveDisplay(std::ostream& out, std::size_t depth, const std::string& str) const;
    std::map<char, TNode*> children;
    TNode* suffix_link = nullptr;
    std::size_t start;
    std::size_t* end = nullptr;
};

class TSuffixTree {
public:
    friend std::ostream& operator<<(std::ostream&, const TSuffixTree&);

    explicit TSuffixTree(const std::string&  str);
    explicit TSuffixTree(std::string&& str);
    void Build();
    std::string FindMinimumString(std::size_t n) const;

    ~TSuffixTree();

private:
    std::string FindMinimumStringRecursive(const TNode* &node, std::size_t n) const;

    void AppendSuffixesOfPrefix(std::size_t pos);

    inline void UpdateLastNodeLink(TNode*& node);



    TNode* root = nullptr;
    TNode* lastNode = nullptr;

    std::string m_str;

    TNode* activeNode = nullptr;

    std::size_t* globalEnd = nullptr;

    std::size_t activeEdge = 0;
    std::size_t activeEdgeLength = 0;
    std::size_t remainder = 0;

};



#endif //LAB5_SUFFIXTREE_H
