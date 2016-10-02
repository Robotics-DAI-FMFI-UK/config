#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include "config.h"

static var_val_pair *first;
static unsigned char parse_in_progress = 0;  // should be atomic_flag, needs gcc 4.9 :(

char *config_get_strval(config_data config, char *var_name)
{
  var_val_pair *browse = config;
  while (browse)
  {
     if (strcmp(var_name, browse->var) == 0)
       return browse->val;
     browse = browse->next;
  }
  return 0;
}

int config_get_intval(config_data config, char *var_name)
{
  int x = 0;
  char *val = config_get_strval(config, var_name);
  if (val != 0) sscanf(val, "%d", &x);
  return x;
}

double config_get_doubleval(config_data config, char *var_name)
{
  double x = 0;
  char *val = config_get_strval(config, var_name);
  if (val != 0) sscanf(val, "%lf", &x);
  return x;
}

void config_dispose(config_data config)
{
  var_val_pair *browse = config;
  while (config)
  {
    browse = config->next;
    free(config);
    config = browse;
  }
}

void add_var_val_pair(char *var_name, int var_len, char *val_start, char *val_last)
{
   var_val_pair *inserted = (var_val_pair *)malloc(sizeof(var_val_pair));
   inserted->var = (char *)malloc(var_len + 1);
   strncpy(inserted->var, var_name, var_len);
   *(inserted->var + var_len) = 0;
   int val_len = val_last - val_start + 1;
   inserted->val = (char *)malloc(val_len + 1);
   strncpy(inserted->val, val_start, val_len);
   *(inserted->val + val_len) = 0;
   inserted->next = first;
   first = inserted;
}

#define EXPECT_LINE_START 0
#define LOADS_VARIABLE 1
#define EXPECT_COLON 2
#define EXPECT_VALUE 3
#define LOADS_VALUE 4
#define EXPECT_LINE_END 5

void parse_char(char **ch, unsigned char *state)
{
  static int ln = 1;
  static char *var_start, *val_start, *val_last;
  static int var_len;

  if (**ch == '\n') ln++;
 
  switch (*state) {
  case EXPECT_LINE_START:
        if (**ch == '#')
          *state = EXPECT_LINE_END;
	else if (!isspace(**ch))
        {
          *state = LOADS_VARIABLE;
          var_start = *ch;
          var_len = 1;
        }
        break;

  case LOADS_VARIABLE:
        if (**ch == '\n') printf("unexpected end of line at ln. %d\n", ln - 1);
        else if (isspace(**ch)) 
          *state = EXPECT_COLON;
        else if (**ch == ':')
	  *state = EXPECT_VALUE;
        else var_len++;
        break;

  case EXPECT_COLON:
        if (**ch == '\n') printf("unexpected end of line at ln. %d\n", ln - 1);
	if (**ch == ':') *state = EXPECT_VALUE;
        break;

  case EXPECT_VALUE:
        if (!isspace(**ch))
        {
          val_start = *ch;
          val_last = *ch;
          *state = LOADS_VALUE;
        }
        break;

  case LOADS_VALUE:
	if ((**ch == '\n')  || (**ch == '#'))
        {
           add_var_val_pair(var_start, var_len, val_start, val_last);
           *state = (**ch == '\n') ? EXPECT_LINE_START : EXPECT_LINE_END;
        } 
        else if (!isspace(**ch)) val_last = *ch;
        break;

  case EXPECT_LINE_END:
        if (**ch == '\n') *state = EXPECT_LINE_START;
        break;
  }
  (*ch)++;
}

void parse_config(char *buf, long config_size)
{
    char *stop_at_char = buf + config_size;
    unsigned char state = EXPECT_LINE_START;
    while (buf < stop_at_char)
        parse_char(&buf, &state); 
}

config_data read_config(char *filename)
{
    while (parse_in_progress == 1)   //should be atomic test and set flag
        usleep(100000);
    parse_in_progress = 1;
    do {
        first = 0;
        struct stat sb;
        if (stat(filename, &sb) == -1) break;
      
        long config_size = (long)sb.st_size;
        char *config_buf = (char *)malloc(config_size + 1);
        if (config_buf == 0) break;
    
        do {
      
            int f = open(filename, O_RDONLY);
            if (f < 0) break;
            int nread = read(f, config_buf, config_size);
            if (nread < config_size)
            {
              close(f);
              break;
            }
            close(f);
	    *(config_buf + config_size) = '\n';
            parse_config(config_buf, config_size + 1);
    
        } while(0);
        free(config_buf);
    } while (0);
    config_data retval = first;
    parse_in_progress = 0;  //atomic_clear_flag
    return retval;
}


