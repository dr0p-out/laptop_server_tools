/*
 * daemon.c - Monitor on low-battery
 */

#include <stdio.h>
#include <unistd.h>

#include "foo_.h"

// FIXME: make these things changable
#define DELAY 30
#define CRITICAL 50

static inline void
do_once_chk_ (void)
{
  unsigned char curr;
  static unsigned char last = 0xff;

  if (get_bat_lvl_ (&curr) < 0)
  {
    fprintf (stderr, "Cannot read status\n");
    return;
  }

  // dont logspam ourselves
  if (curr == last)
    return;

  last = curr;
  printf ("Charge: %hhu%%\n", curr);

  if (curr <= CRITICAL)
    shut_down_ ();  // bedtime
}

__attribute__((__noreturn__)) static inline void
forever_loop_ (void)
{
  for (; ; )
  {
    do_once_chk_ ();
    sleep (DELAY);
  }
}

int
main ()
{
  forever_loop_ ();
}
