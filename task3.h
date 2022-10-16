// Copyright 2022 Buzea Alexandru-Mihai-Iulian 321CAb
// Contact: alexandru.buzea2007@stud.acs.upb.ro

#ifndef TASK3_H_
#define TASK3_H_

#include "task.h"

class Task3 : public Task
{
    public:

        // the fields of the class
        Graph *graph;
        int numberOfColors;
        int *answer;
        bool result;

        // A method which solves the current task.
        void solve()
        {
            this->read_problem_data();
            this->formulate_oracle_question();
            this->ask_oracle();
            this->decipher_oracle_answer();
            this->write_answer();
        }

        // A method which reads (parses) the input for our problem
        // from STDIN
        void read_problem_data()
        {
            int nodes, edges, numberOfColors;
            std::cin >> nodes >> edges >> numberOfColors;

            graph = new Graph(nodes, edges);
            answer = new int[nodes + 1];
            result = false;

            for (int i = 0; i < edges; ++i) {
                int first, second;
                std::cin >> first >> second;
                graph->addEdge(first, second);
            }

            this->numberOfColors = numberOfColors;
        }

        void formulate_oracle_question()
        {
            // shorter notations which are also used in the homework's
            // formulation
            int n = graph->nodes;
            int m = graph->edges;
            int k = numberOfColors;
        
            // clauses for node existance - for each existing color, we must
            // have at least one node colored with that color
            int numberOfClauses = k;

            // clauses for adjacent nodes to not be colored with
            // the same color
            numberOfClauses += m * k;

            // clauses for every node to be colored with at least one color
            numberOfClauses += n;
            
            // clauses for every node to be colored with at most one color
            numberOfClauses += n * k * (k - 1) / 2;

            std::ofstream fout("sat.cnf");
            fout << "p cnf" << " " << n << " " << numberOfClauses << "\n";

            // there must be at least one node for each color
            for (int v = 1; v <= k; ++v) {
                for (int i = 1; i <= n; ++i) {
                    int index = (i - 1) * k + v;
                    fout << index << " ";
                }
                fout << "0\n";
            }

            // adjacent nodes clauses
            for (int i = 1; i <= n - 1; ++i) {
                for (int j = i + 1; j <= n; ++j) {
                    if (graph->mat[i][j] == true) {
                        for (int v = 1; v <= k; ++v) {
                            int index1 = (i - 1) * k + v;
                            int index2 = (j - 1) * k + v;

                            fout << "-" << index1 << " ";
                            fout << "-" << index2 << " " << "0\n";
                        }
                    }
                }
            }

            // a node must have at least one color
            for (int i = 1; i <= n; ++i) {
                for (int v = 1; v <= k; ++v) {
                    int index = (i - 1) * k + v;

                    fout << index << " "; 
                }
                fout << "0\n";
            }

            // a node must have at most one color
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

        // a function which performs the translation of the output given
        // by the oracle (SAT solver) and saves the output to our problem
        // in the answer field.
        void decipher_oracle_answer()
        {
            std::ifstream fin("sat.sol");
            std::string resultString;
            fin >> resultString;

            std::string falseString("False");

            if (resultString.compare(falseString) == 0) {
                result = false;
                return;
            }

            result = true;

            // number of variables
            int v;
            fin >> v;

            int k = numberOfColors;

            for (int i = 0; i < v; ++i) {
                int value;
                fin >> value;

                if (value < 0)
                    continue;
                
                int node, color;

                if (value % k == 0) {
                    node = value / k;
                    color = k;
                } else {
                    node = (value / k) + 1;
                    color = value % k;
                }

                answer[node] = color;
            }
        }

        // A function which prints the output of our problem to STDOUT.
        void write_answer()
        {
            if (!result) {
                std::cout << "False" << "\n";
                return;
            }

            std::cout << "True" << "\n";

            for (int i = 1; i <= graph->nodes; ++i)
                std::cout << answer[i] << " ";

            std::cout << "\n";
        }

        ~Task3()
        {
            delete graph;
            delete[] answer;
        }
};

#endif  // TASK1_H_