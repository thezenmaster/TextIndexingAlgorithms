#ifndef EDGE_H
#define EDGE_H

struct Edge {
	int arrayIndex;
	int startIndex;
	int endIndex;
	int endNodeIndex;
};
typedef struct Edge Edge;

Edge *Edge_Create(int arrayIndex, int startIndex, int nodeIndex);
#endif // !EDGE_H


