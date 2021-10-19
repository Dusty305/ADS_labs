#ifndef listset_h
#define listset_h

#include <iostream>

using namespace std;



class ListSet
{

private:
    struct ListNode
    {
        char field;
        ListNode* next;

        ListNode(char c) : field(c), next(nullptr)
        { }
    } *begin, *end;
    
public:

    ListSet() : begin(nullptr), end(nullptr)
    { }

    ListSet(char arr[]) : begin(nullptr), end(nullptr)
    { 
        ListNode *newnode, *q;
        for (int i = 0; arr[i]; i++) 
            push_back(new ListNode(arr[i]));
        
    }

    void push_back(ListNode* node)
    {
        if (!end)
            begin = end = node;
        else
            end = end->next = node;
    }

    ListSet& operator|=(const ListSet& list)
    {
        for (ListNode* node1 = list.begin; node1; node1 = node1->next) {
            bool in_set = false;
            for (ListNode* node2 = this->begin; node2; node2 = node2->next)
                if (node1->field == node2->field)
                {
                    in_set = true;
                    break;
                }
                if (!in_set)
                    this->push_back(new ListNode(node1->field));
        }

        return *this;
    }

    ListSet operator&(const ListSet& list1)
    {
        ListSet result;
        ListSet list2(*this);

        for (ListNode* node1 = list1.begin; node1; node1 = node1->next)
            for (ListNode* node2 = list2.begin; node2; node2 = node2->next)
                if (node1->field == node2->field)
                    result.push_back(new ListNode(node1->field));

        return move(result);
    }

    friend ostream& operator<<(ostream& os, ListSet list)
    {
        for (ListNode* node = list.begin; node; node = node->next)
            os << node->field << " ";
        return os;
    }

};

#endif