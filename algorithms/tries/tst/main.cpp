
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
        int   m_value;
};

Node::Node()
    :m_lnode(NULL)
    ,m_rnode(NULL)
    ,m_value(-1)
{}

Node::~Node() {
    delete m_lnode;
    delete m_rnode;
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

        /* Root node of the trie */
        Node *m_root;
};

int main(int argc, char *argv[]) {

    return 0;
}


