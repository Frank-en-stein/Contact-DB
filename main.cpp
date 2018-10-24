#include <map>
#include <limits>
#include <iostream>
#include <iterator>

using namespace std;

template <typename K,typename V>
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
        auto tmpR = r;
        auto defV = (--tmpR)->second;
        cout<<l->first<<" "<<r->first<<endl;
        // NOTE: handle begin and end cases
        if (l != r) {
            if (l == m_map.begin() || r == m_map.end()) m_map.erase(l, r);
            else if (l->first < r->first) m_map.erase(l, r);
        }
        auto ll = l, rr = r;

        if (rr == m_map.end()) m_map[keyEnd] = defV;
        else if (rr->first != keyEnd) m_map[keyEnd] = defV;

        --ll;
        if (ll == m_map.begin()) m_map[keyBegin] = val;
        else if (ll->second != val) m_map[keyBegin] = val;
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
		for(auto entry : m_map) {
			std::cout << entry.first << " " << entry.second << std::endl;
		}
	}

private:
	std::map<K,V> m_map;
};

int main() {
	mymap<unsigned int, char> imap {'#'};
	//imap.test_interval_map();
    imap.assign(1,7,'m');
    imap.assign(7, 10, 'm');
    imap.assign(0,68,'o');
    cout<<imap[1001]<<endl;
    imap.show();
}


