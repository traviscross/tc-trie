#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "trie.h"
#include "cJSON.h"

cJSON* parse_file(char *file) {
  char *buf;
  struct stat s;
  int fd = open(file, O_RDONLY);
  if (fd < 0) return NULL;
  fstat(fd, &s);
  buf = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  if (buf == MAP_FAILED) return NULL;
  cJSON *root = cJSON_Parse(buf);
  if (munmap(buf, s.st_size) < 0) abort();
  return root;
}

struct bit_trie_node *load_file(char *file) {
  cJSON *root = parse_file(file);
  if (!root) return NULL;
  struct bit_trie_node *node = bit_trie_create();
  cJSON *item = root->child;
  while(item) {
    if (item->string && item->valuestring) {
      bit_trie_set(node, (unsigned char*)item->string, strlen(item->string),
                   strdup(item->valuestring), strlen(item->valuestring)+1);
    }
    item = item->next;
  }
  cJSON_Delete(root);
  return node;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: %s <data.json> <string>\n", argv[0]);
  }
  char *file = argv[1];
  char *target = argv[2];
  struct bit_trie_node *res = NULL;
  struct bit_trie_node *node = load_file(file);
  uint32_t len = bit_trie_get(&res, node, (unsigned char*)target, strlen(target));
  if (len > 0 && res && res->value) {
    printf("Result: %s, Len: %d\n", (unsigned char*)res->value, len);
  } else if (len > 0 && res) {
    printf("Matched %d chars of key but no leaf value\n", len);
  }
  printf("Trie is %d bytes\n", bit_trie_byte_size(node));
  printf("Freed %d nodes\n", bit_trie_free(node));
  return 0;
}
