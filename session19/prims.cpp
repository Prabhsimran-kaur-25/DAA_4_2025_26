#include <iostream>
#include<vector>
using namespace std;
int SpanningTree(int V, vector<vector<int>>& edges) {

    int E = edges.size();
    vector<vector<pair<int,int>>> adj(V);

    for (int i = 0; i < E; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];

        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    }

    vector<bool> visited(V, false);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    pq.push({0, 0});

    int minCost = 0;

    while (!pq.empty()) {

        int wi = pq.top().first;
        int ui = pq.top().second;
        pq.pop();

        if (visited[ui]) continue;

        visited[ui] = true;
        minCost += wi;

        for (auto &pr : adj[ui]) {
            int currW = pr.first;
            int vi = pr.second;

            if (!visited[vi]) {
                pq.push({currW, vi});
            }
        }
    }

    return minCost;
}

int main() {

    int V = 10;

    vector<vector<int>> edges = {
        {0, 5, 2},
        {0, 6, 7},
        {1, 2, 4},
        {1, 5, 8},
        {1, 9, 7},
        {2, 4, 7},
        {3, 0, 2}
    };

    int result = SpanningTree(V, edges);

    cout << "Minimum Cost of MST: " << result << endl;

    return 0;
}