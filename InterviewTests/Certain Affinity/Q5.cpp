#include <iostream>
#include <conio.h>
#include <string>
#include <assert.h>
#include <map>
   
// Not touching this, assuming is not in my control:
struct Node
{
   long value;
   struct Node *nextNode;
   struct Node *randomNode; // can be NULL, will not be self
   
   // these methods should be used for any allocating & deallocating of Node structures
   static struct Node *allocate(void)
   {
       return (struct Node *)malloc(sizeof(struct Node));
   }
   static void deallocate(struct Node *node)
   {
       assert(node!=NULL);
       free(node);
       return;
   }
};

Node *LinkedListDuplicate(Node *inputList)
{
	std::map< Node * , Node * > oldToNewMap;

	// Traverse link list creating new nodes.
	// Assumes no circular links.
	Node * current = inputList;
	while( current )
	{
		Node * newNode = Node::allocate();
		newNode->value = current->value;
		oldToNewMap[ current ] = newNode;
		current = current->nextNode;
	}

	// At this point we have an exact duplicate of nodes value wise, and mapping to old, now create the heirarchy.
	current = inputList;
	while( current )
	{
		Node * newCurrent = oldToNewMap[ current ];
		Node * newNext = current->nextNode == 0 ? 0 : oldToNewMap[ current->nextNode ];
		Node * newRandom = current->randomNode == 0 ? 0 : oldToNewMap[ current->randomNode ];
		newCurrent->nextNode = newNext;
		newCurrent->randomNode = newRandom;
		current = current->nextNode;
	}

	return oldToNewMap[ current ];
}

void DeleteLinkedList( Node * head )
{
	while( head )
	{
		Node * next = head->nextNode;
		delete head;
		head = next;
	}
}

Node * AddNode( Node * current, long value, Node * randomNode = 0 )
{
	Node * newNode = Node::allocate();
	newNode->value = value;
	newNode->nextNode = 0;
	newNode->randomNode = randomNode;
	if ( current )
	{
		current->nextNode = newNode;
	}
	return newNode;
}

void Q5()
{
	Node * inputList = AddNode( 0, 1 );
	Node * current = inputList;

	current = AddNode( current, 2 );
	Node * monkey = current;

	current = AddNode( current, 3 );
	current = AddNode( current, 4 );
	current = AddNode( current, 5 );
	current = AddNode( current, 5 );
	Node * tail = current;
	monkey->randomNode = tail;

	Node * outputList;
	outputList = LinkedListDuplicate( inputList );

	DeleteLinkedList( outputList );
	outputList = 0;
	DeleteLinkedList( inputList );
	inputList = 0;

	std::cout << std::endl;
}