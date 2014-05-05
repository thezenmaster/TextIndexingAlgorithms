#include "stdafx.h"
#include "Edge.h"
#include <stdlib.h>

Edge *Edge_Create(int arrayIndex, int startIndex, int nodeIndex)
{
	Edge *edge = (Edge*) malloc(sizeof(Edge));

	edge->arrayIndex = arrayIndex;
	edge->startIndex = startIndex;
	/*-1 indicates the edge is open.*/
	edge->endIndex = -1;
	edge->nodeIndex = nodeIndex;

	return edge;
}
