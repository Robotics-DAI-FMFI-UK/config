#include <stdio.h>

#include "config.h"

int main()
{
  config_data cfg = read_config("example.cfg");
  printf("abc=%s\n", config_get_strval(cfg, "abc"));
  printf("def=%s\n", config_get_strval(cfg, "def"));
  printf("ghi=%d\n", config_get_intval(cfg, "ghi"));
  printf("jkl=%d\n", config_get_intval(cfg, "jkl"));
  printf("mno=%f\n", config_get_doubleval(cfg, "mno"));
  printf("pqr=%f\n", config_get_doubleval(cfg, "pqr"));

  config_dispose(cfg);
  return 0;
}

