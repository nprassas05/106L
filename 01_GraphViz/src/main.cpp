#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <sstream>
#include <limits>
#include <cctype>
#include "SimpleGraph.h"


const double kPi = 3.14159265358979323;
const double KRepel = 10e-3;
const double kAttract = 10e-3;

using std::cout;	using std::endl;
using std::cin;
using std::string;

bool fileExists(string);
string getGraphFile();
void fillSimpleGraphFromFile(const string &graphFile, SimpleGraph &graph);
void updateGraphLayout(SimpleGraph &graph);
void exertRepulsiveForces(SimpleGraph &graph);
void exertAttractiveForces(SimpleGraph &graph);
char getYesOrNo();

// Main method
int main() {

    while (true) {
        string graphFile = getGraphFile();
        SimpleGraph graph;
        fillSimpleGraphFromFile(graphFile, graph);
        InitGraphVisualizer(graph);

        int i = 1;
        while (true) {
            updateGraphLayout(graph);
            DrawGraph(graph);
            if (++i > 5000) break;
        }

        cout << "Would you like to continue visualizing graphs?(y or n): ";
        char ans = getYesOrNo();
        if (ans == 'n') {
            cout << "Goodbye! See you next time!" << std::flush;
            break;
        }
    }

    return 0;
}

char getYesOrNo() {
    char c, remaining;
    string line;

    while (true) {
        getline(cin, line);
        std::istringstream input(line);
        input >> c;

        if (input.fail()) {
            cout << "Must enter a character (y or n). Try again: ";
        } else if (input >> remaining) {
            cout << "Invalid answer. Too many characters entered. "
                 <<  "Enter Only one character answer (y or n).\nTry again: ";
        } else if (c != 'y' && c != 'n') {
            cout << "Invalid answer.  Must be the character y or n. Try again: ";
        } else {
            return c;
        }
    }
}

bool fileExists(string filename) {
    std::ifstream input(filename);
    return input.good();
}

string getGraphFile() {
    string filename;
    string line;
    char remaining;

    while (true) {
        cout << "Enter the name of a file containing a graph to visualize: ";
        getline(cin, line);
        std::stringstream input(line);
        input >> filename;

        if (filename.empty()) {
            cout << "Cannot enter an empty string.  Please try again" << endl;
        } else if (input >> remaining) {
            cout << "Entered Extra characters.  Please try again";
        } else if (!fileExists(filename)) {
            cout << "The file " << filename <<
                    "could not be found, please try again"
                 << endl;
        } else {
            break;
        }
    }

    return filename;
}

void fillSimpleGraphFromFile(const string &graphFile, SimpleGraph &graph) {
    std::ifstream input(graphFile.c_str());
    assert(input.good());
    int numNodes;
    size_t u, v;

    input >> numNodes;

    while (input >> u >> v) {
        graph.edges.push_back(Edge{u, v});
    }
    for (int i = 0; i < numNodes; ++i) {
        double xPos = cos((2 * i * kPi) / numNodes);
        double yPos = sin((2 * i * kPi) / numNodes);
        graph.nodes.push_back(Node{xPos, yPos});
    }
}

void updateGraphLayout(SimpleGraph &graph) {
    exertRepulsiveForces(graph);
    exertAttractiveForces(graph);
}


void exertRepulsiveForces(SimpleGraph &graph) {
    std::vector<double> xChanges(graph.nodes.size(), 0);
    std::vector<double> yChanges(graph.nodes.size(), 0);

    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        double x0 = graph.nodes[i].x;
        double y0 = graph.nodes[i].y;

        for (size_t j = i + 1; j < graph.nodes.size(); ++j) {
            double x1 = graph.nodes[j].x;
            double y1 = graph.nodes[j].y;

            double fRepel = KRepel / sqrt(
                        pow(y1 - y0, 2) +
                        pow(x1 - x0, 2)
                        );
            double theta = atan2(y1 - y0, x1 - x0);
            xChanges[i] -= (fRepel * cos(theta));
            yChanges[i] -= (fRepel * sin(theta));
            xChanges[j] += (fRepel * cos(theta));
            yChanges[j] += (fRepel * sin(theta));

        }
    }

    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        graph.nodes[i].x += xChanges[i];
        graph.nodes[i].y += yChanges[i];
    }
}


void exertAttractiveForces(SimpleGraph &graph) {
    std::vector<double> xChanges(graph.nodes.size(), 0);
    std::vector<double> yChanges(graph.nodes.size(), 0);

    for (size_t i = 0; i < graph.edges.size(); ++i) {
        size_t u = graph.edges[i].start;
        size_t v = graph.edges[i].end;

        double x0 = graph.nodes[u].x;
        double y0 = graph.nodes[u].y;
        double x1 = graph.nodes[v].x;
        double y1 = graph.nodes[v].y;

        double fAttract = kAttract * (
                    pow(x0 - x1, 2) + pow(y0 - y1, 2));
        double theta = atan2(y1 - y0, x1 - x0);

        xChanges[u] += (fAttract * cos(theta));
        yChanges[u] += (fAttract * sin(theta));
        xChanges[v] -= (fAttract * cos(theta));
        yChanges[v] -= (fAttract * sin(theta));
    }

    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        graph.nodes[i].x += xChanges[i];
        graph.nodes[i].y += yChanges[i];
    }
}
