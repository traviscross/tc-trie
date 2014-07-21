struct bit_trie_node {
  struct bit_trie_node *next[2];
  void *value;
};

struct bit_trie_node* bit_trie_create();
uint32_t bit_trie_get(struct bit_trie_node **node_out,
                      struct bit_trie_node *node,
                      unsigned char *key,
                      uint32_t key_len);
struct bit_trie_node* bit_trie_set(struct bit_trie_node *node,
                                   unsigned char *key,
                                   uint32_t key_len,
                                   void *value);
void bit_trie_free(struct bit_trie_node *node);
