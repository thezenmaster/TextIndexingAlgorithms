#ifndef NODE_H
#define NODE_H
struct Node{
	/*Position in the array where we keep track of all nodes*/
	int arrayIndex;
	int numberofEdges;
	int isLeaf;

	/*Specifes to which string Si from the set of strigs
	this (leaf) node belongs*/
	int stringIndex;

	/*Fields used by the temporary list of edges for all nodes*/
	int edgeStartIndex;
	int currentEdgeIndex;

	/*Base address in the table of transitions*/
	int baseAddress;
};
typedef struct Node Node;

Node *Node_Create(int arrayIndex, int textIndex, int isLeaf);
#endif