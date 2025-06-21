#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

// 定义无穷大
const int INF = numeric_limits<int>::max();

// Dijkstra算法
int dijkstra(const vector<vector<int>>& graph, int source) {
    int num_vertices = graph.size();
    vector<int> dist(num_vertices, INF); // 初始化距离数组
    dist[source] = 0; // 源点到源点的距离为0
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // 最小堆
    pq.push(make_pair(0, source)); // 源点入队

    while (!pq.empty()) {
        int u = pq.top().second; // 取出距离最小的顶点
        pq.pop();

        for (int v = 0; v < num_vertices; ++v) {
            if (graph[u][v] > 0) { // 如果存在边连接u和v
                int alt = dist[u] + graph[u][v]; // 计算新的距离
                if (alt < dist[v]) { // 如果新的距离小于当前距离
                    dist[v] = alt; // 更新距离
                    pq.push(make_pair(dist[v], v)); // 将顶点v入队
                }
            }
        }
    }

    int max_dist = 0;
    for (int i = 0; i < num_vertices; ++i) {
        if (dist[i] > max_dist && dist[i] < INF) {
            max_dist = dist[i]; // 寻找最长的一条路径
        }
    }
    return max_dist; // 返回最长路径的长度
}

// 读取文件构建图
vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件 " << filename << endl;
        exit(1);
    }

    int num_vertices;
    file >> num_vertices;
    vector<vector<int>> graph(num_vertices, vector<int>(num_vertices, 0));

    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            file >> graph[i][j];
        }
    }

    file.close();
    return graph;
}

int main() {
    string filename;
    cout << "请输入包含图的文件名: ";
    cin >> filename;

    vector<vector<int>> graph = readGraphFromFile(filename);
    int num_vertices = graph.size();

    int min_max_dist = INF;
    int min_source = -1;
    for (int source = 0; source < num_vertices; ++source) {
        int max_dist = dijkstra(graph, source);
        if (max_dist < min_max_dist) {
            min_max_dist = max_dist;
            min_source = source;
        }
    }

    cout << "选择顶点 " << min_source + 1 << " 作为谣言的源头，可以使谣言传播的最长时间最短，为 " << min_max_dist << "。" << endl;

    return 0;
}


