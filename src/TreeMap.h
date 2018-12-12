#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi {

template <typename KeyType, typename ValueType>
class TreeMap {
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:
	struct  Node {
		Node* up;
		Node *left, *right;
		value_type *value;
		int bf;
	};
	size_type SIZE;
	Node *root;
	//usuwanie poddrzew podanego wierzcholka i tego wierzcholka, dla node==root zostanie usuniete cale drzewo 
	void Clear(Node *node) {
		if (node == nullptr) return;
		Clear(node->left);
		Clear(node->right);

		delete node->value;
		delete node;
	}
	//ROTACJE RR, LL, RL, LR
	void RR(Node * A) {
		Node * B = A->right, *p = A->up;

		A->right = B->left;
		if (A->right) A->right->up = A;

		B->left = A;
		B->up = p;
		A->up = B;

		if (p) {
			if (p->left == A) p->left = B;
			else p->right = B;
		}
		else root = B;

		if (B->bf == -1) A->bf = B->bf = 0;
		else {
			A->bf = -1;
			B->bf = 1;
		}
	}
	void LL(Node * A) {
		Node * B = A->left, *p = A->up;

		A->left = B->right;
		if (A->left) A->left->up = A;

		B->right = A;
		B->up = p;
		A->up = B;

		if (p) {
			if (p->left == A) p->left = B;
			else p->right = B;
		}
		else root = B;

		if (B->bf == 1) A->bf = B->bf = 0;
		else {
			A->bf = 1;
			B->bf = -1;
		}
	}
	void RL(Node * A) {
		Node * B = A->right, *C = B->left, *p = A->up;

		B->left = C->right;
		if (B->left) B->left->up = B;

		A->right = C->left;
		if (A->right) A->right->up = A;

		C->left = A;
		C->right = B;
		A->up = B->up = C;
		C->up = p;

		if (p) {
			if (p->left == A) p->left = C;
			else p->right = C;
		}
		else root = C;

		if (C->bf == -1) A->bf = 1;
		else A->bf = 0;
		if (C->bf == 1) B->bf = -1;
		else B->bf = 0;

		C->bf = 0;
	}
	void LR(Node * A) {
		Node * B = A->left, *C = B->right, *p = A->up;

		B->right = C->left;
		if (B->right) B->right->up = B;

		A->left = C->right;
		if (A->left) A->left->up = A;

		C->right = A;
		C->left = B;
		A->up = B->up = C;
		C->up = p;

		if (p) {
			if (p->left == A) p->left = C;
			else p->right = C;
		}
		else root = C;

		if (C->bf == 1) A->bf = -1;
		else A->bf = 0;
		if (C->bf == -1) B->bf = 1;
		else B->bf = 0;

		C->bf = 0;
	}
	///KONIEC rotacji
	Node* treeSearch(Node *node, key_type &key)const {
		while (node != nullptr && key != node->value->first) {
			if (key < node->value->first)
				node = node->left;
			else
				node = node->right;
		 }
		return node;
	}
	Node* findMin(Node* t)const {
		Node* tmp = t;
		while (tmp->left != nullptr)
			tmp = tmp->left;
		return tmp;
	}
	Node* findMax(Node*t)const {
		Node* tmp = t;
		while (tmp->right != nullptr)
			tmp = tmp->right;
		return tmp;
	}
	Node* prevNode(Node *node)const  {
		/*Node * r;
		if (node) {
			if (node->left) {
				node = node->left;
				while (node->right) node = node->right;
			}
			else
				do {
					r = node;
					node = node->up;
				} while (node && node->right != r);
		}
		return node;*/
		if (node->left != nullptr) {
			node = findMax(node->left);
			return node;
		}
		Node* tmp = node->up;
		while (tmp != nullptr && tmp->right != node) {
			node = tmp;
			tmp = tmp->up;
		}
		node = tmp;
		return node;
	}
	Node* nextNode(Node *node)const {
		if (node->right != nullptr) {
			node = findMin(node->right);
			return node;
		}
		Node* tmp = node->up;
		while (tmp != nullptr && tmp->left != node) {
			node = tmp;
			tmp = tmp->up;
		}
		node = tmp;
		return node;
	}
	//wyjmuje wezel z drzewa i go zwraca, zwalaniac trzeba osobno
	Node* removeNode(Node * node) {
		Node  *t, *y, *z;
		bool nest;

		if (node->left && node->right) {
			y = removeNode(root, prevNode(node));
			nest = false;
		}
		else {
			if (node->left) {
				y = node->left;
				node->left = nullptr;
			}
			else {
				y = node->right;
				node->right = nullptr;
			}
			node->bf = 0;
			nest = true;
		}

		if (y) {
			y->up = node->up;
			y->left = node->left;
			if (y->left)  y->left->up = y;
			y->right = node->right;
			if (y->right)  y->right->up = y;
			y->bf = node->bf;
		}

		if (node->up) {
			if (node->up->left == node) node->up->left = y;
			else node->up->right = y;
		}
		else root = y;

		if (nest) {
			z = y;
			y = node->up;
			while (y) {
				if (!y->bf) {
					// Przypadek nr 1
					if (y->left == z)  y->bf = -1;
					else y->bf = 1;
					break;
				}
				else {
					if (((y->bf == 1) && (y->left == z)) || ((y->bf == -1) && (y->right == z))) {
						// Przypadek nr 2
						y->bf = 0;
						z = y;
						y = y->up;
					}
					else {
						if (y->left == z)  t = y->right;
						else t = y->left;
						if (!t->bf) {
							// Przypadek 3A
							if (y->bf == 1) LL(y);
							else RR(y);
							break;
						}
						else if (y->bf == t->bf) {
							// Przypadek 3B
							if (y->bf == 1) LL(y);
							else RR(y);
							z = t;
							y = t->up;
						}
						else {
							// Przypadek 3C
							if (y->bf == 1) LR(y);
							else RL(y);
							z = y->up;
							y = z->up;
						}
					}
				}
			}
		}
		return node;
	}
	

public:
    TreeMap() {
		root = nullptr;
		SIZE = 0;
    }
	~TreeMap() {
		Clear(root);
		SIZE = 0;
	}

    TreeMap(std::initializer_list<value_type> list) {
		for (auto it = list.begin(); it < list.end(); it++)	{
			this->operator[](it->first) = it->second;
		}
    }

    TreeMap(const TreeMap& other) {
		for (auto it = other.begin(); it != other.end(); it++) {
			this->operator[](it->first) = it->second;
		}
    }

    TreeMap(TreeMap&& other) {
		*this = std::move(other);
    }

    TreeMap& operator=(const TreeMap& other) {
		if (this == other)
			return *this;

		Clear(root);
		SIZE = 0;
		root = nullptr;
		for (auto it = other.begin(); it != other.end(); it++) {
			this->operator[](it->first) = it->second;
		}
		return *this;
    }

    TreeMap& operator=(TreeMap&& other) {
		Clear(root);
		root = nullptr;
		SIZE = 0;

		std::swap(root, other.root);
		std::swap(SIZE, other.SIZE);
		return *this;
    }

    bool isEmpty() const {
		if (SIZE == 0) return true;
		else return false;
    }
	
    mapped_type& operator[](const key_type& key) {
        (void)key;
        throw std::runtime_error("TODO");
    }

    const mapped_type& valueOf(const key_type& key) const {
		Node *tmp = root;
		tmp = treeSearch(tmp, key);
		if (tmp == nullptr)
			throw std::out_of_range("Nie ma elementu o podanym kluczu w mapie");
		else
			return tmp->value->second;
    }

    mapped_type& valueOf(const key_type& key) {
		Node *tmp = root;
		tmp = treeSearch(tmp, key);
		if (tmp == nullptr)
			throw std::out_of_range("Nie ma elementu o podanym kluczu w mapie");
		else
			return tmp->value->second;
    }

    const_iterator find(const key_type& key) const {
		ConstIterator it;
		it.tree = this;
		it.node = treeSearch(root, key);
		return it;
    }

    iterator find(const key_type& key) {
		Iterator it;
		it.tree = this;
		it.node = treeSearch(root, key);
		return it;
    }

    void remove(const key_type& key) {
		Node *tmp = treeSearch(root, key);
		if (tmp == nullptr)
			throw std::out_of_range("Nie ma takiego elemntu do usuniecia");
		removeNode(tmp);
		delete tmp->value;
		delete tmp;
    }

    void remove(const const_iterator& it) {
		Node *tmp = it.node;
		if (tmp == nullptr)
			throw std::out_of_range("Nie ma takiego elemntu do usuniecia");
		removeNode(tmp);
		delete tmp->value;
		delete tmp;
    }

    size_type getSize() const {
		return SIZE;
    }
	//Prawdopodonie do poprawy
    bool operator==(const TreeMap& other) const {
		if (SIZE != other.SIZE)
			return false;
		auto itOther = other.begin();
		for (auto it = begin(); it != end(); ++it, ++itOther) {
			if (*it != *itOther)
				return false;
		}
		return true;
    }

    bool operator!=(const TreeMap& other) const {
        return !(*this == other);
    }

    iterator begin() {
		Iterator it;
		it.tree = this;
		Node *node = root;
		while (node != nullptr && node->left != nullptr)
			node = node->left;

		it.node = node;
		return it;
    }

    iterator end() {
		/*Iterator it;
		it.tree = this;
		it.node = nullptr;
		return it;*/
		return Iterator(this, nullptr);
    }

    const_iterator cbegin() const {
		ConstIterator it;
		it.tree = this;
		Node *node = root;
		while (node != nullptr && node->left != nullptr)
			node = node->left;

		it.node = node;
		return it;
    }

    const_iterator cend() const {
		/*ConstIterator it;
		it.tree = this;
		it.node = nullptr;
		return it;*/
		return ConstIterator(this, nullptr);
    }

    const_iterator begin() const {
		return cbegin();
    }

    const_iterator end() const {
        return cend();
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename TreeMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TreeMap::value_type;
    using pointer = const typename TreeMap::value_type*;

	Node *node;
	const TreeMap *tree;

    explicit ConstIterator(TreeMap *t, Node *n):tree(t), node(n) {
    }

    ConstIterator(const ConstIterator& other) {
		node = other->node;
		tree = other->tree;
    }

    ConstIterator& operator++() { //PREinkrementacja
		if (node == nullptr)
			throw std::out_of_range("Iterator null");
		node = tree->nextNode(node);
		return *this;
    }

    ConstIterator operator++(int) { //POSTinkrementacja
		if (node == nullptr)
			throw std::out_of_range("Iterator null");

		ConstIterator it;
		it.tree = tree;
		it.node = node;
		node = tree->nextNode(node);
		return it;
    }

    ConstIterator& operator--() { //PREdekrementacja
		if (node == nullptr)
			throw std::out_of_range("Iterator null");
		node = tree->prevNode(node);
		return *this;
    }

    ConstIterator operator--(int) { //POSTdekrementacja
		if (node == nullptr)
			throw std::out_of_range("Iterator null");

		ConstIterator it;
		it.tree = tree;
		it.node = node;
		node = tree->prevNode(node);
		return it;
    }

    reference operator*() const {
		if (node == nullptr)
			throw std::out_of_range("Iterator null");
		return node->value;
    }

    pointer operator->() const {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const { //poprawic
		if (tree == other->tree && node == other->node)
			return true;
		else return false;
    }

    bool operator!=(const ConstIterator& other) const {
        return !(*this == other);
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename TreeMap::reference;
    using pointer = typename TreeMap::value_type*;

    explicit Iterator() {
    }

    Iterator(const ConstIterator& other)
        : ConstIterator(other) {
    }

    Iterator& operator++() {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int) {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--() {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int) {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const {
        return &this->operator*();
    }

    reference operator*() const {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif /* AISDI_MAPS_MAP_H */
