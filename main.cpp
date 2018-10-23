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
		m_map.insert(m_map.begin(), std::make_pair(std::numeric_limits<K>::max(), val));
	}
	V & operator [] (K const & key) {
		return (--m_map.upper_bound(key))->second;
	}

	void assign(K const & keyBegin, K const & keyEnd, V const & val) {

		if (keyBegin >= keyEnd) return;
        auto l = m_map.lower_bound(keyBegin);
        if (l == m_map.end()) l = m_map.begin();
        auto r = m_map.lower_bound(keyEnd);
        //auto lprev = --l; ++l;
        //auto rpost = ++r; --r;
        cout<<l->first<<" "<<r->first<<endl;
        if (l->first < r->first) m_map.erase(l, r);


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
			std::cout << entry.first << entry.second << std::endl;
		}
	}

private:
	std::map<K,V> m_map;
};

int main() {
	mymap<unsigned int, char> imap {'a'};
	imap.test_interval_map();
    imap.assign(0,10,'m');
    imap.show();
}


