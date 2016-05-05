#include <iostream>
#include <conio.h>
#include <string>
#include <assert.h>
#include <algorithm>

void Reverse( char * first, char * last )
{
	while( first < last )
	{
		std::swap( *first, *last );
		++first;
		--last;
	}
}

void ReverseWords(char *sentence)
{
	if ( ! sentence )
	{
		return;
	}

	size_t length = strlen( sentence );
	Reverse( sentence, sentence + length - 1 );

	size_t l = 0;
	size_t r = 1;
	while( r < length )
	{
		while( sentence[ r ] != ' ' && sentence[ r ] != 0 )
		{
			char s = sentence[ r ];
			++r;
		}
		Reverse( sentence + l, sentence + r - 1 );
		l = ++r;
	}
}

void Q6()
{
	char sentence[]= "My name is Chris";
	printf("[\n");
	printf("%s\n", sentence);
	ReverseWords(sentence);
	printf("%s\n", sentence);
	printf("]\n");

	char sentence2[]= "";
	printf("[\n");
	printf("%s\n", sentence2);
	ReverseWords(sentence2);
	printf("%s\n", sentence2);
	printf("]\n");

	char sentence3[]= "I";
	printf("[\n");
	printf("%s\n", sentence3);
	ReverseWords(sentence3);
	printf("%s\n", sentence3);
	printf("]\n");

	ReverseWords(0);

	std::cout << std::endl;
}
