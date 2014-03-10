#ifndef NODE_EDGE_H
#define NODE_EDGE_H
struct NodeEdge{
	char symbol;
	int nextElement;
	int edgeIndex;
	//int nextNode;
};
typedef struct NodeEdge NodeEdge;

NodeEdge *NodeEdge_Create(char c, int edgeIndex);
#endif