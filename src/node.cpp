#include "node.h"
#include "debug.h"

void Node::addChild(Node *n)
{
    tile->open(n->tile);
    if (fChild)
    {
        fChild->addSibling(n);
        return;
    }

    fChild = n;
    // this is working now
    // it was saying the integer was 0
    // i dont know why
    *(fChild->siblings) = 1;
}

void Node::addSibling(Node *n)
{
    if(n == this)
        throw "wat";
    // do a check to see if the tile already has siblings
    if (nSibling)
    {
        nSibling->addSibling(n);
        return;
    }

    // keep this one and delete the tiles
    // not 100% sure about this
    // might make memory leak
    // its using a shared pointer tho
    n->siblings = siblings;

    // siblings
    nSibling = n;
    n->pSibling = this;

    // update sibling counts
    (*siblings)++;
}

Node *Node::randomChild()
{
    int sibNum = fChild->getSiblings();
    if (sibNum == 1)
        return fChild;

    int index = rand() % sibNum;
    // std::cout << "Random index: " << index << std::endl;

    Node **children = new Node *[sibNum];

    Node *child = fChild;
    for (int i = 0; i < sibNum; i++)
    {
        children[i] = child;
        child = child->nextSibling();
    }

    child = children[index];
    delete[] children;
    return child;
}