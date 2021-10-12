#ifndef listset_h
#define listset_h

#include <iostream>

using namespace std;


class ListSet
{

private:
    struct List
    {
        char field;
        struct List *next;
    }node;

    List *create_node(char C)
    {
        List *NewNode=NULL;

        List *NewNode = new List;
        if(NewNode)
        {
            NewNode->field = C;
            NewNode->next=NULL;
        }
        return NewNode;
    }

    void insert_after(List *my_node,List *current_node)
    {
        if(my_node&&current_node)
        {
            if(current_node->next==NULL)
            {
                current_node->next=my_node;
            }
            else
            {
                my_node->next=current_node->next;
                current_node->next=my_node;
            }
        }
    }

    
public:

    ListSet(char arr[])
    { 
        List *newnode, *q;
        for (int i = 0; arr[i]; i++)
            newnode = create_node()
            if(i)
            {
                insert_after(newnode,q);
            }
            q = newnode;
    }

    ListSet& operator|=(const ListSet& list2)
    {
        ListSet result(*this);

        bool in_set;

        for (List it1 = list2.node; it1; it1 = it1.next) {
            in_set = false;
            for (List it2 = result.node; it2; it2 = it2.next)
                if (*it1 == *it2)
                {
                    in_set = true;
                    break;
                }
            if (!in_set)
                result.insert_after(*it2,*it1);
        }

        return *this;
    }

    ListSet operator&(const ListSet& list1)
    {
        ListSet result;
        ListSet list2(*this);

        for (auto it1 = list1.cbegin(); *it1; it1++)
            for (auto it2 = list2.cbegin(); *it2; it2++)
                if (*it1 == *it2)
                    result.push_back(*it1);
        return move(result);
    }

    friend ostream& operator<<(ostream& os, ListSet l)
    {
        for (char c : l)
            os << c << " ";
        return os;
    }

};

#endif