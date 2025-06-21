import sys as s
s.setrecursionlimit(100000000)
# 弗洛伊德算法
def floyd():
    n = len(graph)
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if graph[i][k] + graph[k][j] < graph[i][j]:
                    graph[i][j] = graph[i][k] + graph[k][j]#更新距离
                    parents[i][j] = parents[k][j]  # 更新父结点
# 打印路径
def print_path(i, j):
    if i != j:
        print_path(i, parents[i][j]-1)
    print(j+1, end='->')
 
datas = [
    [1, 2, 9],[1, 3, 3],
    [2, 1, 9],[2, 4, 4],
    [3, 1, 3],[3, 2, 2],[3, 4, 8],[3, 5, 2],[3, 6, 7],
    [4, 2, 1],[4, 3, 8],[4, 7, 5],[4, 10, 9],
    [5, 3, 2],[5, 6, 8],[5, 7, 6],[5, 8, 4],
    [6, 3, 7],[6, 5, 9],[6, 8, 2],
    [7, 4, 5],[7, 5, 6],[7, 8, 1],[7, 9, 2],[7, 10, 3],
    [8, 5, 4],[8, 6, 1],[8, 7, 3],[8, 9, 6],
    [9, 7, 2],[9, 8, 6],[9, 10, 5],
    [10, 7, 3],[10, 9, 5],
]
n = 10
# 定义无穷大
inf = 9999999999
# 初始图的权值数据
graph = [[(lambda x: 0 if x[0] == x[1] else inf)([i, j]) for j in range(n)] for i in range(n)]
parents = [[i+1] * n for i in range(n)]  # 关键地方，i-->j 的父结点初始化都为i
for u, v, c in datas:
    graph[u-1][v-1] = c	# 因为是有向图，边权只赋给graph[u][v]
    #graph[v][u] = c # 如果是无向图，要加上这条。
floyd()
'''
print('Weight:')
for row in graph:
    for e in row:
        print('∞' if e == inf else e, end='\t')
    print()
print()
print('Path:')
'''
for i in range(n):
    for j in range(n):
        print('Path({}->{}): '.format(i+1, j+1), end='')
        print_path(i, j)
        print(' Weight:', graph[i][j])
