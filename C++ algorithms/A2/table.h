/*************************************************************/
/*    V 1.1:                                                 */
/*      - added prototypes for numRecords() and isEmpty()    */
/*      - coded isEmpty() and numRecords() in SimpleTable    */
/*      - created empty functions for isEmpty() and          */
/*        numRecords() in LPTable                            */
/*************************************************************/

#include <string>
#include <utility>
#include <algorithm>
#include <functional>
#include <cassert>

#include <iostream>
using namespace std;

//Complexity was:
//search()
//Key exist: O(n). It can be the last element in array
//Key not exist: O(n). We should iterate over all elements to understand that key not exist
//======================================================
//sort()
//Simple bubble sort. O(n^2)
//=======================================================
//grow()
//Complexity is O(n) because we need to iterate over all elements and copy it to new array
//=======================================================
//copy constructor
//update() - if item does not exists so you need to add it as a new record, without grow()
//Search - O(n), sort - O(n^2). So complexity - O(n + n^2)

//Iterate over all keys in other - O(n)
//Complexity - O(n * (k + k^2)). K is amount of items in the current table
//=========================================================
//copy assignment
//Iterate over all keys in other - O(n)
//Complexity - O(n * (k + k^2)). K is amount of items in the current table

//remove() - if item is there
//Search - O(n), shift items O(n-keyIdx). Complexity - O(n + n - keyIdx)

//update() - if item does not exists so you need to add it as a new record, without grow()
//Search - O(n), sort - O(n^2). Complexity - O(n + n^2)

//Complexity - O(n * (k + k^2) + n * (n + n^2))
//========================================================
//find() - if item is there
//Search - O(n). Complexity - O(n). Key can be last

//find() - if item is not there
//Search - O(n). Complexity - O(n).
//========================================================
//remove() - if item is there
//Search - O(n), shift items O(n-keyIdx). Complexity - O(n + n - keyIdx)

//remove() - if item is not there
//Search - O(n). Complexity - O(n)
//========================================================
//Destructor
//remove() - if item is there
//Search - O(n), shift items O(n-keyIdx). Complexity - O(n + n - keyIdx)
//Complexity: O(n * (n + n - keyIdx))
//========================================================
//update() - if item does not exists so you need to add it as a new record with grow()
//Search - O(n), grow - O(n), sort - O(n^2). So complexity - O(n + n + n^2)

//update() - if item does not exists so you need to add it as a new record, without grow()
//Search - O(n), sort - O(n^2). So complexity - O(n + n^2)

//update() - if item does exists and you are just modifying what is there
//Search - O(n). Complexity - O(n)
//=========================================================

//Changed:
//search
//update
//Constructor. Now it can accept 0
//Copy constructor
//Copy assigment operator
//remove
//update
//unused sort
//unused search


//New complexity:
//findNearest()
//Split array by middle. Check value. If value less then found then split piece by middle.
//We should do it until no elements left
//Key exist: O(log2(n)+1)
//Key not exist: O(log2(n)+1)
//=======================================================
//grow()
//Complexity is O(n) because we need to iterate over all elements and copy it to new array
//=======================================================
//copy constructor
//Iterate over all keys in other - O(n)
//Complexity - O(n)
//=========================================================
//copy assignment
//Remove all current items - O(n)
//Iterate over all keys in other - O(n)
//Complexity - O(n + n)
//========================================================
//find() - if item is there
//findNearest - O(log2(n)+1)

//find() - if item is not there
//findNearest - O(log2(n)+1)
//========================================================
//remove() - if item is there
//findNearest - O(log2(n)+1), shift items O(n-keyIdx). Complexity - O(log2(n)+1 + n - keyIdx)

//remove() - if item is not there
//findNearest - O(log2(n)+1). Complexity - O(log2(n)+1)
//========================================================
//Destructor
//Iterate over all items. Complexity: O(n)
//========================================================
//update() - if item does not exists so you need to add it as a new record with grow()
//findNearest - O(log2(n)+1), grow - O(n), shift - O(n - nearest). So complexity - O(log2(n)+1 + n + n - nearest)

//update() - if item does not exists so you need to add it as a new record, without grow()
//findNearest - O(log2(n)+1), shift - O(n - nearest). So complexity - O(log2(n)+1 + n - nearest)

//update() - if item does exists and you are just modifying what is there
//findNearest - O(log2(n)+1). Complexity - O(log2(n)+1)
//=========================================================


template <class TYPE>
class Table{
public:
	Table(){}
	virtual bool update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable:public Table<TYPE>{

	struct Record{
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data){
			key_=key;
			data_=data;
		}
	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int search(const string& key);
	void sort();
	void grow();

	bool findNearest(const string &key, int &idx);
	void insertBefore(const string &key, const TYPE &value, int idx);

    void shiftRight(int from);
    void shiftLeft(int from);
public:
	SimpleTable(int maxExpected);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
	virtual bool isEmpty() const{return size_==0;}
	virtual int numRecords() const{return size_;}
};

//Key exist: O(n). It can be the last element in array
//Key not exist: O(n). We should iterate over all elements to understand that key not exist
//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key){
	int rc=-1;
	for(int i=0;i<size_;i++){
		if(records_[i]->key_==key){
			rc=i;
		}
	}
	return rc;
}

//Simple bubble sort. O(n^2)
//sort the according to key in table
template <class TYPE>
void SimpleTable<TYPE>::sort(){
   int minIdx=0;
   for(int i=0;i<size_;i++){
   		minIdx=i;
     	for(int j=i+1;j<size_;j++){
     		if(records_[j]->key_ < records_[minIdx]->key_){
     			minIdx=j;
     		}
    	}
    	Record* tmp=records_[i];
    	records_[i]=records_[minIdx];
    	records_[minIdx]=tmp;
   }
}

//Complexity is O(n) because we need to iterate over all elements and copy it to new array
//grow the array by one element
template <class TYPE>
void SimpleTable<TYPE>::grow(){
    if (max_ == 0) max_ = 1;

	Record** newArray=new Record*[max_*2];
	max_=max_*2;

    std::copy(records_, records_ + size_, newArray);
    if (size_ > 0) delete [] records_;

	records_=newArray;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int maxExpected): Table<TYPE>(){
    if (maxExpected != 0) {
        records_ = new Record *[maxExpected];
        max_ = maxExpected;
    } else {
        max_ = 0;
    }

	size_=0;
}

//copy constructor
//update() - if item does not exists so you need to add it as a new record, without grow()
//Search - O(n), sort - O(n^2). So complexity - O(n + n^2)

//Iterate over all keys in other - O(n)
//Complexity - O(n * (k + k^2)). K is amount of items in the current table
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
	records_=new Record*[other.max_];
	max_=other.max_;
	size_=other.size_;

    for(int i = 0; i < size_; i++){
        auto rec = other.records_[i];
        records_[i] = new Record{rec->key_, rec->data_};
    }
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
	size_=other.size_;
	max_=other.max_;
	records_=other.records_;
	other.records_=nullptr;
	other.size_=0;
	other.max_=0;
}

//update() - if item does not exists so you need to add it as a new record with grow()
//Search - O(n), grow - O(n), sort - O(n^2). So complexity - O(n + n + n^2)

//update() - if item does not exists so you need to add it as a new record, without grow()
//Search - O(n), sort - O(n^2). So complexity - O(n + n^2)

//update() - if item does exists and you are just modifying what is there
//Search - O(n). Complexity - O(n)
template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	int idx = 0;
	bool found = findNearest(key, idx);
	if (found){
		records_[idx]->data_ = value;
		return true;
	}

    insertBefore(key, value, idx);
	return true;
}

//remove() - if item is there
//Search - O(n), shift items O(n-keyIdx). Complexity - O(n + n - keyIdx)

//remove() - if item is not there
//Search - O(n). Complexity - O(n)
template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
    int idx = 0;
	if (!findNearest(key, idx)) return false;

    delete records_[idx];
    shiftLeft(idx);

    --size_;
    return true;
}

//find() - if item is there
//Search - O(n). Complexity - O(n). Key can be last

//find() - if item is not there
//Search - O(n). Complexity - O(n).
template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
    int idx = 0;
    if (!findNearest(key, idx)) return false;

    value = records_[idx]->data_;
	return true;
}

//copy assignment
//Iterate over all keys in other - O(n)
//Complexity - O(n * (k + k^2)). K is amount of items in the current table

//remove() - if item is there
//Search - O(n), shift items O(n-keyIdx). Complexity - O(n + n - keyIdx)

//update() - if item does not exists so you need to add it as a new record, without grow()
//Search - O(n), sort - O(n^2). Complexity - O(n + n^2)

//Complexity - O(n * (k + k^2) + n * (n + n^2))

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
    if (this == &other) return *this;

    if(records_) {
        for (int i = 0; i < size_; i++) {
            delete records_[i];
        }
        delete[] records_;
    }

    max_= other.max_;
    size_= other.size_;

    records_ = new Record*[max_];
    for(int i = 0; i < size_; i++){
        auto rec = other.records_[i];
        records_[i] = new Record{rec->key_, rec->data_};
    }

	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
	swap(records_,other.records_);
	swap(size_,other.size_);
	swap(max_,other.max_);
	return *this;
}

//remove() - if item is there
//Search - O(n), shift items O(n-keyIdx). Complexity - O(n + n - keyIdx)
//Complexity: O(n * (n + n - keyIdx))
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){
		for(int i=0;i<size_;i++){
			delete records_[i];
		}
		delete [] records_;
	}
}

//Created new find
//Find Record by key and write it index
//key: key value that function should find
//idx: if function succeed in idx will be index of found element
//else index after larger element
//Return true if key found. If key doesn't exist return false
template <class TYPE>
bool SimpleTable<TYPE>::findNearest(const string &key, int &idx) {
    auto end = records_ + size_;
    auto found = std::lower_bound(records_, end, key,
                                  [](const Record* lhs, const std::string &rhs){
                                      return lhs->key_ < rhs;
                                  });

    if (found == end){
        idx = size_;
        return false;
    }

    idx = std::distance(records_, found);
    return (*found)->key_ == key;
}

//Inserts new Record after index
//key: record key
//value: record value
//idx: index of element after which new element will be inserted
template <class TYPE>
void SimpleTable<TYPE>::insertBefore(const string &key, const TYPE &value, int idx) {
	if (size_ + 1 > max_){
		grow();
	}

    if (size_ == 0) {
        records_[idx] = new Record{key, value};
        ++size_;
        return;
    }

	if (idx == size_){
		records_[idx] = new Record{key, value};
		++size_;
		return;
	}


    shiftRight(idx);
	records_[idx] = new Record{key, value};
	++size_;
}

//Copy all elements from index right
//from: index which should be moved
template <class TYPE>
void SimpleTable<TYPE>::shiftRight(int from) {
    std::copy(records_ + from, records_ + size_, records_ + from + 1);
}

//Copy all elements from index left. Item on from will be replaced by from + 1
//from: index which should be moved
template <class TYPE>
void SimpleTable<TYPE>::shiftLeft(int from) {
    std::copy(records_ + from + 1, records_ + size_, records_ + from);
}


template <class TYPE>
class LPTable:public Table<TYPE>{
public:
	LPTable(int maxExpected,double percentOpen);
	LPTable(const LPTable& other);
	LPTable(LPTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const LPTable& operator=(const LPTable& other);
	virtual const LPTable& operator=(LPTable&& other);
	virtual ~LPTable();
	virtual bool isEmpty() const { return used == 0;}
	virtual int numRecords() const { return used; }

private:
    struct Object {
        Object(const std::string &key, const TYPE &value, bool removed = false)
                : key(key), value(value), removed(removed)
        {}

        std::string key;
        TYPE value;
        bool removed;
    };

    bool probe(const std::string &key, int from, int &idx, bool skipRemoved);
    void clear();

    hash<string> hashFn;
    Object **objects;
    int capacity;
    int used;
};
template <class TYPE>
LPTable<TYPE>::LPTable(int maxExpected, double percentOpen): Table<TYPE>(), used(0) {
    capacity = static_cast<int>(maxExpected + maxExpected * percentOpen);
    objects = new Object*[capacity];

    for (int i = 0; i<capacity; ++i)
        objects[i] = nullptr;

}

template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other){
    this->capacity = other.capacity;
    this->used = other.used;

    objects = new Object*[other.capacity];
    for (int i = 0; i < capacity; ++i){
        auto obj = other.objects[i];
        if (obj) {
            objects[i] = new Object(obj->key, obj->value, obj->removed);
        } else {
            objects[i] = nullptr;
        }
    }
}
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other){
    this->used = other.used;
    this->capacity = other.capacity;
    this->objects = other.objects;

    other.objects = nullptr;
    other.capacity = 0;
    other.used = 0;
}
template <class TYPE>
bool LPTable<TYPE>::update(const string& key, const TYPE& value){
    if (capacity == used) return false;
    size_t idx = hashFn(key) % capacity;

    int objIdx = 0;
    if (probe(key, idx, objIdx, false)){
        objects[objIdx]->value = value;
    } else {
        if (!objects[objIdx]) {
            objects[objIdx] = new Object{key, value};
        } else {
            objects[objIdx]->key = key;
            objects[objIdx]->value = value;
            objects[objIdx]->removed = false;
        }
        ++used;
    }

    return true;
}

template <class TYPE>
bool LPTable<TYPE>::remove(const string& key){
    auto idx = hashFn(key) % capacity;

    int objIdx;
    if (!probe(key, idx, objIdx, true)) return false;
    objects[objIdx]->removed = true;

    --used;
	return true;
}

template <class TYPE>
bool LPTable<TYPE>::find(const string& key, TYPE& value){
    auto idx = hashFn(key) % capacity;

    int objIdx;
    if (!probe(key, idx, objIdx, true)) return false;

    value = objects[objIdx]->value;
    return true;
}

template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other){
    if (this == &other) return *this;
    clear();

    this->capacity = other.capacity;
    this->used = other.used;

    objects = new Object*[other.capacity];
    for (int i = 0; i < capacity; ++i){
        auto obj = other.objects[i];
        if (obj) {
            objects[i] = new Object(obj->key, obj->value, obj->removed);
        } else {
            objects[i] = nullptr;
        }
    }

    return *this;
}
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other){
    if (this == &other) return *this;
    clear();

    this->used = other.used;
    this->capacity = other.capacity;
    this->objects = other.objects;

    other.objects = nullptr;
    other.capacity = 0;
    other.used = 0;
	return *this;
}
template <class TYPE>
LPTable<TYPE>::~LPTable(){
    clear();
}

//Trying to find key in array or nearest available place for next element
//key: Key for table.
//from: Index from search will be started
//idx: index for first blank or removed object
//skipRemoved: if true then continue until blank item or item with key = arg key
//if false then break on first blank/removed item
//return true if key found else false
template <typename TYPE>
bool LPTable<TYPE>::probe(const std::string &key, int from, int &idx, bool skipRemoved) {
    for (idx = from; idx < capacity; ++idx){
        auto obj = objects[idx];
        if (!obj) return false;
        if (obj->removed) {
            if (skipRemoved) continue;
            return false;
        }

        if (obj->key == key) return true;
    }

    for (idx = 0; idx < from; ++idx){
        auto obj = objects[idx];
        if (!obj) return false;
        if (obj->removed) {
            if (skipRemoved) continue;
            return false;
        }


        if (obj->key == key) return true;
    }

    idx = -1;
    return false;
}

//Deallocate each item from objects. After deallocate array.
template <typename TYPE>
void LPTable<TYPE>::clear() {
    for (int i = 0; i < capacity; ++i){
        if (objects[i]) delete objects[i];
    }

    delete[] objects;
}


