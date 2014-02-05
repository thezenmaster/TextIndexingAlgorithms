#ifndef NODE_H
#define NODE_H
struct Node{
	/*Position in the array where we keep track of all nodes*/
	int arrayIndex;
	/*Points at the last character of the substring that leads to this node*/
	int textIndex;
	/*Array that holds the indices of all outgoing edges from this node.
	The size of this array is equal to the number of symbols in the alphabet of the input string. 
	For now, we could initialize it with the number of ASCII symbols.*/
	int *edges;

	/*Helpers, used to initialize *edges in constant time.
	Basically, the following check will be performed before returning an edge:
	if (from[n] < top && to[from[n]] == n)
            return edges[n];*/
	unsigned *from;
	unsigned *to;
	unsigned top;
};
typedef struct Node Node;

Node *Node_Create(int arrayIndex, int textIndex);
#endif