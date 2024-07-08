#include <iostream>
#include <set>
#include <vector>

void floyd(std::vector<std::vector<int>>& dist, int n, std::vector<std::vector<std::pair<int, int>>>& parent) {
    for (int i = 0; i < n; ++i) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (dist[u][i] != -1 && dist[i][v] != -1 && (dist[u][v] == -1 || dist[u][v] > dist[u][i] + dist[i][v])) {
                    dist[u][v] = dist[u][i] + dist[i][v];
                    parent[u][v] = {u, i};
                }
            }
        }
    }
}

void task_6() {
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

    std::vector<std::vector<std::pair<int, int>>> parent(n, std::vector<std::pair<int, int>>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (dist[i][j] != -1) {
                parent[i][j] = {i, i};
            }
        }
    }

    floyd(dist, n, parent);

    char start_c, end_c;

    std::cin >> start_c >> end_c;

    int start = std::min(start_c - 'a', 20), end = std::min(end_c - 'a', 20);

    std::pair<int, int> cur_v = parent[start][end];

    std::vector<int> path;
    path.push_back(end);
    while (cur_v.first != start || cur_v.second != start) {
        path.push_back(cur_v.second);
        cur_v = parent[cur_v.first][cur_v.second];
    }
    path.push_back(start);
    std::cout << "length path: " << dist[start][end] << '\n';
    std::cout << "path: ";
    for (int i = path.size() - 1; i >= 0; --i) {
        if (path[i] == 20) {
            path[i] = 25;
        }
        std::cout << char(path[i] + 'a') << " ";
    }
    std::cout << '\n';
}