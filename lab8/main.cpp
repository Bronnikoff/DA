// Made by Max Bronnikov
#include <iostream>
#include <vector>
#include <algorithm>

// Labortory №8
// Varint №6

using namespace std;

bool cyclic(int v, vector<vector<int>>& graph, vector <int> &color ) { 
    color[v] = 1;
    for(size_t i = 0; i < graph[v].size(); ++i) {
        int to = graph[v][i];
        if(color[to] == 0){ 
            if(cyclic (to,  graph,  color)){
                return true;
            }
        }
        else if(color[to] == 1){ 
            return true; 
        }
    }
    color[v] = 2; 
    return false;
}


void dfs(int v, vector <vector<int>> graph, vector<bool> &used, vector<int> &answer) {
    used[v] = true; 
    for(int i=0; i < graph[v].size(); i++) {
        int to = graph[v][i]; 
        if(!used[to]){
            dfs(to, graph, used, answer);
        }
    }
    answer.push_back(v+1); 
}
 
void topological_sort(int n, vector <vector<int>> graph, vector<bool> &used, vector<int> &answer) {
    for (int i = 0; i < n; i++) 
        used[i] = false;
    for (int i = 0; i < n; i++)
        if (!used[i])
            dfs(i, graph, used, answer); 
    reverse(answer.begin(), answer.end());
}
 
int main() {
    int N, M; 
    cin >> N >> M;
    int A, B;
    vector<vector<int>> graph(N); 
    vector<bool> used (N);
    vector<int> answer;
    vector<int> color (N,0); 
    for (int i = 0; i < M; i++){
            cin >> A >> B;
            graph[A-1].push_back(B-1);
        }
    for (int i = 0; i < N; i++){
        if (cyclic(i, graph, color)){
            cout << "-1" << endl;
            return 0;
        }
    }
    topological_sort(N, graph, used, answer);
    for (int i = 0; i < answer.size(); i++)
    {
        cout << answer[i] << ' '; 
    }
    cout << endl;
    return 0;
}