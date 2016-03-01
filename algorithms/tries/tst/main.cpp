
#include <stdio.h>
#include <string>

/* Node of TST (Ternary Search Trie). Each node contains links to
 * right and left nodes and integer value. Value -1 is reserved for
 * invalid value.
 */
class Node {
    public:
        Node();
        Node(const Node &) = delete;
        Node &operator= (const Node &) = delete;
       ~Node();
        
        Node *m_lnode;
        Node *m_rnode;
        Node *m_nnode;
        char  m_letter;
        int   m_value;
};

Node::Node()
    :m_lnode(NULL)
    ,m_rnode(NULL)
    ,m_nnode(NULL)
    ,m_letter(0)
    ,m_value(-1)
{}

Node::~Node() {
    delete m_lnode;
    delete m_rnode;
    delete m_nnode;
}

class Tst {
    public:
        Tst();
        Tst(const Tst &) = delete;
        Tst &operator= (const Tst &) = delete;
       ~Tst();

        /* Insert pair (key, value) into the trie. Method will create
         * new node if node with such key doesn't exist, otherwise it
         * will update value for provided key.
         */
        void insert(const std::string &key, int val);

        /* Erase node corresponding to key from the trie. If there is
         * no node for provided key then do nothing.
         */
        void erase(const std::string &key);

        /* Is trie contains node for given key? */
        bool contains(const std::strin &key) const;

        /* Get value of a node for given key. The first element of the
         * returned pair is boolean flag. It has true value if there
         * is node for provided key in the trie. Otherwise it has
         * value false.
         */
        std::pair<bool, int> get(const std::string &key) const;

    private:

        /* Auxilliary procedure that is used in implementation of
         * insert method. It returns pointer to sub-trie (node) at
         * level (depth) with (key, value) pair inserted in it.
         */
        Node *insert_impl(Node *node, const std::string &key,
                int val, int depth);

        /* Root node of the trie */
        Node *m_root;
};

Tst::Tst()
    :m_root(NULL)
{}

Tst::~Tst() {
    delete m_root;
}

void Tst::insert(const std::string &key, int val) {
    m_root = insert_impl(m_root, key, val, 0);
}

Node *Tst::insert_impl(Node *node, const std::string &key,
        int value, int depth) {

    if (node == NULL) {
        node = new Node();
        node->m_letter = key[depth];
    }

    if (depth == key.size()) {
        node->m_value = value;
    } else if (key[depth] == node->m_letter) {
        node->m_nnode = insert_impl(node->m_nnode, key, value, depth + 1);
    } else if (key[depth] < node->m_letter) {
        node->m_lnode = insert_impl(node->m_lnode, key, value, depth);
    } else if (key[depth] > node->m_letter) {
        node->m_rnode = insert_impl(node->m_rnode, key, value, depth);
    }
    
    return node;
}

int main(int argc, char *argv[]) {

    return 0;
}


