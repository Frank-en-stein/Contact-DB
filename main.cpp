#include <map>
#include <limits>
#include <iostream>
#include <iterator>
#include <string>

template <typename K, typename V>
class interval_map {
public:
	interval_map(V const& val) {
		m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
	}
	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		return (--m_map.upper_bound(key))->second;
	}

	void assign(K const & keyBegin, K const & keyEnd, V const & val) {
		if (!(keyBegin < keyEnd)) return;
		if (keyBegin < std::numeric_limits<K>::lowest()) return;

		auto l = m_map.lower_bound(keyBegin);
		auto r = m_map.lower_bound(keyEnd);
		auto defV = std::prev(r)->second;
		//cout << l->first << " " << r->first << endl;
		if (l != r) {
			if (l == m_map.begin() || r == m_map.end()) m_map.erase(l, r);
			else if (l->first < r->first) m_map.erase(l, r);
		}

		auto ll = m_map.insert(r, std::pair<K, V>(keyBegin, val));
		while (ll != m_map.begin()) {
			if (!(std::prev(ll)->second == ll->second)) break;
			else {
				auto tmp = ll;
				ll = std::prev(ll);
				m_map.erase(tmp);
			}
		}

		if (r == m_map.end()) {
			auto rr = m_map.insert(r, std::pair<K, V>(keyEnd, defV));
			while (std::next(rr) != m_map.end()) {
				if (!(std::next(rr)->second == rr->second)) break;
				else rr = std::next(rr);
			}
			while (rr != m_map.begin()) {
				if (!(std::prev(rr)->second == rr->second)) break;
				else {
					auto tmp = rr;
					rr = std::prev(rr);
					m_map.erase(tmp);
				}
			}
		}
		else if (keyEnd < r->first && !(r->second == defV)) {
			auto rr = m_map.insert(r, std::pair<K, V>(keyEnd, defV));
			while (std::next(rr) != m_map.end()) {
				if (!(std::next(rr)->second == rr->second)) break;
				else rr = std::next(rr);
			}
			while (rr != m_map.begin()) {
				if (!(std::prev(rr)->second == rr->second)) break;
				else {
					auto tmp = rr;
					rr = std::prev(rr);
					m_map.erase(tmp);
				}
			}
		}
		else if (ll != m_map.end()) if(std::next(ll) != m_map.end()) {
			if (ll->second == std::next(ll)->second) m_map.erase(std::next(ll));
		}
	}

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
	interval_map<unsigned int, std::string> imap{ "#" };
	
	
	imap.assign(80, 458, "m");
	imap.assign(85, 100, "p");
	imap.assign(90, 95, "q");
	imap.assign(91, 93, "r");
	imap.show();
}


