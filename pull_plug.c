/*
 * pull_plug.c - Turn system off
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "foo_.h"

#define err_(s_, ...) fprintf (stderr, s_ "\n", ##__VA_ARGS__)

static inline void
chk_prog_ret_ (int wstatus)
{
  int ex;
  if ((ex = WEXITSTATUS(wstatus)))
    err_ ("Command ended with code %d", ex);
}

static inline void
wait_child_ (pid_t pid)
{
  int wstatus;
  int sig;

  if (waitpid (pid, &wstatus, 0) < 0)
  {
    perror ("waitpid");
    return;
  }

  if (WIFEXITED(wstatus))
    chk_prog_ret_ (wstatus);
  else if (WIFSIGNALED(wstatus))
  {
    sig = WTERMSIG(wstatus);
    err_ ("Killed by %s", strsignal (sig));
  }
  else
    err_ ("Something went wrong");
}

__attribute__((__noreturn__)) static inline void
do_stuff_and_die_ (void)
{
  // XXX: or allow running a custom script?
  if (execv ("/usr/bin/poweroff", (char *[]){"poweroff", NULL}) < 0)
    perror ("execv");
  _exit (1);
}

void
shut_down_ (void)
{
  pid_t pid;

  pid = fork ();
  if (pid < 0)
  {
    perror ("fork");
    return;
  }

  if (pid > 0)
    wait_child_ (pid);  // parent
  else
    do_stuff_and_die_ ();  // subproc
}
