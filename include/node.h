#include "tile.h"
class Node
{
    // first child
    Node *fChild = nullptr;

    // next sibling
    Node *nSibling = nullptr;

    // previous sibling
    Node *pSibling = nullptr;

    // A reference to a collective integer representing the number of siblings
    std::shared_ptr<int> siblings = std::make_shared<int>(int(1));

public:
    Tile *tile = nullptr;
    bool visited = false;

    ~Node()
    {
        (*siblings)--;
        if (nSibling)
            nSibling->pSibling = pSibling;
        if (pSibling)
            pSibling->nSibling = nSibling;
    }

    inline int getSiblings()
    {
        return *siblings;
    }

    inline Node *firstChild()
    {
        return fChild;
    }

    inline Node *nextSibling()
    {
        return nSibling;
    }

    inline Node *prevSibling()
    {
        return pSibling;
    }

    
    void addChild(Node *);
    void addSibling(Node *);
    Node *randomChild();
};