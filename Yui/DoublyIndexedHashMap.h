#ifndef _DOUBLy_INDEXED_HASH_MAP
#define _DOUBLy_INDEXED_HASH_MAP

#ifndef HASH_MAP_TYPE
#include <unordered_map>
#define	HASH_MAP_TYPE std::unordered_map
#endif

template<class Key1, class Key2, class Value>
class DoublyIndexedHashMap
{
private:
	HASH_MAP_TYPE<Key1, HASH_MAP_TYPE<Key2, Value> > data_;
	
public:	
	DoublyIndexedHashMap()	{}
	void Insert(const Key1 &key1, const Key2 &key2, const Value &value)
	{
		(data_[key1])[key2] = value;
	}
	
	bool Contains(const Key1 &key1, const Key2 &key2)	const
	{
		return data_.find(key1) != data_.end() && data_.at(key1).find(key2) != data_.at(key1).end();
	}
	
	const Value &At(const Key1 key1, const Key2 &key2)	const
	{
		return data_.at(key1).at(key2);
	}
	
	Value &At(const Key1 key1, const Key2 &key2)
	{
		return data_.at(key1).at(key2);
	}
};

#if 0
int main() {
	// your code goes here
	int *a = new int;
	int *b = new int;
	int *c = new int;
	DoublyIndexedHashMap<int*, int*, bool> test;
	test.Insert(a, b, false);
	test.Insert(a, c, false);
	cout << test.Contains(a, a); 
	cout << test.Contains(a, b);
	cout << test.Contains(a, c); 
	return 0;
}
#endif

#endif
