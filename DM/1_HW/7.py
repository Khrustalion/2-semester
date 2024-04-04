from itertools import product


n , m = [int(x) for x in input().split()]
graph = [set() for x in range(n)]

for i in range(m):
    start, end = [int(x) for x in input().split()]
    graph[start].add(end)
    graph[end].add(start)



for pr in product(range(n), repeat=n):
    if len(set(pr)) == len(pr):
        diff = set()
        flag = False
        visited = [False] * n
        for i in range(n):
            visited[i] = True
            
            for nv in graph[i]:
                if (not visited[nv]):
                    diff.add(abs(pr[i] - pr[nv]))
        if (len(diff) == m):
            print(pr)

    


