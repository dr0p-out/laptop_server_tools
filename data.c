/*
 * data.c - Read sensor values
 */

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "foo_.h"

// FIXME: should move hardcode into cfg
#define CAPACITY_PATH "/sys/class/power_supply/BAT0/capacity"

static inline int
readl_ (int fd, char *buff, size_t len, long *out)
{
  ssize_t nul_off;
  char *end;
  long tmp;

  nul_off = read (fd, (void *)buff, len);
  if (nul_off <= (ssize_t)0)
    return -1;

  // may be terminated or not
  if (!buff[nul_off - 1])
    nul_off--;
  else
    buff[nul_off] = '\0';

  tmp = strtol (buff, &end, 10);
  if (end == (buff + nul_off))
  {
ok:
    *out = tmp;
    return 0;
  }

  // allow trailing newline as well
  if (end == (buff + nul_off - 1) &&
      *end == '\n')
    goto ok;

  return -1;
}

static inline void
auto_clo_ (int *res)
{
  int fd;
  fd = *res;
  if (fd < 0)
    return;
  close (fd);
}

#define AUTO_CLO_ __attribute__((__cleanup__(auto_clo_)))

int
get_bat_lvl_ (unsigned char *out)
{
  AUTO_CLO_ int fd;
  char buff[6];  // 0~100, NUL and newline, plus one more to be fool-proof
  long percentage;

  fd = open (CAPACITY_PATH, O_RDONLY);
  if (fd < 0)
    return -1;

  if (readl_ (fd, buff, sizeof (buff) - 1, &percentage) < 0)
    return -1;

  // sanity chk
  if (percentage < 0 || percentage > 100)
    return -1;

  *out = (unsigned char)percentage;
  return 0;
}
