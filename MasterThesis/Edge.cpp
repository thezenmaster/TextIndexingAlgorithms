#include "stdafx.h"
#include "Edge.h"
#include <stdlib.h>

Edge *Edge_Create(int arrayIndex, int startIndex, int endIndex, int nodeIndex)
{
	Edge *edge = (Edge*) malloc(sizeof(Edge));

	edge->arrayIndex = arrayIndex;
	edge->startIndex = startIndex;
	edge->endIndex = endIndex;
	//-1 in the case of open edge.
	edge->nodeIndex = nodeIndex;

	return edge;
}
