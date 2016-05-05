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
	std::map< const Node * , Node * > oldToNewMap;

	// Traverse link list creating new nodes.
	// Assumes no circular links.
	const Node * current = inputList; // Const to be safer
	while( current )
	{
		Node * newNode = Node::allocate(); // Allocate new node.
		newNode->value = current->value; // Copy value.
		oldToNewMap[ current ] = newNode;
		current = current->nextNode; // Continue to next, duplicating all the way.
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

	return oldToNewMap[ inputList ];
}

void DeleteLinkedList( Node * head )
{
	// Delete head, all the while taking it's next as the new head
	// until we are all out of nodes (head will be 0).
	while( head )
	{
		Node * next = head->nextNode;
		Node::deallocate( head );
		head = next;
	}
}

Node * AddNode( Node * current, long value )
{
	Node * newNode = Node::allocate();
	newNode->value = value;
	newNode->nextNode = 0;
	newNode->randomNode = 0;
	if ( current )
	{
		current->nextNode = newNode;
	}
	return newNode;
}

void PrintList( Node * node )
{
	std::cout << "{ ";
	bool first = true;
	while( node )
	{
		if ( ! first )
		{
			std::cout << ", ";
		}
		else
		{
			first = false;
		}
		std::cout << node->value;
		std::cout << ".rnd:";
		if ( node->randomNode ) 
		{
			std::cout << node->randomNode->value;
		}
		else
		{
			std::cout << "null";
		}
		node = node->nextNode;
	}
	std::cout << "}" << std::endl;
}

void Q5()
{
	Node * inputList = AddNode( 0, 1 );
	Node * current = inputList;

	current = AddNode( current, 2 );
	Node * randomNode1 = current; // Keep a random node.

	current = AddNode( current, 3 );
	current = AddNode( current, 4 );
	current = AddNode( current, 5 );
	current = AddNode( current, 6 );
	Node * randomNode2 = current; // Keep another random node.
	current = AddNode( current, 7 );
	current = AddNode( current, 8 );
	current->randomNode = randomNode1; // Node '8' will randomly point to node '2';
	current = AddNode( current, 9 );
	current = AddNode( current, 10 );
	current->randomNode = randomNode2; // Node '10' will randomly point to node '6'
	current = AddNode( current, 11 );
	current = AddNode( current, 12 ); // No one should care about me except 11 *sigh*.

	Node * outputList;
	outputList = LinkedListDuplicate( inputList );

	// Display list 1...
	std::cout << "List 1: ";
	PrintList( inputList );

	std::cout << "List 2: ";
	PrintList( outputList );

	DeleteLinkedList( outputList );
	outputList = 0;
	DeleteLinkedList( inputList );
	inputList = 0;
}

int main( int argc, char ** argv )
{
	Q5();
	printf( "Press any key to end program.\n" );
	while( !_getch() );
	return 0;
}