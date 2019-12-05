#include <iostream>
#include <queue>

using namespace std;

        typedef struct Node{
            uint32_t count;
            char value;
        }node;
        typedef struct functor{
            bool operator()(const node& lhs, const node& rhs){
                return lhs.count > rhs.count;
            }
        }comparator;

int main(){
    priority_queue<node, vector<node>, comparator> que;
    for(int i = 0; i < 26; ++i){
        que.push(node{i, 'a' + i});
    }
    for(int i = 0; i < 26; ++i){
        node n = que.top();
        que.pop();
        cout << n.count << " : " << n.value << endl;
    }

    return 0;
}