G = {
     1: {1: 0, 2: 4, 3: 8},
     2: {1: 4, 2: 0, 3: 11, 4: 8},
     3: {1: 8, 3: 0, 5: 1},
     4: {2: 8, 4: 0, 5: 8, 6: 7, 7: 4},
     5: {3: 1, 4: 8, 5: 0, 7: 2},
     6: {4: 7, 6: 0, 7: 14, 8: 9},
     7: {4: 4, 5: 2, 6: 14, 7: 0, 8: 10},
     8: {6: 9, 7: 10, 8: 0}
     }
def Dijkstra(G, v0, INF=999):  # 999表示该结点到起始点的距离还没有一个确切的值
    dis = dict((i, INF) for i in G.keys())  # 初始化一个距离表，这个表记录着起始结点v0到图中各个点的距离(为inf）
    current = v0  # 一开始，当前点设置为起始点v0
    dis[v0] = 0  # 初始点到自己的距离自然为0
    visited = []  # 用来记录已经遍历过的结点，当把所有的结点都存入的时候，算法结束
    ###
    path = dict((i, []) for i in G.keys())  # 初始化一个路径表，这个表记录着起始结点到途中各个点的最短路径
    path[v0] = str(v0)  # 初始点到自己的路径自然为自己
    ###
 
    while len(G) > len(visited):  # 图的结点还没被遍历完时执行循环以继续遍历
        visited.append(current)  # 当前点正在被遍历，所以把当前点放入visited表中
        for k in G[current]:  # 遍历当前点的所有相邻点
            if dis[current] + G[current][k] < dis[k]:  # 如果（起始点到当前点的距离+当前点到相邻点k的距离）小于（起始点到当前点的相邻点k的距离）
                dis[k] = dis[current] + G[current][k]  # 则（起始点到当前点的相邻点k的距离）更新为（起始点到当前点的距离+当前点到相邻点k的距离）
                seq = (path[current], str(k))# 更新路径
                sym = '-'
                path[k] = sym.join(seq)  # 起始点到（当前点的相邻点k）的最短路径，以'-'来连接seq中的两个字符串
        # 接着来选下一个当前点(current)
        # 从剩下未遍历的点中，选取与当前点的距离最小的那个点作为下一个当前点
        new = INF
        for node in dis.keys():
            if node in visited:
                continue
            if dis[node] < new:
                new = dis[node]
                current = node
 
    return dis, path
v0=1
dis, path = Dijkstra(G, v0)
print("%s 到各个结点的最短距离："%(str(v0)),dis)
print("最短路径为：",path)
