// Copyright 2022 Buzea Alexandru-Mihai-Iulian 321CAb
// Contact: alexandru.buzea2007@stud.acs.upb.ro

#ifndef GRAPH_H_
#define GRAPH_H_

// a graph class that stores the number of nodes, number of edges
// and an adjacency matrix containing the edges.
class Graph
{
    public:
        // The fields of the class
        int nodes;
        int edges;
        bool **mat;

        // This method is used to add an edge into the graph
        void addEdge(int first, int second)
        {
            mat[first][second] = true;
            mat[second][first] = true;
        }

        // a constructor for the graph, which uses dynamic memory allocation
        Graph(int n, int m)
        {
            nodes = n;
            edges = m;
            mat = new bool*[n + 1];
            
            for (int i = 0; i <= nodes; ++i)
                mat[i] = new bool[n + 1];

            for (int i = 0; i <= n; ++i)
                for (int j = 0; j <= n; ++j)
                    mat[i][j] = false;
        }

        // A destructor for the graph
        ~Graph()
        {
            for (int i = 0; i <= nodes; ++i)
                delete[] mat[i];

            delete[] mat;
        }
};

#endif  /* GRAPH_H_ */