#ifndef _CONFIG_H_
#define _CONFIG_H_

// internal representation
typedef struct var_val_pair_str {
    char *var;
    char *val;
    struct var_val_pair_str *next; 
} var_val_pair;

// a structure that holds the configuration file loaded to memory,
// you do not need to access it directly
typedef var_val_pair *config_data;

// read configuration file to memory and parse it
config_data read_config(char *filename);

// retrieve a string value for the specified variable (pointer to a string that will be deallocated in dispose)
char *config_get_strval(config_data config, char *var_name, char *default_value);
// retrieve a string value for the specified variable in a newly allocated string
char *config_get_alloc_strval(config_data config, char *var_name, char *default_value);
// retrieve an int value for the specified variable
int config_get_intval(config_data config, char *var_name, int default_value);
// retrieve a double value for the specified variable
double config_get_doubleval(config_data config, char *var_name, double default_value);

// release config data from the memory
void config_dispose(config_data config);

#endif
