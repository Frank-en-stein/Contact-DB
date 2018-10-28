#include <map>
#include <limits>
#include <iostream>
#include <iterator>

using namespace std;

template <typename K, typename V>
class mymap {
public:
	mymap(V const & val) {
		m_map.insert(m_map.begin(), std::make_pair(std::numeric_limits<K>::min(), val));
	}
	V & operator [] (K const & key) {
		return (--m_map.upper_bound(key))->second;
	}

	void assign(K const & keyBegins, K const & keyEnds, V const & val) {
		K keyBegin = keyBegins;
		K keyEnd = keyEnds;
		if (keyBegin >= keyEnd) return;
		if (keyBegin < std::numeric_limits<K>::min()) keyBegin = std::numeric_limits<K>::min();
		if (keyEnd > std::numeric_limits<K>::max()) keyEnd = std::numeric_limits<K>::max();

		auto l = m_map.lower_bound(keyBegin);
		auto r = m_map.lower_bound(keyEnd);
		auto defV = std::prev(r)->second;
		//cout << l->first << " " << r->first << endl;
		if (l != r) {
			if (l == m_map.begin() || r == m_map.end()) m_map.erase(l, r);
			else if (l->first < r->first) m_map.erase(l, r);
		}

		auto ll = m_map.insert(std::pair<K, V>(keyBegin, val));
		auto pointer = ll.first;
		while (pointer != m_map.begin()) {
			if (prev(pointer)->second != pointer->second) break;
			else {
				auto tmp = pointer;
				pointer = prev(pointer);
				m_map.erase(tmp);
			}
		}

		if (r == m_map.end()) {
			auto rr = m_map.insert(std::pair<K, V>(keyEnd, defV));
			_clear(rr.first);
		}
		else if (r->first != keyEnd && r->second != defV) {
			auto rr = m_map.insert(std::pair<K, V>(keyEnd, defV));
			_clear(rr.first);
		}
		else if (next(pointer) != m_map.end()) {
			if (pointer->second == next(pointer)->second) m_map.erase(next(pointer));
		}
	}

	template<typename Iterator>
	void _clear(Iterator pointer) {
		while (next(pointer) != m_map.end()) {
			if (next(pointer)->second != pointer->second) break;
			else pointer = std::next(pointer);
		}
		while (pointer != m_map.begin()) {
			if (prev(pointer)->second != pointer->second) break;
			else {
				auto tmp = pointer;
				pointer = std::prev(pointer);
				m_map.erase(tmp);
			}
		}
	}

	template<typename Iterator>
	void _clearDuplicates(Iterator a, Iterator b) {
		if (a == b) return;
		V val = a->second;
		Iterator curr = a;
		do {
			curr = next(curr);
			if (curr->second == val) {
				Iterator tmp = curr;
				m_map.erase(tmp);
			}
			else {
				val = curr->second;
			}
		} while (curr != b);
	}

	void test_interval_map() {
		m_map.insert(std::make_pair(1, 'b'));
		m_map.insert(std::make_pair(4, 'c'));
		m_map.insert(std::make_pair(7, 'd'));
		m_map.insert(std::make_pair(10, 'e'));
		m_map.insert(std::make_pair(13, 'a'));
	}

	// a print function for debugging
	void show() {
		std::cout << "show" << std::endl;
		for (auto entry : m_map) {
			std::cout << entry.first << " " << entry.second << std::endl;
		}
	}

private:
	std::map<K, V> m_map;
};

int main() {
	mymap<unsigned int, char> imap{ '#' };
	//imap.test_interval_map();
	
	imap.assign(-1, 69, 'm');
	imap.assign(0, 7, 'm');
	imap.show();
}


