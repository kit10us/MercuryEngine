#include <iostream>
#include <conio.h>
#include <string>
#include <assert.h>

long MultiplyBySeven(long input)
{
	return (input << 3) - input;
}

void Q4()
{
   long oneTimesSeven = MultiplyBySeven(1);
   assert(oneTimesSeven==7);
   long twoTimesSeven = MultiplyBySeven(2);
   assert(twoTimesSeven==14);
   long threeTimesSeven = MultiplyBySeven(3);
   assert(threeTimesSeven==21);
   long fourTimesSeven = MultiplyBySeven(4);
   assert(fourTimesSeven==28);
   std::cout << "Complete!" << std::endl;
   std::cout << std::endl;
}
