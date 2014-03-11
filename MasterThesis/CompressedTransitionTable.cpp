#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Node.h"
#include "NodeEdge.h"
#include "Edge.h"

const int asciiStartIndex = 32;
const int alphabetSize = 100;

int CheckEdges(NodeEdge* ne, NodeEdge **nodeEdges, int* check, int offset, int tableLength);
void SetEdges(NodeEdge **nodeEdges, int* check, int* next, int offset, Node* n, Edge** edges);
int ComputeCharIndex(char c);

void ConstructTable(NodeEdge **nodeEdges, Node** nodes, int numNodes, Edge** edges, int numEdges, int* tableLength, int* check, int* next)
{
	int count = numEdges * alphabetSize;
	for (int i = 0; i < count; i++)
	{
		check[i] = 0;
	}
	*tableLength = count;
	int freeIndex = 0;

	/*root node is at index 1.*/
	for (int i = 1; i < numNodes; i++)
	{
		if(nodes[i]->numberofEdges == 0)
			continue;

		NodeEdge* ne = nodeEdges[nodes[i]->edgeStartIndex];

		for (int j = 0; j < (*tableLength); j++)
		{
			if(check[j] != 0 || (CheckEdges(ne, nodeEdges, check, j, *tableLength) == 0))
				continue;
			
			nodes[i]->baseAddress = j;
			break;
		}

		/*We haven't found spot for the node. That means we need to increase the arrays*/
		if(nodes[i]->baseAddress == -1)
		{
			check = (int*) realloc(check, 2*(*tableLength)*sizeof(int));

			next = (int*) realloc(next, 2*(*tableLength)*sizeof(int));

			for (int j = (*tableLength); j < 2*(*tableLength); j++)
			{
				check[j] = 0;
			}
			
			nodes[i]->baseAddress = *tableLength;
			(*tableLength) *= 2;
		}
		SetEdges(nodeEdges, check, next, nodes[i]->baseAddress, nodes[i], edges);
	}

	return;
}

int CheckEdges(NodeEdge* ne, NodeEdge **nodeEdges, int* check, int offset, int tableLength)
{
	while(ne->nextElement > -1)
	{
		/*don't take the first 32 ascii symbols into account*/
		int index = offset + (ComputeCharIndex(ne->symbol)) - asciiStartIndex;
		if(index >= tableLength || check[index] != 0)
			return 0;
		ne = nodeEdges[ne->nextElement];
	}

	return 1;
}

void SetEdges(NodeEdge **nodeEdges, int* check, int* next, int offset, Node* n, Edge** edges)
{
	NodeEdge* ne = nodeEdges[n->edgeStartIndex];
	while(ne->nextElement > -1)
	{
		int index = offset + (ComputeCharIndex(ne->symbol)) - asciiStartIndex;
		check[index] = n->arrayIndex;
		next[index] = ne->edgeIndex;
		ne = nodeEdges[ne->nextElement];
	}
}

/*To improve compression assign small index to frequent characters.*/
int ComputeCharIndex(char c)
{
	int index = (int) c;

	/*Cause an error if we receive a control character or an extended ASCII symbol.*/
	if(index < 32 || index > 126)
		return -1;

	if(index >= 65 && index <=90)
		index -= 65;
	else if(index >= 97 && index <= 122)
		index -= 71;
	else if(index >= 32 && index <= 64)
		index +=20;
	else if(index >=91 && index <= 96)
		index -=6;
	else
		/*index >= 123 && index <= 126*/
		index -=32;

	return index;
}