// Copyright 2022 Buzea Alexandru-Mihai-Iulian 321CAb
// Contact: alexandru.buzea2007@stud.acs.upb.ro

#ifndef TASK2_H_
#define TASK2_H_

#include "task.h"

class Task2 : public Task
{
    public:
        // the fields used for this task
        Graph *graph;
        std::vector<int> answer;
        int optimalSize;

        // a method which solves the task (reads the input + translation of
        // the input + calls oracle multiple times in order to get the optimal
        // result + transaltion of the output + writes the output)
        void solve()
        {
            this->read_problem_data();
            int low = 1, high = graph->nodes;

            // we know (obviously) that there is a cover with the nodes of
            // the graph, but we check if there is a cover with a single node
            // aka we have a star graphs
            optimalSize = low;

            this->get_answer();

            // if there is a star graph, then we no longer need to perform
            // binary search.
            if (!answer.empty()) {
                this->write_answer();
                return;
            }

            // binary search
            while(high > low + 1) {
                int medium = (low + high) / 2;
                optimalSize = medium;
                
                this->get_answer();

                if (!answer.empty()) {
                    high = medium;
                    continue;
                } else {
                    low = medium;
                    continue;
                }
            }

            // this is the minimum vertex cover size.
            optimalSize = high;

            // we do not know if the last iteration has been performed
            // using low or high, so we need to call the oracle once more.
            this->formulate_oracle_question();
            this->ask_oracle();
            this->decipher_oracle_answer();

            this->write_answer();
        }

        // a method which performs an oracle call and puts the output in the
        // answer variable
        void get_answer()
        {
            this->formulate_oracle_question();
            this->ask_oracle();
            this->decipher_oracle_answer();
        }

        // A method which reads the problem data (in this case, a graph).
        void read_problem_data()
        {
            int nodes, edges;
            std::cin >> nodes >> edges;

            graph = new Graph(nodes, edges);

            for (int i = 0; i < edges; ++i) {
                int first, second;
                std::cin >> first >> second;
                graph->addEdge(first, second);
            }
        }

        // a method which translates the input of our problem to the
        // input of the CNF-SAT problem. (which will be solved with
        // a call to our oracle)
        void formulate_oracle_question()
        {
            // shorter notations which are also used in the homework's
            // formulation
            int n = graph->nodes;
            int m = graph->edges;
            int k = optimalSize;
        
            // clauses for node existance - for each position in the cover
            int numberOfClauses = k;

            // clauses for vertex cover problem : for each edge, one node must
            // appear in the solution
            numberOfClauses += m;

            // clauses in order to have only one node for a position in
            // vertex cover
            numberOfClauses += k * n * (n - 1) / 2;
            
            // a node must not appear on multiple positions in the cover
            numberOfClauses += n * k * (k - 1) / 2;

            std::ofstream fout("sat.cnf");
            fout << "p cnf" << " " << n << " " << numberOfClauses << "\n";

            // existance clauses
            for (int v = 1; v <= k; ++v) {
                for (int i = 1; i <= n; ++i) {
                    int index = (i - 1) * k + v;
                    fout << index << " ";
                }
                fout << "0\n";
            }

            // the clauses that, for every edge, one of the nodes must appear
            // in the solution.
            for (int i = 1; i <= n - 1; ++i) {
                for (int j = i + 1; j <= n; ++j) {
                    if (graph->mat[i][j] == true) {
                        for (int v = 1; v <= k; ++v) {
                            int index1 = (i - 1) * k + v;
                            int index2 = (j - 1) * k + v;
                            fout << index1 << " " << index2 << " ";
                        }

                        fout << "0\n";
                    }
                }
            }

            // clauses that multiple nodes may not appear on the same position
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

            // a node must not appear on multiple positions in the vertex cover
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

        // A method which translates the output of the oracle to the output
        // of our problem and stores it in the answer variable
        void decipher_oracle_answer()
        {
            std::ifstream fin("sat.sol");
            std::string result;
            fin >> result;

            std::string falseString("false");

            if (result.compare(falseString) == 0)
                return;

            // number of variables
            int v;
            fin >> v;

            int k = optimalSize;

            // prepare the container for getting the result
            answer.clear();

            for (int i = 0; i < v; ++i) {
                int value;
                fin >> value;

                if (value < 0)
                    continue;
                
                int node;

                if (value % k == 0)
                    node = value / k;
                else
                    node = (value / k) + 1;
                
                answer.push_back(node);
            }
        }

        // a method which writes the answer to STDOUT (that is, the content of
        // the answer variable)
        void write_answer()
        {
            std::vector<int>::iterator it;

            for (it = answer.begin(); it != answer.end(); ++it)
                std::cout << *it << " ";

            std::cout << "\n";
        }

        // the destructor for this task
        ~Task2()
        {
            delete graph;
        }
};

#endif  // TASK2_H_