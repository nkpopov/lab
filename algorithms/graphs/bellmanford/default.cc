
#include <queue>
#include <vector>
#include <limits>
#include <utility>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

// Weighted edge a-(w)->b
struct Edge
{
  int a;
  int b;
  int w;
};

class Graph
{
public:

  // Make empty graph.
  Graph();

  // Add weighted edge a-(w)->b.
  void add_edge(int a, int b, int w);

  // Get edges that lead to successors of node a.
  const std::vector<Edge>& successors(int a) const;

  // Get all vertices of the graph.
  const std::unordered_set<int>& vertices() const;

  // Get all edges of the graph.
  const std::vector<Edge>& edges() const;

private:

  // Adjacency list graph representation.
  std::unordered_map<int, std::vector<Edge>> adj_list_;

  // All vertices of the graph.
  std::unordered_set<int> vertices_;

  // Empty vector of successors to return.
  std::vector<Edge> empty_;

  // All edges of the graph.
  std::vector<Edge> edges_;
};

Graph::Graph()
{
}

void Graph::add_edge(int a, int b, int w)
{
  adj_list_[a].push_back({a, b, w});
  vertices_.insert(a);
  vertices_.insert(b);
  edges_.push_back({a, b, w});
}

const std::vector<Edge>& Graph::successors(int a) const
{
  const auto it = adj_list_.find(a);
  if (it == adj_list_.cend())
  {
    return empty_;
  }
  return it->second;
}

const std::unordered_set<int>& Graph::vertices() const
{
  return vertices_;
}

const std::vector<Edge>& Graph::edges() const
{
  return edges_;
}

// Complexity: O(VE).
void bellman_ford(
  const Graph& graph,
  const int v,
  std::unordered_map<int, int>& distances)
{
  // Initialize distances map.
  const auto& edges = graph.edges();
  const auto& vertices = graph.vertices();
  const std::size_t n = vertices.size();
  distances.reserve(vertices.size());
  for (const int node: vertices)
  {
    distances[node] = std::numeric_limits<int>::max();
  }
  distances[v] = 0;

  for (std::size_t i = 0; i < n - 1; ++i)
  {
    for (const auto& edge: edges)
    {
      const int a = edge.a;
      const int b = edge.b;
      const int w = edge.w;
      if (distances[a] == std::numeric_limits<int>::max())
      {
	continue;
      }
      const int d = distances[a] + w;
      if (distances[b] > d)
      {
	distances[b] = d;
      }
    }
  }

  for (const auto& edge: edges)
  {
      const int a = edge.a;
      const int b = edge.b;
      const int w = edge.w;
      if (distances[a] == std::numeric_limits<int>::max())
      {
	  continue;
      }
      const int d = distances[a] + w;
      if (distances[b] > d)
      {
	  distances.clear();
	  return;
      }
  }
}


int main()
{

  // 0----(4)---->1----(3)---->3
  // |            ^
  // |            |
  // |           (2)
  // |            |
  // +----(1)---->2
  Graph graph;
  graph.add_edge(0, 1, 4);
  graph.add_edge(0, 2, 1);
  graph.add_edge(2, 1, 2);
  graph.add_edge(1, 3, 3);
  std::unordered_map<int, int> distances;
  bellman_ford(graph, 0, distances);
  for (const auto& [v, d]: distances)
  {
    std::cout << "v, d: " << v << ", " << d << "\n";
  }
  return 0;
}
