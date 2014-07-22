#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "trie.h"

int main(int argc, char **argv) {
  struct bit_trie_node *node = bit_trie_create();
  struct bit_trie_node *res = NULL;
  bit_trie_set(node, (unsigned char*)"01234", 5, strdup("test-01234"), 11);
  bit_trie_set(node, (unsigned char*)"012345", 6, strdup("test-012345"), 12);
  bit_trie_set(node, (unsigned char*)"012346", 6, strdup("test-012346"), 12);
  bit_trie_set(node, (unsigned char*)"01243", 5, strdup("test-01243"), 11);
  uint32_t len = bit_trie_get(&res, node, (unsigned char*)"01234", 5);
  if (len > 0 && res && res->value) {
    printf("Result: %s, Len: %d\n", (unsigned char*)res->value, len);
  } else if (len > 0 && res) {
    printf("Matched %d chars of key but no leaf value\n", len);
  }
  printf("Trie is %d bytes\n", bit_trie_byte_size(node));
  printf("Freed %d nodes\n", bit_trie_free(node));
  return 0;
}
