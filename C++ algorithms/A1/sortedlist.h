#include <iostream>

template <typename T>
class SortedList{
	struct Node{
        //Constructor that accept T data, pointer to next and prev node;
		Node(const T& data={},Node* nx=nullptr,Node* pr=nullptr):
				data(data), nx(nx), pr(pr)
        {}

        //Return pointer to next node
        Node* next(){ return nx; }

        //Return pointer to prev node
        Node* prev() { return pr; }

        //Set pointer to next node as arg next
        void setNext(Node *next){ this->nx = next; }

        //Set pointer to prev node as arg prev
        void setPrev(Node *prev){ this->pr = prev; }

        //Return stored data
        T& getData(){ return data; }
    private:
        T data;
		Node *nx;
		Node *pr;
	};

public:
	class const_iterator{
        //Set ParentClass as friend for accessing private/protected data
        friend class SortedList;
	public:
        //Default constructor
		const_iterator(){}

        //Constructor init node
        const_iterator(Node *node): node(node){}

        //Return next node
		const_iterator operator++(){
            node = node->next();
            return const_iterator{node};
        }

        //Return old value and move to next node
		const_iterator operator++(int){
            auto tmp = node;
            node = tmp->next();
            return const_iterator{tmp};
        }

        //Return prev node
		const_iterator operator--(){
            node = node->prev();
            return const_iterator{node};
        }

        //Return old val and move to prev node
		const_iterator operator--(int){
            auto tmp = node;
            node = node->prev();
            return const_iterator{tmp};
        }

        //Compare pointers to internal node
		bool operator==(const_iterator rhs){ return node == rhs.getNode();}

        //Compare pointers to internal node
		bool operator!=(const_iterator rhs){ return node != rhs.getNode();}
		const T& operator*()const{
            return node->getData();
        }

    protected:
        //return current node
        Node* getNode() { return node; }
        //return current node as cons
        const Node* getNode() const { return node; }

    private:
        Node *node;
	};

	class iterator: public const_iterator{
	public:
        //Default constructor
		iterator(){}
        //Constructor with init node. Init node will be passed to parent constructor.
        iterator(Node *node): const_iterator(node){};

        //Call parent prefix operator ++ and return new iter
		iterator operator++(){
            const_iterator::operator++();
            return iterator{const_iterator::getNode()};
        }

        //Call parent postfix operator ++ and return new iter
		iterator operator++(int){
            const_iterator::operator++();
            return iterator{const_iterator::getNode()->prev()};
        }

        ////Call parent prefix operator -- and return new iter
		iterator operator--(){
            const_iterator::operator--();
            return iterator{const_iterator::getNode()};
        }

        //Call parent postfix operator -- and return new iter
		iterator operator--(int){
            const_iterator::operator--();
            return iterator{const_iterator::getNode()->next()};
        }

        //Return mutable ref to internal data
		T& operator*(){
            return const_iterator::getNode()->getData();
        }

        //Return immutable ref to internal data
		const T& operator*()const {
            return const_iterator::getNode()->getData();
        }

	};

    //Default constructor
	SortedList();
	~SortedList();

    //Copy constructor. Deep copy will be performed
    SortedList(const SortedList& rhs);

    //Copy operator. Deep copy will be performed
	SortedList& operator=(const SortedList& rhs);

    //Move constructor. Move internal data to this and left old list clear.
    //No deep copy.
	SortedList(SortedList&& rhs);

    //Move copy operator. Move internal dat to this and left old list clear
    //No deep copy.
	SortedList& operator=(SortedList&& rhs);

    //Return iterator to first(head) element. If head empty return tail then.
	iterator begin(){ return iterator{(head)? head: tail}; }

    //Return iterator to last(tail) element
	iterator end(){ return iterator{tail}; }

	const_iterator begin() const { return const_iterator{ (head)? head: tail}; }
    const_iterator end() const { return const_iterator{tail}; }

    //Insert data in list. New node will be placed between smaller and larger data
	iterator insert(const T& data);

    //Search data
	iterator search(const T& data);
    //Same for const
	const_iterator search(const T& data) const;
    //Remove iter node from list. All iterators to this object will be invalidated.
	iterator erase(iterator it);
    //Remove all between first and last. All iterators to this sequenceo will be invalidated.
	iterator erase(iterator first, iterator last);
    //Return true if list contain atleast one element, except tail
	bool empty() const;
    //Return size of list
	int size() const;
    //Remove all data from this list
    void clear();

private:
    //Helper method for finding Node by data
    Node *find(const T &data) const;
    //Helper method for inserting 'newNode' directly after 'after'.
    Node *insertAfter(Node *after, Node *newNode);
    //Helper static method for deep copy of list
    static void make_copy(const SortedList<T> &from, SortedList<T> &to);

    //Poiter to first element
    Node *head;
    //Pointer to last not existing elemnt
    Node *tail;
    int length;
};

//Default constructor. Will create tail node for end and begin iterators.
template <typename T>
SortedList<T>::SortedList(): head(nullptr), length(0) {
    tail = new Node{};
}

template <typename T>
SortedList<T>::~SortedList(){
    clear();
}

template <typename T>
SortedList<T>::SortedList(const SortedList& rhs){
    //Set head and tail to null for clean current list
    head = nullptr;
    tail = nullptr;

    make_copy(rhs, *this);
}
template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs){
    make_copy(rhs, *this);
    return *this;
}

template <typename T>
SortedList<T>::SortedList(SortedList&& rhs){
    //Copy pointer to the rhs head element, tail and len
    head = rhs.head;
    tail = rhs.tail;
    length = rhs.size();

    //Set rhs to null, because this list now owner of rhs data
    rhs.head = nullptr;
    rhs.tail = nullptr;
    rhs.length = 0;
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::insert(const T& data){
    auto inserted = new Node{data};

    //If head not exist create it and fix prev/next for tail and head
    if (!head) {
        head = inserted;
        head->setNext(tail);

        tail->setPrev(head);

        length = 1;
        return iterator{head};
    }

    //Iterate through all nodes until end or larger data in node
    auto curr = head;
    while (curr && curr != tail && curr->getData() < data ){
        curr = curr->next();
    }

    //Insert it after found node
    insertAfter(curr, inserted);

    ++length;
    return iterator{};
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const T& data){
    auto node = find(data);
    if (!node) return end();

    return iterator{node};
}

template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const T& data) const{
    auto node = find(data);
    if (!node) return end();

    return const_iterator{node};
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it){
    if (length == 0) return end();

    Node *node = it.getNode();

    auto prev = node->prev();
    auto next = node->next();

    //If we havent prev node then we're removing current head
    if (!prev){
        //If next item is tail then this is the last elem
        if (next == tail) {
            head = nullptr;
        }else{
            head = next;
        }
    } else {
        //If we have prev element then set next as next of removed Node
        prev->setNext(next);
    }

    //Set prev node as prev of removed Node
    next->setPrev(prev);

    --length;

    //Free memory
    delete node;
    return iterator{next};
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator first, iterator last){
    //Iterating until reach of last iter. Remove node on each iteration.
    while(first != last){
        first = erase(first);
    }

    return last;
}
template <typename T>
bool SortedList<T>::empty() const{
    return length == 0;
}
template <typename T>
int SortedList<T>::size() const{
   return length;
}

template <typename T>
typename SortedList<T>::Node *SortedList<T>::find(const T &data) const {
    if (!head) return nullptr;

    //Iterate nodes until we reach tail or find node with the data
    auto curr = head;
    while(curr != tail && curr->getData() != data){
        curr = curr->next();
    }

    return curr;
}

template <typename T>
typename SortedList<T>::Node *SortedList<T>::insertAfter(SortedList::Node *after, SortedList::Node *newNode) {
    //Fix prev and next for 'newNode'
    newNode->setNext(after);
    newNode->setPrev(after->prev());

    //If the node hasn't prev then it's head
    //else we set next node to 'newNode' for it's prev
    if (!after->prev()){
        head = newNode;
    } else {
        after->prev()->setNext(newNode);
    }

    after->setPrev(newNode);

    return newNode;
}

template <typename T>
void SortedList<T>::clear() {
    //Check tail. If we have empty list we should delete it too.
    if (!head) {
        delete tail;
        tail = nullptr;
        return;
    }

    //Iterate fron head to last node and free mem for each
    auto curr = head;
    while (curr){
        auto next = curr->next();
        delete curr;
        curr = next;
    }

    head = nullptr;
    tail = nullptr;
}

template <typename T>
void SortedList<T>::make_copy(const SortedList<T> &from, SortedList<T> &to) {
    to.clear();
    if (from.empty()) return;

    auto it = from.begin();
    if (it == from.end()) return;

    to.head = new Node{*it++};

    //After 'to' was cleared we should iterate in from list and copy each node
    Node *prev = to.head;
    for (; it != from.end(); ++it) {
        auto node = new Node{*it, nullptr, prev};
        prev->setNext(node);

        prev = node;
    }

    //Fix tail
    to.tail = new Node{T(), nullptr, prev};
    prev->setNext(to.tail);

    to.length = from.size();
}

template <typename T>
SortedList<T> &SortedList<T>::operator=(SortedList &&rhs) {
    //Same as for move constructor
    clear();

    head = rhs.head;
    tail = rhs.tail;
    length = rhs.size();

    rhs.head = nullptr;
    rhs.tail = nullptr;
    rhs.length = 0;

    return *this;
}
