#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

// ���������
const int INF = numeric_limits<int>::max();

// Dijkstra�㷨
int dijkstra(const vector<vector<int>>& graph, int source) {
    int num_vertices = graph.size();
    vector<int> dist(num_vertices, INF); // ��ʼ����������
    dist[source] = 0; // Դ�㵽Դ��ľ���Ϊ0
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // ��С��
    pq.push(make_pair(0, source)); // Դ�����

    while (!pq.empty()) {
        int u = pq.top().second; // ȡ��������С�Ķ���
        pq.pop();

        for (int v = 0; v < num_vertices; ++v) {
            if (graph[u][v] > 0) { // ������ڱ�����u��v
                int alt = dist[u] + graph[u][v]; // �����µľ���
                if (alt < dist[v]) { // ����µľ���С�ڵ�ǰ����
                    dist[v] = alt; // ���¾���
                    pq.push(make_pair(dist[v], v)); // ������v���
                }
            }
        }
    }

    int max_dist = 0;
    for (int i = 0; i < num_vertices; ++i) {
        if (dist[i] > max_dist && dist[i] < INF) {
            max_dist = dist[i]; // Ѱ�����һ��·��
        }
    }
    return max_dist; // �����·���ĳ���
}

// ��ȡ�ļ�����ͼ
vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ļ� " << filename << endl;
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
    cout << "���������ͼ���ļ���: ";
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

    cout << "ѡ�񶥵� " << min_source + 1 << " ��Ϊҥ�Ե�Դͷ������ʹҥ�Դ������ʱ����̣�Ϊ " << min_max_dist << "��" << endl;

    return 0;
}


