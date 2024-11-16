
/* RENAME maybe - stack.c - shattuck's stack */

/* Sat 16 Nov 13:25:38 UTC 2024 */
/* was: Wed 10 Jul 05:03:10 UTC 2024 */
/* see: C_Lang_snippets-a/stack-s.d */

// ported to pico-sdk, was: #include <Arduino.h>

/* https://github.com/CharleyShattuck/Feather-M0-interpreter/blob/master/Interpreter.ino#L11-L44 */

/* This example code is in the public domain */

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */

/* const int STKSIZE = 8; */ /* b0rked */
#define STKSIZE 8

/* const int STKMASK = 7; */ /* not b0rked but templated the same way */
#define STKMASK 7

int cstack[STKSIZE];
int cstackP = 0;

/* cTOS is Top Of Stack */
/* double define may be an issue here */
#define cTOS cstack[cstackP]

/* push n to top of data stack */
void cpush(int n) { // TODO recursive grep for unique
  cstackP = (cstackP + 1)& STKMASK;
  cTOS = n; // TODO recursive grep for unique
}

/* return top of stack */
int cpop() {
  int n = cTOS;
  cstackP = (cstackP - 1)& STKMASK;
  return n;
}

/* end. */
