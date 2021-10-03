#include <vector>
#include <string>
#pragma once
using namespace std;

struct cat
{
    string _name;
    vector<cat *> _kittens;

public:
    string name();
    vector<cat *> kittens();
    cat(string);
    void insertKitten(string name);
};

// void LevelOrderTraversal(cat * root)
// {
//     if (root==NULL)
//         return;

//     // Standard level order traversal code
//     // using queue
//     queue<cat *> q;  // Create a queue
//     q.push(root); // Enqueue root
//     while (!q.empty())
//     {
//         int n = q.size();

//         // If this node has children
//         while (n > 0)
//         {
//             // Dequeue an item from queue and print it
//             cat * p = q.front();
//             q.pop();
//             cout << p->name << " ";

//             // Enqueue all children of the dequeued item
//             for (int i=0; i<p->child.size(); i++)
//                 q.push(p->child[i]);
//             n--;
//         }

//         cout << endl; // Print new line between two levels
//     }
// }