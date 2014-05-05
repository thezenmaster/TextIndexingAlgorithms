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
/*The original 95 printable ASCII characters*/
const int alphabetSize = 95;
int lastIndex = 0;

int CheckEdges(Node* n, NodeEdge **nodeEdges, int* check, int offset, int tableLength);
void SetEdges(NodeEdge **nodeEdges, int* check, int* next, int offset, Node* n);
int ComputeCharIndex(char c);

void ConstructTable(NodeEdge **nodeEdges, Node** nodes, int numNodes, Edge** edges, int* tableLength, int* check, int* next)
{
	/*TODO: Check if we can get out of the array bounds.*/
	for (int i = 0; i < (*tableLength); i++)
	{
		check[i] = 0;
	}
	//int freeIndex = 0;

	/*root node is at index 1.*/
	for (int i = 1; i <= numNodes; i++)
	{
		if(nodes[i]->numberofEdges == 0)
			continue;

		//NodeEdge* ne = nodeEdges[nodes[i]->edgeStartIndex];
		while(nodes[i]->baseAddress == -1)
		{
			for (int j = 0; j < (*tableLength); j++)
			{
				if(check[j] != 0 || (CheckEdges(nodes[i], nodeEdges, check, j, *tableLength) == 0))
					continue;
			
				nodes[i]->baseAddress = j;
				break;
			}

			/*We haven't found spot for the node. That means we need to increase the arrays*/
			if(nodes[i]->baseAddress == -1)
			{
				(*tableLength) *= 2;
				check = (int*) realloc(check, (*tableLength)*sizeof(int));

				next = (int*) realloc(next, (*tableLength)*sizeof(int));

				for (int k = (*tableLength); k < (*tableLength); k++)
				{
					check[k] = 0;
				}
			
				//nodes[i]->baseAddress = *tableLength;
			}
		}
		SetEdges(nodeEdges, check, next, nodes[i]->baseAddress, nodes[i]);
	}

	if((lastIndex + 1) < (*tableLength))
	{		
		lastIndex++;
		check= (int*) realloc(check, (lastIndex)*sizeof(int));
		next = (int*) realloc(next, (lastIndex)*sizeof(int));
		(*tableLength) = lastIndex;
	}

	return;
}

int CheckEdges(Node* n, NodeEdge **nodeEdges, int* check, int offset, int tableLength)
{
	int nextElement = n->edgeStartIndex;
	while(nextElement > -1)
	{
		NodeEdge* ne = nodeEdges[nextElement];
		int index = offset + (ComputeCharIndex(ne->symbol));
		if(index >= tableLength || check[index] != 0)
			return 0;
		nextElement = ne->nextElement;
	}

	return 1;
}

void SetEdges(NodeEdge **nodeEdges, int* check, int* next, int offset, Node* n)
{
	int nextElement = n->edgeStartIndex;
	while(nextElement > -1)
	{
		NodeEdge* ne = nodeEdges[nextElement];
		int index = offset + (ComputeCharIndex(ne->symbol));
		check[index] = n->arrayIndex;
		next[index] = ne->edgeIndex;
		nextElement = ne->nextElement;
		if(lastIndex < index)
			lastIndex = index;
	}
}

/*To improve compression assign small indices to frequent characters.*/
int ComputeCharIndex(char c)
{
	int index = (int) c;

	/*Cause an error if we receive a control character or an extended ASCII symbol.*/
	if(index < 32 || index > 126)
		return -1;
	/*A-Z*/
	if(index >= 65 && index <=90)
		index -= 65;//39;
	/*a-z*/
	else if(index >= 97 && index <= 122)
		index -= 71;//97;
	/*Numbers and punctuation marks*/
	else if(index >= 32 && index <= 64)
		index +=20;
	/* [\]^_` */
	else if(index >=91 && index <= 96)
		index -=6;
	else
		/*index >= 123 && index <= 126*/
		/* {|}~ */
		index -=32;

	return index;
}