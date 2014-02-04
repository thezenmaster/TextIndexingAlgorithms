#ifndef EDGE_H
#define EDGE_H

struct Edge {
	int startIndex;
	int endIndex;
	int nodeIndex;
};
typedef struct Edge Edge;

Edge *Edge_Create(int startIndex, int endIndex, int nodeIndex);
#endif // !EDGE_H


