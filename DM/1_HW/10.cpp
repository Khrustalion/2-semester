// def boruvki:
//     mst
//     component[n]
//     while mst.size < n - 1
//         for u из V:
//             minEdge
//             for uv из E:
//                 if component[u] != component[v]:
//                 minEdge = uv
//             mst.add(minEdge)
//             Union(u, v)
//     return mst

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

int find(int*& parent, int v) {
    if (parent[v] != v) {
        parent[v] = find(parent, parent[v]);
    }
    return parent[v];
}

void Union(int v1, int v2, int*& parent) {
    v1 = find(parent, v1);
    v2 = find(parent, v2);
    if (v1 != v2) {
        parent[v1] = v2;
    }
}

std::set<std::pair<int, int>> Boruvki(std::vector<std::vector<int>>& graph, int n) {
    std::set<std::pair<int, int>> mst;

    int* parent = new int[n];

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    while (mst.size() <= n - 1) {
        std::cout << mst.size() << '\n';
        for (int i = 0; i < n; ++i) {
            int vertex = 0;
            int min_edge = -1;
            for (int j = 0; j < n; ++j) {
                if (graph[i][j] != -1 && (find(parent, i) != find(parent, j)) && (min_edge == -1 || min_edge > graph[i][j])) {
                    min_edge = graph[i][j];
                    vertex = j;
                }
            }
            if (min_edge != -1 && std::find(mst.begin(), mst.end(), std::pair<int, int>{vertex, i}) == mst.end()) {
                mst.insert({i, vertex});
            }
        }
        for (auto p : mst) {
            Union(p.first, p.second, parent);
        }
    }
    return mst;
}

void task_10() {
    int n = 21;
    std::vector<std::vector<int>> dist(n);
    //         a  b  c  d  e    f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   z
    dist[0] = {0, 2, 4, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //         a  b  c   d  e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   z
    dist[1] = {2, 0, 3, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //         a  b  c   d  e  f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   z
    dist[2] = {4, 3, 0, -1, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //         a   b   c  d   e  f  g   h   i   j   k   l   m   n   o   p   q   r   s   t   z
    dist[3] = {1, -1, -1, 0, -1, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //          a  b  c   d  e   f   g  h   i   j   k   l   m   n   o   p   q   r   s   t   z
    dist[4] = {-1, 1, 2, -1, 0, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //          a   b  c  d   e  f  g  h  i  j   k   l   m   n   o   p   q   r   s   t   z
    dist[5] = {-1, -1, 2, 5, -1, 0, 3, 3, 2, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //          a   b   c  d   e  f  g   h   i   j  k   l   m   n   o   p   q   r   s   t   z
    dist[6] = {-1, -1, -1, 4, -1, 3, 0, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //          a   b   c   d  e  f   g  h   i   j   k  l   m   n  o   p   q   r   s   t   z
    dist[7] = {-1, -1, -1, -1, 3, 3, -1, 0, -1, -1, -1, 1, -1, -1, 8, -1, -1, -1, -1, -1, -1};
    //          a   b   c   d   e  f   g   h  i  j   k  l  m   n   o   p   q   r   s   t   z
    dist[8] = {-1, -1, -1, -1, -1, 2, -1, -1, 0, 3, -1, 3, 2, -1, -1, -1, -1, -1, -1, -1, -1};
    //          a   b   c   d   e  f   g   h  i  j  k   l  m  n   o   p   q   r   s   t   z
    dist[9] = {-1, -1, -1, -1, -1, 4, -1, -1, 3, 0, 6, -1, 6, 3, -1, -1, -1, -1, -1, -1, -1};
    //           a   b   c   d   e   f  g   h   i  j  k   l   m  n   o   p   q  r   s   t   z
    dist[10] = {-1, -1, -1, -1, -1, -1, 2, -1, -1, 6, 0, -1, -1, 4, -1, -1, -1, 2, -1, -1, -1};
    //           a   b   c   d   e   f   g  h  i   j   k  l  m   n  o   p   q   r   s   t   z
    dist[11] = {-1, -1, -1, -1, -1, -1, -1, 1, 3, -1, -1, 0, 3, -1, 6, -1, -1, -1, -1, -1, -1};
    //           a   b   c   d   e   f   g   h  i  j   k  l  m  n  o  p   q   r   s   t   z
    dist[12] = {-1, -1, -1, -1, -1, -1, -1, -1, 2, 6, -1, 3, 0, 5, 4, 2, -1, -1, -1, -1, -1};
    //           a   b   c   d   e   f   g   h   i  j  k   l  m  n   o   p  q  r   s   t   z
    dist[13] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 4, -1, 5, 0, -1, -1, 2, 1, -1, -1, -1};
    //           a   b   c   d   e   f   g  h   i   j   k  l  m   n  o  p   q   r  s   t   z
    dist[14] = {-1, -1, -1, -1, -1, -1, -1, 8, -1, -1, -1, 6, 4, -1, 0, 2, -1, -1, 6, -1, -1};
    //           a   b   c   d   e   f   g   h   i   j   k   l  m   n  o  p  q   r  s  t   z
    dist[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, 2, 0, 1, -1, 2, 1, -1};
    //           a   b   c   d   e   f   g   h   i   j   k   l   m  n   o  p  q  r   s  t   z
    dist[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, 1, 0, 8, -1, 3, -1};
    //           a   b   c   d   e   f   g   h   i   j  k   l   m  n   o   p  q  r   s  t   z
    dist[17] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, 1, -1, -1, 8, 0, -1, 5, -1};
    //           a   b   c   d   e   f   g   h   i   j   k   l   m   n  o  p   q   r  s   t   z
    dist[18] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6, 2, -1, -1, 0, -1, 2};
    //           a   b   c   d   e   f   g   h   i   j   k   l   m   n   o  p  q  r   s  t   z
    dist[19] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 3, 5, -1, 0, 8};
    //           a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s  t  z
    dist[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, 8, 0};

    std::cout << "here\n";

    auto answer = Boruvki(dist, n);

    std::cout << answer.size() << '\n';

    for (auto v: answer) {
        std::cout << "{" << char('a' + v.first) << ", " << char('a' + v.second) << "}," << " ";
    }
}