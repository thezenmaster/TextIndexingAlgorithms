#ifndef EDGE_H
#define EDGE_H

struct Edge {
	int arrayIndex;
	int startIndex;
	int endIndex;
	int nodeIndex;
	int isLeaf;
};
typedef struct Edge Edge;

Edge *Edge_Create(int arrayIndex, int startIndex, int endIndex, int nodeIndex);
#endif // !EDGE_H


