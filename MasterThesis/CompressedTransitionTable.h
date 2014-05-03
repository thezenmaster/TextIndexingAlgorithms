#include "NodeEdge.h"
#include "Node.h"
#include "Edge.h"

#ifndef COMPRESSED_TRANSITION_TABLE
#define COMPRESSED_TRANSITION_TABLE
/*
Input: Reference to an array of the transitions of all tree nodes - 'nodeEdges', reference to an array of all nodes,
number of all nodes of the suffix tree 'numNodes', reference to an array of all edges,
the number of edges 'numEdges'.
Output: Constructs two arrays - 'check' and 'next'. They are used to store nodes transitions in the Suffix tree.
The size of the arrays 'check' and 'next' - 'tableLength'.
*/
void ConstructTable(NodeEdge **nodeEdges, Node** nodes, int numNodes, Edge** edges, int* tableLength, int* check, int* next);
#endif