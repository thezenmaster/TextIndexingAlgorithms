#include "stdafx.h"
#include "Edge.h"
#include <stdlib.h>

Edge *Edge_Create(int startIndex, int endIndex, int nodeIndex)
{
	Edge *edge = (Edge*) malloc(sizeof(Edge));

	edge->startIndex = startIndex;
	edge->endIndex = endIndex;
	edge->nodeIndex = nodeIndex;

	return edge;
}
