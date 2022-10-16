// Copyright 2022 Buzea Alexandru-Mihai-Iulian 321CAb
// Contact: alexandru.buzea2007@stud.acs.upb.ro

#ifndef TASK1_H_
#define TASK1_H_

#include "task.h"

class Task1 : public Task
{
    public:

        // the fields used in order to solve the problem
        Graph *graph;
        int cliqueSize;
        std::vector<int> answer;

        // the solve method used to perform all operations
        void solve()
        {
            this->read_problem_data();
            this->formulate_oracle_question();
            this->ask_oracle();
            this->decipher_oracle_answer();
            this->write_answer();
        }

        // A method which parses data from STDIN
        void read_problem_data()
        {
            int nodes, edges, cliqueSize;
            std::cin >> nodes >> edges >> cliqueSize;

            // creating the graph and reading the edges
            graph = new Graph(nodes, edges);

            for (int i = 0; i < edges; ++i) {
                int first, second;
                std::cin >> first >> second;
                graph->addEdge(first, second);
            }

            this->cliqueSize = cliqueSize;
        }

        // translates the given input to the input for the CNF-SAT problem
        void formulate_oracle_question()
        {
            // shorter notations which are also used in the homework's
            // formulation
            int n = graph->nodes;
            int m = graph->edges;
            int k = cliqueSize;
        
            // clauses for node existance - for each position in the clique
            int numberOfClauses = k;

            // clauses for non-edge check
            numberOfClauses += (n * (n - 1) / 2 - m) * k * (k - 1);

            // clauses in order to have only one node for a position in clique
            numberOfClauses += k * n * (n - 1) / 2;
            
            // a node must not appear on multiple positions in the clique
            numberOfClauses += n * k * (k - 1) / 2;

            std::ofstream fout("sat.cnf");
            fout << "p cnf" << " " << n << " " << numberOfClauses << "\n";

            // existance clauses - for each position in our clique, we need
            // to have at least one node that occupies it.
            for (int v = 1; v <= k; ++v) {
                for (int i = 1; i <= n; ++i) {
                    int index = (i - 1) * k + v;
                    fout << index << " ";
                }
                fout << "0\n";
            }

            // non-edge clauses: two nodes that are not connected by an edge
            // are not allowed simultaneously be in the clique.
            for (int i = 1; i <= n - 1; ++i) {
                for (int j = i + 1; j <= n; ++j) {
                    if (graph->mat[i][j] == false) {
                        for (int v = 1; v <= k - 1; ++v) {
                            for (int w = v + 1; w <= k; ++w) {
                                int index1 = (i - 1) * k + v;
                                int index2 = (j - 1) * k + w;
                                int index3 = (i - 1) * k + w;
                                int index4 = (j - 1) * k + v;

                                fout << "-" << index1 << " ";
                                fout << "-" << index2 << " " << "0\n";
                                fout << "-" << index3 << " ";
                                fout << "-" << index4 << " " << "0\n";
                            }
                        }
                    }
                }
            }

            // clauses that multiple nodes must not appear on the same position
            for (int v = 1; v <= k; ++v) {
                for (int i = 1; i <= n - 1; ++i) {
                    for (int j = i + 1; j <= n; ++j) {
                        int index1 = (i - 1) * k + v;
                        int index2 = (j - 1) * k + v;

                        fout << "-" << index1 << " ";
                        fout << "-" << index2 << " " << "0\n";
                    }
                }
            }

            // a node must not appear on multiple positions in the clique
            for (int i = 1; i <= n; ++i) {
                for (int v = 1; v <= k - 1; ++v) {
                    for (int w = v + 1; w <= k; ++w) {
                        int index1 = (i - 1) * k + v;
                        int index2 = (i - 1) * k + w;

                        fout << "-" << index1 << " ";
                        fout << "-" << index2 << " " << "0\n";
                    }
                }
            }
        }

        // after we used the CNF-SAT solver, we need to translate the
        // given output back to our problem's output.
        void decipher_oracle_answer()
        {
            std::ifstream fin("sat.sol");
            std::string result;
            fin >> result;

            std::string falseString("false");

            // if the given output consists just of the string "False",
            // the result is that there is no such clique.
            if (result.compare(falseString) == 0)
                return;

            // number of variables
            int v;
            fin >> v;

            int k = cliqueSize;

            for (int i = 0; i < v; ++i) {
                int value;
                fin >> value;

                if (value < 0)
                    continue;
                
                int node;

                // translating the positive-valued outputs into
                // graph's nodes
                if (value % k == 0)
                    node = value / k;
                else
                    node = (value / k) + 1;
                
                answer.push_back(node);
            }
        }

        // A method which writes the deciphered answer at STDOUT
        void write_answer()
        {
            // if we have no clique, write this response.
            if (answer.empty()) {
                std::cout << "False" << "\n";
                return;
            }

            // otherwise, write the corresponding output
            std::cout << "True" << "\n";

            std::vector<int>::iterator it;

            for (it = answer.begin(); it != answer.end(); ++it)
                std::cout << *it << " ";

            std::cout << "\n";
        }

        // destructor for the Task1's fields.
        ~Task1()
        {
            delete graph;
        }
};

#endif  // TASK1_H_