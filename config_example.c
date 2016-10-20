#include <stdio.h>

#include "config.h"

int main()
{
  config_data cfg = read_config("example.cfg");
  printf("abc=%s\n", config_get_strval(cfg, "abc", "xx"));
  printf("def=%s\n", config_get_strval(cfg, "def", "xx"));
  printf("ghi=%d\n", config_get_intval(cfg, "ghi", 777));
  printf("jkl=%d\n", config_get_intval(cfg, "jkl", 999));
  printf("mno=%f\n", config_get_doubleval(cfg, "mno", 777.999));
  printf("pqr=%f\n", config_get_doubleval(cfg, "pqr", 999.777));
  printf("this_is_one=%d\n", config_get_intval(cfg, "this_is_one", 999));
  printf("this_is_zero=%d\n", config_get_intval(cfg, "this_is_zero", 999));

  config_dispose(cfg);
  return 0;
}

