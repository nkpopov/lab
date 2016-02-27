
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>

/* Node of r-way trie. Each node contains optional value and links to
 * next 256 nodes which correspond to all possible next characters in
 * the stored string.
 */
template <typename T>
class Node {
    public:

        Node();
        Node(const Node &) = delete;
        Node &operator= (const Node &) = delete;
       ~Node();

        /* Set of child nodes */
        std::vector<Node *> m_next;

        /* Node's value */
        T m_value;

        /* Amount of all possible characters in the trie. Here we
         * assume that we are dealing with extended ASCII alphabet. 
         */
        static const int s_base = 256;
        static const T s_invalid = T();
};

template <typename T>
Node<T>::Node()
    :m_next(std::vector<Node *>(s_base, NULL))
    ,m_value(s_invalid)
{}

template <typename T>
static inline void delete_node(Node<T> *node) {
    delete node;
}

template <typename T>
Node<T>::~Node() {
    std::for_each(m_next.begin(), m_next.end(), delete_node<T>);
}

template <typename T>
class Trie {
    public:
        
        Trie();
        Trie(const Trie &) = delete;
        Trie &operator= (const Trie &) = delete;
       ~Trie();

        /* Insert pair (key, value) into the trie. Procedure will
         * create new node if node with suceh key doesn't exist. 
         * Otherwise it will update value for provided key. 
         */
        void insert(const std::string &key, const T &value);

        /* Get value that corresponds to key in arguments. There are
         * different ways to design signature of this method. Most
         * nice one with const T& return value. But this signature
         * requires std::out_of_range exception thrown in case if no
         * such key in the trie. 
         */
        std::pair<bool, T> get(const std::string &key) const;
        
        /* Check if key is in the trie */
        bool contains(const std::string &key) const;

        /* Erase element with key from the trie */
        void erase(const std::string &key);
        
    private:

        /* Auxilliary method that is used in the insert method
         * implementation. It returns updated version of the node
         * (object in arguments) that is located at the level (depth)
         * of trie in result of insertion pair (key, value). Levels
         * numeration starting from 0 for root and grows down.
         */
        Node<T> *insert_impl(Node<T> *node, const std::string &key,
                const T &value, int depth);

        /* Auxiliary method that is used in erase method
         * implementation. It returns updated version of the node in
         * lower level of trie. NULL value for erased node and not
         * NULL value for other nodes.
         */
        Node<T> *erase_impl(Node<T> *node, const std::string &key,
                int depth);

        /* Auxiliary method that is used in get method implementation.
         * It returns node that corresponds to key in arguments. 
         * Otherwise it returns NULL. 
         */
        const Node<T> *get_impl(const Node<T> *node,
                const std::string &key, int depth) const;

        /* Is node from arguments leaf node? */
        bool leaf_node(const Node<T> *node) const;

        /* Root node of trie */
        Node<T> *m_root;
};

template <typename T>
Trie<T>::Trie()
    :m_root(new Node<T>())
{}

template <typename T>
Trie<T>::~Trie() {
    delete m_root;
}

template <typename T>
void Trie<T>::insert(const std::string &key, const T &value) {
    m_root = insert_impl(m_root, key, value, 0);
}

template <typename T>
Node<T> *Trie<T>::insert_impl(Node<T> *node,
    const std::string &key, const T &value, int depth) { 

    if (node == NULL)
        node = new Node<T>();

    if (depth == key.size()) {
        node->m_value = value;
        return node;
    }

    char c          = key[depth];
    Node<T> *next   = node->m_next[c];
    node->m_next[c] = insert_impl(next, key, value, depth + 1); 
    return node;
}

template <typename T>
bool Trie<T>::contains(const std::string &key) const {
    std::pair<bool, T> value = get(key);
    return value.first;
}

template <typename T>
std::pair<bool, T> Trie<T>::get(const std::string &key) const {
    const Node<T> *node = get_impl(m_root, key, 0);

    if (node == NULL)
        return std::make_pair(false, T());

    const T &value = node->m_value; 
    return std::make_pair(value != Node<T>::s_invalid, value);
}

template <typename T>
const Node<T> *Trie<T>::get_impl(const Node<T> *node,
        const std::string &key, int depth) const {

    if (node == NULL)
        return  NULL;

    if (depth == key.size())
        return node;

    char c = key[depth];
    return get_impl(node->m_next[c], key, depth + 1);
}

template <typename T>
void Trie<T>::erase(const std::string &key) {
    m_root = erase_impl(m_root, key, 0);
}

template <typename T>
Node<T> *Trie<T>::erase_impl(Node<T> *node,
        const std::string &key, int depth) {

    /* If there is no such node which corresponds to key in the trie
     * then there is no node to delete.
     */
    if (node == NULL)
        return  NULL;

    /* We found node corresponding to key. Mark it as intermediate. If
     * it is a leaf node (there are no other keys for which path goes
     * through this node) then it can be erased.
     */
    if (depth == key.size() &&
        node->m_value != Node<T>::s_invalid) {
        node->m_value  = Node<T>::s_invalid;

        if (leaf_node(node)) {
            delete node;
            return NULL;
        }

        return node;
    }

    char c          = key[depth];
    node->m_next[c] = erase_impl(node->m_next[c], key, depth + 1);

    /* Clean up all other leaf intermediate nodes on the way back from
     * erased node to the root.
     */
    if (leaf_node(node) &&
        node->m_value == Node<T>::s_invalid) {
        delete node;
        return NULL;
    }
        
    return node;
}

template <typename T>
bool Trie<T>::leaf_node(const Node<T> *node) const {
    for (const Node<T> *i: node->m_next) {
        if (i != NULL) {
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[]) {

    Trie<int> trie;
    trie.insert("potato",   1);
    trie.insert("pomidoro", 2);
    trie.insert("carrot",   3);

    trie.erase("pomidoro");
    trie.erase("cabbage");
    trie.erase("garlic");

    printf("potato:   %d\n", trie.get("potato").second);
    printf("pomidoro: %d\n", trie.get("pomidoro").second);
    printf("carrot:   %d\n", trie.get("carrot").second);
    printf("tomato:   %d\n", trie.get("tomato").second);
    return 0;
}
    

