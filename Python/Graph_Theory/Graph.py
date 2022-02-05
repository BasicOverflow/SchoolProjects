from ADT import Queue, PriorityQueue

class Graph(object):
    
    def __init__(self, V=(), E=()):
        raise NotImplementedError

    def add_vertex(self, v):
        raise NotImplementedError

    def remove_vertex(self, v):
        raise NotImplementedError

    def add_edge(self, u, v):
        raise NotImplementedError

    def remove_edge(self, u, v):
        raise NotImplementedError

    def neighbors(self, v):
        raise NotImplementedError

    def weight(self, u, v):
        raise NotImplementedError



class AdjacencySetGraph(Graph):
    def __init__(self, V = (), E = ()):
        self._V = set()
        self._nbrs = {}

        for v in V: 
            self.add_vertex(v)
        for e in E: 
            self.add_edge(*e)

    def vertices(self):
        return iter(self._V)

    def edges_helper(self):
        for u in self._V:
            for v in self._nbrs(u):
                yield (u,v)
    
    def edges(self):
        E = {frozenset(e) for e in self.edges_helper}
        return iter(E)

    def add_edge(self, u, v, w=None):
        self._nbrs[u][v] = w
        self._nbrs[v][u] = w

    def remove_edge(self, u, v):
        del self._nbrs[u][v]
        del self._nbrs[v][u]

    def add_vertex(self, v):
        self._V.add(v)
        self._nbrs[v] = {}

    def weight(self, u, v):
        return self._nbrs[u][v]

    def neighbors(self, v):
        return iter(self._nbrs[v])

    def dfs(self, v):
        tree = {}
        tovisit = [(None, v)]
        while tovisit:
            a,b = tovisit.pop()
            if b not in tree:
                tree[b] = a
                for n in self.neighbors(b):
                    tovisit.append((b,n))
        return tree

    def bfs(self, v): 
        tree = {}
        tovisit = Queue()
        tovisit.enqueue((None, v))
        while tovisit:
            a,b = tovisit.dequeue()
            if b not in tree:
                tree[b] = a
                for n in self.neighbors(b):
                    tovisit.enqueue((b,n))
        return tree

    def dijkstra(self, v):
        tree = {v: None}
        D = {u: float("inf") for u in self.vertices()}
        D[v] = 0
        tovisit = PriorityQueue(entries = [(u, D[u]) for u in self.vertices()])
        for u in tovisit:
            for n in self.neighbors(u):
                if D[u] + self.weight(u,n) < D[n]:
                    D[n] = D[u] + self.weight(u,n)
                    tree[n] = u
                    tovisit.changepriority(n, D[n])
        return tree, D

    def primm(self, v):
        tree = {}
        tovisit = PriorityQueue()
        tovisit.insert((None, v), 0)
        for a, b in tovisit:
            if b not in tree:
                tree[b] = a
                for n in self.neighbors(b):
                    tovisit.insert((b,n), self.weight(b,n))
        return tree



class EdgeSetGraph(Graph):

    def __init__(self, V=(), E=()):
        self._V = set()
        self._E = set()

        for vert in V: 
            self.add_vertex(vert)

        for e in E:
            self.add_edge(*e)

    def vertices(self):
        return iter(self._V)

    def add_vertex(self, v):
        self._V.add(v)

    def remove_vertex(self, v):
        self._V.remove(v)

    def add_edge(self, u, v, w=None):
        self._E.add(frozenset({u,v,w}))

    def remove_edge(self, u, v, w=None):
        self._E.remove(frozenset({u,v,w}))

    def neighbors(self, v):
        for u, w, x in self._E:
            for i in u,w,x:
                if type(i) == type(float(4)):
                    if i == x: pass
                    elif i == u:
                        new_u = x
                        x = u
                        u = new_u
                    elif i == w:
                        new_w = x
                        x = w
                        w = new_w

            if u == v:
                yield w
            elif w == v:
                yield u

    def weight(self, u, v):
        for e in self._E:
            if u in e and v in e:
                for e0 in e:
                    if type(e0) == type(float()):
                        return e0

    def bfs(self, v):         
        tree = {}
        tovisit = Queue()
        tovisit.enqueue((None, v))
        while tovisit:
            a,b = tovisit.dequeue()
            if b not in tree:
                tree[b] = a
                for n in self.neighbors(b):
                    tovisit.enqueue((b,n))
        return tree

    def dfs(self, v):
        tree = {}
        tovisit = [(None, v)]
        while tovisit:
            a,b = tovisit.pop()
            if b not in tree:
                tree[b] = a
                for n in self.neighbors(b):
                    tovisit.append((b,n))
        return tree

    def dijkstra(self, v):
        tree = {v: None}
        D = {u: float("inf") for u in self.vertices()}
        D[v] = 0
        entries = [(u, D[u]) for u in self.vertices()]
        tovisit = PriorityQueue(entries = entries)
        for u in tovisit:
            for n in self.neighbors(u):
                if D[u] + self.weight(u,n) < D[n]:
                    D[n] = D[u] + self.weight(u,n)
                    tree[n] = u
                    tovisit.changepriority(n, D[n])
        return tree, D

    def primm(self, v): 
        tree = {}
        tovisit = PriorityQueue()
        tovisit.insert((None, v), 0)
        for a, b in tovisit:
            if b not in tree:
                tree[b] = a
                for n in self.neighbors(b):
                    tovisit.insert((b,n), self.weight(b,n))
        return tree






if __name__ == "__main__":
    V = {1, 2, 3}
    E = {(1, 2), (2, 3)}
    g = EdgeSetGraph(V, E)
    h = AdjacencySetGraph(V,E)

    # print(g.bfs(2))
