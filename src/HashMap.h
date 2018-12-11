#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <stack>

namespace aisdi {

template <typename KeyType, typename ValueType>
class HashMap {
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Pair = std::pair<key_type, mapped_type>;
    using Vector = std::vector<Pair>;


    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
private:
    size_type SIZE;
    size_type SIZEMAX;
    Vector *tab;
    std::hash <key_type> makeHash;
	mapped_type& getValue(const key_type& key) const {
		int HASH = makeHash(key) % SIZEMAX;
		if (tab[HASH].empty()) {
			throw std::out_of_range("Nie ma elementu o podanym kluczu w mapie");
		}
		for (size_type i = 0; i < tab[HASH].size(); ++i) {
			if (tab[HASH][i].first == key) {
				return tab[HASH][i].second;
			}
		}
		throw std::out_of_range("Nie ma elementu o podanym kluczu w mapie");
	}
public:
    HashMap() {
        SIZE = 0;
        SIZEMAX = 999999;
        tab = new Vector[SIZEMAX];
    }
    ~HashMap() {
        delete [] tab;
        SIZE = 0;
        SIZEMAX = 0;
    }

    HashMap(std::initializer_list<value_type> list): HashMap() {
		for (auto it = list.begin(); it != list.end(); ++it) {
			operator[] (it->first) = it->second;
		}
    }

    HashMap(const HashMap& other):HashMap() {
		for (auto it = other.begin(); it != other.end(); ++it) {
			operator[](it->first) = it->second;
		}
    }

    HashMap(HashMap&& other):HashMap() {
		/*SIZE = other.SIZE;
		SIZEMAX = other.SIZEMAX;
		tab = other.tab;

		other.SIZE = 0;
		other.SIZEMAX = 0;
		other.tab = 0;*/
		*this = std::move(other);
    }

    HashMap& operator=(const HashMap& other) {
		if (other == *this) {
			return *this;
		}
		SIZE = 0;
		delete[] tab;
		tab = new Vector[SIZEMAX];
		for (auto it = other.begin(); it != other.end(); ++it) {
			operator[](it->first) = it->second;
		}
		return *this;
    }

    HashMap& operator=(HashMap&& other) {
		if (other == *this) {
			return *this;
		}
		SIZE = 0;
		delete[] tab;
		tab = new Vector[SIZEMAX];
		std::swap(SIZE, other.SIZE);
		std::swap(SIZEMAX, other.SIZEMAX);
		std::swap(tab, other.tab);

		return *this;
    }

    bool isEmpty() const {
		if (SIZE == 0)
			return true;
		else
			return false;
    }

    mapped_type& operator[](const key_type& key) { ////Do porawy prawdopodobnie
		int HASH = makeHash(key) % SIZEMAX;
		for (size_type i = 0; i < tab[HASH].size(); ++i) {
			if (tab[HASH][i].first == key) {
				//tab[HASH][i].second = mapped_type{}; //moze - nie wiem czemu
				return tab[HASH][i].second;
			}
		}
		tab[HASH].push_back(value_type(key, mapped_type{}));
		++SIZE;
		return tab[HASH].back().second;
    }

    const mapped_type& valueOf(const key_type& key) const {
		return getValue(key);
    }

    mapped_type& valueOf(const key_type& key) {
		return getValue(key);
    }

    const_iterator find(const key_type& key) const {
		int HASH = makeHash(key) % SIZEMAX;
		if (SIZE==0 || tab[HASH].empty()) {
			return end();
		}

		ConstIterator it;
		it.tab = tab;
		it.HASH = HASH;
		it.pos = tab[HASH].size();
		for (size_type i = 0; i < tab[HASH].size(); ++i) {
			if (tab[HASH][i].first == key) {
				it.pos = i;
				break;
			}
		}
		return it;
    }

    iterator find(const key_type& key) {
		int HASH = makeHash(key) % SIZEMAX;
		if (SIZE == 0 || tab[HASH].empty()) {
			return end();
		}

		 Iterator it;
		it.tab = tab;
		it.HASH = HASH;
		it.pos = tab[HASH].size();
		for (size_type i = 0; i < tab[HASH].size(); ++i) {
			if (tab[HASH][i].first == key) {
				it.pos = i;
				break;
			}
		}
		return it;
    }

    void remove(const key_type& key) {
		int HASH = makeHash(key) % SIZEMAX;
		if (SIZE == 0 || tab[HASH].empty()) {
			throw std::out_of_range("Proba usuniecia elemntu nie istniejacego");
		}
		/*for (size_type i = 0; i < tab[HASH].size(); ++i) {
			if (tab[HASH][i].first == key) {

			}
		}*/
		for (auto it = tab[HASH].begin(); it != tab[HASH].end(); ++it) {
			if (it->first == key) {
				tab[HASH].erase(it);
				--SIZE;
				break;
			}
		}
    }

    void remove(const const_iterator& it) {
		if (tab[it.HASH].empty() || it.pos >= tab[it.HASH].size() || SIZE == 0) {
			throw std::out_of_range("Proba usuniecia elemntu nie istniejacego");
		}
		tab[it.HASH].erase(tab[it.HASH].begin() + it.pos);
		--SIZE;
    }

    size_type getSize() const {
		return SIZE;
    }

    bool operator==(const HashMap& other) const {
		if (SIZE != other.SIZE)
			return false;
		for (size_type i = 0; i < SIZEMAX; ++i) { //nie wydajne - zrobic iteratorem
			if (tab[i] != other.tab[i])
				return false;
		}
		return true;
    }

    bool operator!=(const HashMap& other) const {
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
class HashMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename HashMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using pointer = const typename HashMap::value_type*;

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
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::value_type*;

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

#endif /* AISDI_MAPS_HASHMAP_H */
