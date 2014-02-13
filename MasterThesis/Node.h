#ifndef NODE_H
#define NODE_H
struct Node{
	/*Position in the array where we keep track of all nodes*/
	int arrayIndex;
	/*Points at the last character of the substring that leads to this node*/
	int textIndex;

	int numberofEdges;
	int edgeStartIndex;
	int lastEdgeIndex;
};
typedef struct Node Node;

Node *Node_Create(int arrayIndex, int textIndex);
#endif