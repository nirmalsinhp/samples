#ifndef WGRAPH_H
#define WGRAPH_H

#include <vector>
#include <queue>
#include <deque>
#include <iostream>
#include <limits>
#include <cassert>

struct Edge
{
    int sv;
    int tv;
    double weight;

    Edge(int s, int t, double w);
    Edge();
    ~Edge();
    Edge(const Edge &oth);
    Edge &operator=(const Edge &oth);
    Edge(Edge &&oth);
    Edge &operator=(Edge &&oth);
};

std::ostream &operator<<(std::ostream &os, const Edge &E);
bool operator<(const Edge &lhs, const Edge &rhs);

using AL = std::vector<std::vector<Edge>>;

class Graph
{
public:
    Graph();
    Graph(int num_v, bool d = false);

    void insert_edge(int s, int t, double w);
    void init();
    double prim(int s);
    void printMST() const;
    void printG() const;
    void visit(int v) const;
    void dfs(int start);
    const std::vector<int> preorder() const;
    const std::vector<int> postorder() const;
    const std::vector<int> topological_order();
    std::vector<int> Dijkstra(int s);
    void bellman_ford(int s);
    void relax(const Edge &e);
    void bfs(int start);
    int components();
    int ford_fulkerson(int s, int t);

private:
    void connected_components();
    void scan(int s, std::priority_queue<Edge> &pq);
    bool bfs_ff(std::vector<std::vector<int>> &rGraph, int s, int t, std::vector<int> &parent) const;

    unsigned nv;
    unsigned ne;
    AL al;
    std::vector<bool> visited;
    std::vector<int> pre;
    std::vector<int> post;
    std::vector<int> topo_sort;
    std::vector<int> parent;
    std::vector<double> dist;
    int num_components;
    bool directed;
    std::vector<Edge> MST;
};

#endif // WGRAPH_H
