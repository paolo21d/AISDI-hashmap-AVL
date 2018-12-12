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

	void Clear(Node *node) {
		if (node == nullptr) return;
		Clear(node->left);
		Clear(node->right);

		delete node->value;
		delete node;
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
	//Implementacja rotacji: RR, LL, RL, LR

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
	////KONIEC rotacji
    mapped_type& operator[](const key_type& key) {
        (void)key;
        throw std::runtime_error("TODO");
    }

    const mapped_type& valueOf(const key_type& key) const {
        (void)key;
        throw std::runtime_error("TODO");
    }

    mapped_type& valueOf(const key_type& key) {
        (void)key;
        throw std::runtime_error("TODO");
    }

    const_iterator find(const key_type& key) const {
        (void)key;
        throw std::runtime_error("TODO");
    }

    iterator find(const key_type& key) {
        (void)key;
        throw std::runtime_error("TODO");
    }

    void remove(const key_type& key) {
        (void)key;
        throw std::runtime_error("TODO");
    }

    void remove(const const_iterator& it) {
        (void)it;
        throw std::runtime_error("TODO");
    }

    size_type getSize() const {
        throw std::runtime_error("TODO");
    }

    bool operator==(const TreeMap& other) const {
        (void)other;
        throw std::runtime_error("TODO");
    }

    bool operator!=(const TreeMap& other) const {
        return !(*this == other);
    }

    iterator begin() {
        throw std::runtime_error("TODO");
    }

    iterator end() {
        throw std::runtime_error("TODO");
    }

    const_iterator cbegin() const {
        throw std::runtime_error("TODO");
    }

    const_iterator cend() const {
        throw std::runtime_error("TODO");
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

    explicit ConstIterator() {
    }

    ConstIterator(const ConstIterator& other) {
        (void)other;
        throw std::runtime_error("TODO");
    }

    ConstIterator& operator++() {
        throw std::runtime_error("TODO");
    }

    ConstIterator operator++(int) {
        throw std::runtime_error("TODO");
    }

    ConstIterator& operator--() {
        throw std::runtime_error("TODO");
    }

    ConstIterator operator--(int) {
        throw std::runtime_error("TODO");
    }

    reference operator*() const {
        throw std::runtime_error("TODO");
    }

    pointer operator->() const {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const {
        (void)other;
        throw std::runtime_error("TODO");
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
