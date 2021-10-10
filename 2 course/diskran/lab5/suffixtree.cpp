//
// Created by sakost on 19.06.2021.
//

#include "suffixtree.h"


TSuffixTree::TSuffixTree(const std::string& str) : m_str(str + TERMINATE_LEAF_SYMBOL) {
    root = new TNode(nullptr, 0, nullptr);
    globalEnd = new std::size_t(-1);
    Build();
}

TSuffixTree::TSuffixTree(std::string &&str) : m_str(std::move(str)){
    m_str.push_back(TERMINATE_LEAF_SYMBOL);
    root = new TNode(nullptr, 0, nullptr);
    globalEnd = new std::size_t(-1);
    Build();
}

void TSuffixTree::Build() {
    activeNode = root;
    for (std::size_t i(0); i < m_str.size(); ++i) {
        AppendSuffixesOfPrefix(i);
    }
}

void TSuffixTree::AppendSuffixesOfPrefix(std::size_t pos) {
    lastNode = nullptr;
    ++(*globalEnd);

    remainder++;
    while (remainder > 0) {
        if (activeEdgeLength == 0) {
            activeEdge = pos;
        }

        auto found = activeNode->children.find(m_str[activeEdge]);
        if (found == activeNode->children.end()) { // add new leaf
            activeNode->children.emplace(m_str[activeEdge], new TNode{root, pos, globalEnd});
            UpdateLastNodeLink(activeNode);
        } else {
            auto next_node = found->second;

            if (activeEdgeLength >= next_node->size()) {
                activeEdge += next_node->size();
                activeEdgeLength -= next_node->size();
                activeNode = next_node;
                continue;  // we need to find another node
            }

            if (m_str[next_node->start + activeEdgeLength] == m_str[pos]) {
                if (activeNode != root) {
                    UpdateLastNodeLink(activeNode);
                }
                activeEdgeLength++;
                break;
            }
            // we need to create new internal node
            auto new_internal_node = new TNode{root, next_node->start, new std::size_t(
                    next_node->start + activeEdgeLength - 1)};
            activeNode->children[m_str[activeEdge]] = new_internal_node;
            next_node->start += activeEdgeLength;

            new_internal_node->children.emplace(m_str[next_node->start], next_node); // old node
            new_internal_node->children.emplace(m_str[pos], new TNode{root, pos, globalEnd}); // new leaf

            UpdateLastNodeLink(new_internal_node);
            lastNode = new_internal_node;
        }

        remainder--;

        if(activeNode == root && activeEdgeLength > 0){
            activeEdge++;
            activeEdgeLength--;
        }
        if(activeNode != root){
            activeNode = activeNode->suffix_link; // go to suffix link
        }
    }
}

std::string TSuffixTree::FindMinimumString(std::size_t n) const {
    const TNode* node = root;
    return FindMinimumStringRecursive(node, n);
}

std::string TSuffixTree::FindMinimumStringRecursive(const TNode* &node, std::size_t n) const {
    if (n <= node->size()) {
        return m_str.substr(node->start, n);
    }
    for (const auto &child: node->children) {
        const TNode* tmp = child.second;
        std::string ans = FindMinimumStringRecursive(tmp, n - node->size());
        if (!ans.empty()) {
            return m_str.substr(node->start, node->size()) + ans;
        }
    }
    return std::string();
}

void TSuffixTree::UpdateLastNodeLink(TNode* &node) {
    if (lastNode != nullptr) {
//        if(lastNode == node){
//            throw std::logic_error("CYCLE LINK");
//        }
        lastNode->suffix_link = node;
        lastNode = nullptr;
    }
}

std::ostream &operator<<(std::ostream& out, const TSuffixTree & obj) {
    obj.root->RecursiveDisplay(out, 0, obj.m_str);
    return out;
}


void TNode::RecursiveDisplay(std::ostream& out, std::size_t depth, const std::string& str) const {
    if(end == nullptr){
        out << "[root]";
    } else{
        out << '(' << str.substr(start, (*end)-start+1) << ')' << ' ' << '[' << start << ", " << *end << ']';
    }

    for(const auto& child: children){
        out << std::endl;
        out << std::string(depth+1, '\t');
        child.second->RecursiveDisplay(out, depth+1, str);
    }
}

TSuffixTree::~TSuffixTree(){
    *globalEnd = -1;
    delete root;
    lastNode = activeNode = nullptr;
    delete globalEnd;
}


