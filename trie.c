#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "trie.h"

struct bit_trie_node* bit_trie_create() {
  struct bit_trie_node *node = malloc(sizeof(struct bit_trie_node));
  if (!node) abort();
  memset(node, 0, sizeof(struct bit_trie_node));
  return node;
}

uint32_t bit_trie_get(struct bit_trie_node **node_out,
                      struct bit_trie_node *node,
                      unsigned char *key,
                      uint32_t key_len) {
  unsigned char *keyp = key;
  while(keyp < key + key_len) {
    uint8_t offset = 0;
    while(offset < 8) {
      uint8_t bit = (*keyp >> offset) & 0x01;
      struct bit_trie_node *next = node->next[bit];
      if (next) {
        node = next;
      } else {
        *node_out = node;
        return keyp - key;
      }
      offset++;
    }
    keyp++;
  }
  *node_out = node;
  return keyp - key;
}

struct bit_trie_node* bit_trie_set(struct bit_trie_node *node,
                                   unsigned char *key,
                                   uint32_t key_len,
                                   void *value) {
  unsigned char *keyp = key;
  while(keyp < key + key_len) {
    uint8_t offset = 0;
    while(offset < 8) {
      uint8_t bit = (*keyp >> offset) & 0x01;
      struct bit_trie_node *next = node->next[bit];
      if (next) {
        node = next;
      } else {
        next = bit_trie_create();
        node->next[bit] = next;
        node = next;
      }
      offset++;
    }
    keyp++;
  }
  node->value = value;
  return node;
}

static void bit_trie_free_r(struct bit_trie_node *node) {
  if (!node) return;
  bit_trie_free_r(node->next[0]);
  bit_trie_free_r(node->next[1]);
  free(node->value);
  free(node);
}

void bit_trie_free(struct bit_trie_node *node) {
  return bit_trie_free_r(node);
}
