#ifndef NODE_EDGE_H
#define NODE_EDGE_H
struct NodeEdge{
	char c;
	int nextElement;
	int nextNode;
};
typedef struct Node Node;

NodeEdge *NodeEdge_Create(char c, int nextElement);
#endif