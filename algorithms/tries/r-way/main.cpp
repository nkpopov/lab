
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>

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
        
        std::vector<Node *> m_next;
        T m_value;

        /* Amount of all possible characters in the trie. Here we
         * assume that we are dealing with extended ASCII alphabet. 
         */
        static const int s_base = 256;
};

template <typename T>
Node<T>::Node()
    :m_next(std::vector<Node *>(s_base, NULL))
    ,m_value(T())
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
        
    private:

        /* Auxilliary method that is used in the insert method
         * implementation. It returns updated version of the node
         * (object in arguments) that is located at the level (depth)
         * of trie in result of insertion pair (key, value). Levels
         * numeration starting from 0 for root and grows down.
         */
        Node<T> *insert_impl(Node<T> *node, const std::string &key,
                const T &value, int depth); 

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
Node<T> *Trie<T>::insert_impl(Node<T> *node, const std::string &key,
        const T &value, int depth) { 

    if (node == NULL) {
        node = new Node<T>();
    }

    if (depth == key.size()) {
        node.m_value = value;
        return node;
    };

    char c = key[depth];
    node.m_next[c] = insert_impl(node.m_next[c], key,
        value, depth + 1); 
    return node;
}

int main(int argc, char *argv[]) {
    Trie<int> trie;
    return 0;
}
    

