#include "stdafx.h"
#include <stdlib.h>
#include "NodeEdge.h"
#include <assert.h>

NodeEdge *NodeEdge_Create(char c, int nextElement)
{
	struct NodeEdge *nodeEdge = (NodeEdge*) malloc(sizeof(NodeEdge));
	assert (nodeEdge != NULL);

	nodeEdge->c = c;
	nodeEdge->nextElement = nextElement;
	nodeEdge->nextNode = 1;

	return nodeEdge;
}