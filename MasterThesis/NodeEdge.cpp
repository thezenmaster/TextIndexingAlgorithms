#include "stdafx.h"
#include <stdlib.h>
#include "NodeEdge.h"
#include <assert.h>

NodeEdge *NodeEdge_Create(char c, int edgeIndex)
{
	struct NodeEdge *nodeEdge = (NodeEdge*) malloc(sizeof(NodeEdge));
	assert (nodeEdge != NULL);

	nodeEdge->symbol = c;
	nodeEdge->nextElement = -1;
	nodeEdge->edgeIndex = edgeIndex;
	//nodeEdge->nextNode = 1;

	return nodeEdge;
}