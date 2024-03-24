void save_to_bin(Node* root, int* count_keys);
void order_trav_bin(Node* ptr, FILE* file);
void read_from_bin(Node** root, int* count_keys);
void save_txt_binopt(Node* root, int* count_keys, int* arrkeys);
int count_repeatkey(Node* root, int key);