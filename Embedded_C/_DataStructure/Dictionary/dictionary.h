/* https://gist.github.com/kylef/86784/fe97567ec9baf5c0dce3c7fcbec948e21dfcce09 */

#ifndef __DICT_H__
#define __DICT_H__

typedef struct dict_t_struct {
    char *key;
    void *value;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(void);
void dictDealloc(dict_t **dict);
void *getItem(dict_t *dict, char *key);
size_t getSize(dict_t *dict);
void delItem(dict_t **dict, char *key);
void addItem(dict_t **dict, char *key, void *value);	/* add at front */

#endif
