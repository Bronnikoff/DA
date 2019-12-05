// Made by Max Bronnikov
#include <iostream>
#include <vector>


long long int INF = 10000000000000001;

struct edge {
    int a;
    int b;
    int cost;
};

int main() {
    std::ios::sync_with_stdio(false);
    int N = 0, M = 0, start, finish;
    std::cin >> N >> M >> start >> finish;
    std::vector<edge> e(M);
    std::vector<long long int> d(N, INF);
    d[start - 1] = 0;
    int a, b, cost;

    for(int i = 0; i < M; ++i) {
        std::cin >> a >> b >> cost;
        e[i].a = a - 1;
        e[i].b = b - 1;
        e[i].cost = cost;
    }

    for(int j = 0; j < N - 1; ++j) {
        bool any = false;
        for(int i = 0; i < e.size(); ++i) {
            if(d[e[i].a] < INF) {
                if(d[e[i].b] > d[e[i].a] + e[i].cost) {
                    d[e[i].b] = d[e[i].a] + e[i].cost;
                    any = true;
                }
            }
        }
        if(!any) break;
    }

    if(d[finish - 1] == INF) {
        std::cout << "No solution\n";
    } else {
        std::cout << d[finish - 1] << '\n';
    }
    return 0;
}