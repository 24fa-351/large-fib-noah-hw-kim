#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

// enum vals (success, failure}
enum State
{
   SUCCESS = 1,
   FAILURE = 0
};

// Monad
typedef struct
{
   unsigned long long num;
   enum State state;
} ReturnVal;
int sumWouldOverflow(unsigned long long, unsigned long long);
ReturnVal fib_i(int);
ReturnVal fib_r(int);

ReturnVal fib_i(int seq)
{
   // base
   if (seq == 1)
   {
      return (ReturnVal){0, SUCCESS};
   }
   else if (seq == 2)
   {
      return (ReturnVal){1, SUCCESS};
   }

   unsigned long long int prev = 0;
   unsigned long long int cur = 1;

   // iterate starts from 3rd fib since we already have 1st and 2nd fib
   for (int idx = 2; idx < seq; idx++)
   {
      if (sumWouldOverflow(prev, cur))
      {
         printf("overflow occured at %d\n", seq);
         return (ReturnVal){0, FAILURE};
      }
      unsigned long long int nxt = prev + cur;
      prev = cur;
      cur = nxt;
   }

   return (ReturnVal){cur, SUCCESS};
}

ReturnVal fib_r(int seq)
{
   // base case
   if (seq == 1)
   {
      return (ReturnVal){0, SUCCESS};
   }
   else if (seq == 2)
   {
      return (ReturnVal){1, SUCCESS};
   }

   ReturnVal prev = fib_r(seq - 1);
   if (prev.state == FAILURE)
   {
      return prev;
   }
   ReturnVal prevprev = fib_r(seq - 2);
   if (prevprev.state == FAILURE)
   {
      return prevprev;
   }

   if (sumWouldOverflow(prev.num, prevprev.num))
   {
      printf("overflow occured at %d\n", seq);
      return (ReturnVal){0, FAILURE};
   }

   return (ReturnVal){prev.num + prevprev.num, SUCCESS};
}

int sumWouldOverflow(unsigned long long int num1, unsigned long long int num2)
{
   return num2 > ULLONG_MAX - num1;
}

int main(int argc, char *argv[])
{
   int seq;
   sscanf(argv[1], "%d", &seq);
   char *method = argv[2];

   ReturnVal result;

   if (*method == 'r')
   {
      result = fib_r(seq);
   }
   else if (*method == 'i')
   {
      result = fib_i(seq);
   }

   // nth number starts from 1 not 0
   if (seq < 1)
   {
      printf("fibonacci index should be positive");
   }
   else
   {
      if (result.state)
      {
         printf("%llu", result.num);
      }
   }

   return 0;
}
