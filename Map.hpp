#include <iostream>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <cassert>

namespace cs540{
	template<typename Key_T, typename Mapped_T>
	class Map{
		private: 
			typedef std::pair<const Key_T, Mapped_T> ValueType;
			class Node{
				public:
					ValueType val;
					Node* lChild;
					Node* rChild;
					Node* parent;
					Node* predecessor;
					Node* successor;
					Node(ValueType t1) : val(t1) {}
			};
			Node* dummy = (Node*)malloc(sizeof(Node));
			Node* root = dummy;
			Node* rdummy = (Node*)malloc(sizeof(Node));
			size_t numElements = 0;
			//void inorderTraversal(Node* currentNode);
			void singleRotationLeft(Node* imbalancedRoot);
			void doubleRotationLeft(Node* imbalancedRoot);
			void singleRotationRight(Node* imbalancedRoot);
			void doubleRotationRight(Node* imbalancedRoot);
			int sizeOfSubtree(Node* rootOfSubtree);

		public:
			class ConstIterator;
			class Iterator{
				public:
					Iterator(Node* n) : thisNode(n) {}
					Node* thisNode;
					ValueType& operator*() const{
						return this->thisNode->val;
					}
					ValueType* operator->() const{
						return &(this->thisNode->val);
					}
					Iterator& operator++(){
						this->thisNode = this->thisNode->successor;
						return *(this);
					}
					Iterator operator++(int){
						Iterator it = (*this);
						++(*this);
						return it;
					}
					Iterator& operator--(){
						this->thisNode = this->thisNode->predecessor;
						return *(this);
					}
					Iterator operator--(int){
						Iterator it = (*this);
						--(*this);
						return it;
					}
					bool operator==(const Iterator& I2) const{
						return this->thisNode == I2.thisNode;
					}
					bool operator!=(const Iterator& I2) const{
						return !(this->thisNode == I2.thisNode);
					}
					bool operator==(const ConstIterator& I2) const{
						return this->thisNode == I2.thisNode;
					}
					bool operator!=(const ConstIterator& I2) const{
						return !(this->thisNode == I2.thisNode);
					}
			};
			class ConstIterator{
				public:
					ConstIterator(Node* n) : thisNode(n) {}
					Node* thisNode;
					const ValueType& operator*() const{
						return this->thisNode->val;
					}
					const ValueType* operator->() const{
						return &(this->thisNode->val);
					}
					ConstIterator& operator++(){
						this->thisNode = this->thisNode->successor;
						return *(this);
					}
					ConstIterator operator++(int){
						ConstIterator it = (*this);
						++(*this);
						return it;
					}
					ConstIterator& operator--(){
						this->thisNode = this->thisNode->predecessor;
						return *(this);
					}
					ConstIterator operator--(int){
						ConstIterator it = (*this);
						--(*this);
						return it;
					}
					bool operator==(const Iterator& I2) const{
						return this->thisNode == I2.thisNode;
					}
					bool operator!=(const Iterator& I2) const{
						return !(this->thisNode == I2.thisNode);
					}
					bool operator==(const ConstIterator& I2) const{
						return this->thisNode == I2.thisNode;
					}
					bool operator!=(const ConstIterator& I2) const{
						return !(this->thisNode == I2.thisNode);
					}
			};
			class ReverseIterator{
				public:
					ReverseIterator(Node* n) : thisNode(n) {}
					Node* thisNode;
					ValueType& operator*() const{
						return this->thisNode->val;
					}
					ValueType* operator->() const{
						return &(this->thisNode->val);
					}
					ReverseIterator& operator++(){
						this->thisNode = this->thisNode->predecessor;
						return *(this);
					}
					ReverseIterator operator++(int){
						ReverseIterator it = (*this);
						++(*this);
						return it;
					}
					ReverseIterator& operator--(){
						this->thisNode = this->thisNode->successor;
						return *(this);
					}
					ReverseIterator operator--(int){
						ReverseIterator it = (*this);
						--(*this);
						return it;
					}
					bool operator==(const ReverseIterator& I2) const{
						return this->thisNode == I2.thisNode;
					}
					bool operator!=(const ReverseIterator& I2) const{
						return !(this->thisNode == I2.thisNode);
					}
			};
			size_t size() const{
				return numElements;
			}
			bool empty() const{
				return size() == 0;
			}
			void clear(){
				//used in destructor
				if(!empty()){
					Iterator it1 = begin();
					while(it1 != end()){
						Iterator temp = it1;
						++it1;
						delete temp.thisNode;
					}
				}
				numElements = 0;
			}
			std::pair<Iterator, bool> insert(ValueType t1);

			template<typename IT_T>
			void insert(IT_T range_beg, IT_T range_end);

			void erase(const Key_T& item);
			void erase(Iterator it);
			Iterator find(const Key_T& t1);
			ConstIterator find(const Key_T& t1) const;
			//void traversePrintedSorted();
			Iterator begin();
			Iterator end();
			ConstIterator begin() const;
			ConstIterator end() const;
			ReverseIterator rbegin();
			ReverseIterator rend();
			Mapped_T& at(const Key_T& k);
			const Mapped_T& at(const Key_T& k) const;
			//const Mapped_T& index(int index) const;
			//const Mapped_T& findIndex(int index) const;
			Map() {}
			Map(const Map& m1){
				for(ConstIterator it = m1.begin(); it != m1.end(); ++it){
					insert(std::make_pair(it->first, it->second));
				}
			}
			Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list){
				for(auto it = list.begin(); it != list.end(); ++it){
					insert(*it);
				}
			}
			Map& operator=(const Map& m1){
				if(&m1 != this){
					//simulate destructor of current nodes, prevent mem leak
					if(!empty()){
						Iterator it1 = begin();
						while(it1 != end()){
							Iterator temp = it1;
							++it1;
							delete temp.thisNode;
						}
					}
					numElements = 0;
					//now essentially copy constructor
					for(ConstIterator it = m1.begin(); it != m1.end(); ++it){
						insert(std::make_pair(it->first, it->second));
					}
				}
				return (*this);
			}
			Mapped_T& operator[](const Key_T& k){
				Mapped_T* ret;
				try{
					ret = &(at(k));
				}
				catch(std::out_of_range& ex){
					Mapped_T mt;
					insert(std::make_pair(k, mt));
					ret = &(at(k));
				}
				return *ret;
			}
			~Map();
	};

	template<typename Key_T, typename Mapped_T>
	int Map<Key_T, Mapped_T>::sizeOfSubtree(Node* rootOfSubtree){
		if(rootOfSubtree == NULL){return 0;}
		int max = sizeOfSubtree(rootOfSubtree->lChild);
		if(sizeOfSubtree(rootOfSubtree->rChild) > max){
			max = sizeOfSubtree(rootOfSubtree->rChild);
		}
		return max + 1;
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::singleRotationRight(Node* imbalancedRoot){
		Node* parentOfImbalance = imbalancedRoot->parent;
		Node* firstChild = imbalancedRoot->lChild;
		firstChild->parent = parentOfImbalance;
		if(imbalancedRoot != root && parentOfImbalance->lChild == imbalancedRoot){
			parentOfImbalance->lChild = firstChild;
		}
		else if(imbalancedRoot != root){
			parentOfImbalance->rChild = firstChild;
		}
		imbalancedRoot->lChild = firstChild->rChild;
		if(imbalancedRoot->lChild != NULL){imbalancedRoot->lChild->parent = imbalancedRoot;}
		firstChild->rChild = imbalancedRoot;
		imbalancedRoot->parent = firstChild;
		if(root == imbalancedRoot){root = firstChild;}
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::doubleRotationRight(Node* imbalancedRoot){
		Node* parentOfImbalance = imbalancedRoot->parent;
		//first rotation
		Node* innerRoot = imbalancedRoot->lChild;
		Node* firstChild = innerRoot->rChild;
		firstChild->parent = imbalancedRoot;
		innerRoot->parent = firstChild;
		innerRoot->rChild = firstChild->lChild;
		if(innerRoot->rChild != NULL) {innerRoot->rChild->parent = innerRoot;}
		firstChild->lChild = innerRoot;
		imbalancedRoot->lChild = firstChild;

		//second rotation
		firstChild = imbalancedRoot->lChild;
		imbalancedRoot->lChild = firstChild->rChild;
		if(imbalancedRoot->lChild != NULL){imbalancedRoot->lChild->parent = imbalancedRoot;}
		firstChild->rChild = imbalancedRoot;
		imbalancedRoot->parent = firstChild;
		firstChild->parent = parentOfImbalance;
		if(imbalancedRoot != root && parentOfImbalance->lChild == imbalancedRoot){
			parentOfImbalance->lChild = firstChild;
		}
		else if(imbalancedRoot != root){
			parentOfImbalance->rChild = firstChild;
		}
		if(root == imbalancedRoot){root = firstChild;}
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::singleRotationLeft(Node* imbalancedRoot){
		Node* parentOfImbalance = imbalancedRoot->parent;
		Node* firstChild = imbalancedRoot->rChild;
		firstChild->parent = parentOfImbalance;
		if(imbalancedRoot != root && parentOfImbalance->lChild == imbalancedRoot){
			parentOfImbalance->lChild = firstChild;
		}
		else if(imbalancedRoot != root){
			parentOfImbalance->rChild = firstChild;
		}
		imbalancedRoot->rChild = firstChild->lChild;
		if(imbalancedRoot->rChild != NULL){imbalancedRoot->rChild->parent = imbalancedRoot;}
		firstChild->lChild = imbalancedRoot;
		imbalancedRoot->parent = firstChild;
		if(root == imbalancedRoot){root = firstChild;}
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::doubleRotationLeft(Node* imbalancedRoot){
		Node* parentOfImbalance = imbalancedRoot->parent;
		//first rotation
		Node* innerRoot = imbalancedRoot->rChild;
		Node* firstChild = innerRoot->lChild;
		firstChild->parent = imbalancedRoot;
		innerRoot->parent = firstChild;
		innerRoot->lChild = firstChild->rChild;
		if(innerRoot->lChild != NULL) {innerRoot->lChild->parent = innerRoot;}
		firstChild->rChild = innerRoot;
		imbalancedRoot->rChild = firstChild;

		//second rotation
		firstChild = imbalancedRoot->rChild;
		imbalancedRoot->rChild = firstChild->lChild;
		if(imbalancedRoot->rChild != NULL){imbalancedRoot->rChild->parent = imbalancedRoot;}
		firstChild->lChild = imbalancedRoot;
		imbalancedRoot->parent = firstChild;
		firstChild->parent = parentOfImbalance;
		if(imbalancedRoot != root && parentOfImbalance->lChild == imbalancedRoot){
			parentOfImbalance->lChild = firstChild;
		}
		else if(imbalancedRoot != root){
			parentOfImbalance->rChild = firstChild;
		}
		if(root == imbalancedRoot){root = firstChild;}
	}

	/*template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::inorderTraversal(Node* currentNode){
		if(currentNode->lChild != NULL){
			inorderTraversal(currentNode->lChild);
		}
		std::cout << currentNode->val.first << std::endl;
		if(currentNode->rChild != NULL){
			inorderTraversal(currentNode->rChild);
		}
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::traversePrintedSorted(){
		if(size() != 0){
			inorderTraversal(root);
		}
	}*/

	template<typename Key_T, typename Mapped_T>
	template<typename IT_T>
	void Map<Key_T, Mapped_T>::insert(IT_T range_beg, IT_T range_end){
		while(range_beg != range_end){
			insert(std::make_pair((*range_beg).first, (*range_beg).second));
			++range_beg;
		}
	}

	template<typename Key_T, typename Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::insert(ValueType t1){
		Node* toInsert;
		if(size() == 0){
			root = new Node(t1);
			toInsert = root;
			root->lChild = NULL;
			root->rChild = NULL;
			root->parent = NULL;
			root->predecessor = rdummy;
			root->successor = dummy;
			dummy->predecessor = root;
			rdummy->successor = root;
		}
		else{
			Node* current = root;
			Node* prev = NULL;
			bool left;
			while(current != NULL){
				if(current->val.first == t1.first){
					Iterator toRet = find(t1.first);
					return std::make_pair(toRet, false);
				}
				else if(current->val.first < t1.first){
					prev = current;
					current = current->rChild;
					left = false;
				}
				else{
					prev = current;
					current = current->lChild;
					left = true;
				}
			}
			if(left == true){
				prev->lChild = new Node(t1);
				toInsert = prev->lChild;
				prev->lChild->lChild = NULL;
				prev->lChild->rChild = NULL;
				prev->lChild->parent = prev;
				
				prev->lChild->predecessor = prev->predecessor;
				if(prev->lChild->predecessor != NULL){prev->lChild->predecessor->successor = prev->lChild;}

				prev->predecessor = prev->lChild;
				prev->lChild->successor = prev;
			}
			else{
				prev->rChild = new Node(t1);
				toInsert = prev->rChild;
				prev->rChild->lChild = NULL;
				prev->rChild->rChild = NULL;
				prev->rChild->parent = prev;

				prev->rChild->successor = prev->successor;
				if(prev->rChild->successor != NULL) {prev->rChild->successor->predecessor = prev->rChild;}

				prev->successor = prev->rChild;
				prev->rChild->predecessor = prev;
			}
		}
		while(toInsert != NULL){ 
			if(sizeOfSubtree(toInsert->lChild) - sizeOfSubtree(toInsert->rChild) == 2){
				if(sizeOfSubtree(toInsert->lChild->lChild) > sizeOfSubtree(toInsert->lChild->rChild)){
					singleRotationRight(toInsert);
				}
				else{
					doubleRotationRight(toInsert);
				}
				break;
			}
			else if(sizeOfSubtree(toInsert->lChild) - sizeOfSubtree(toInsert->rChild) == -2){
				if(sizeOfSubtree(toInsert->rChild->rChild) > sizeOfSubtree(toInsert->rChild->lChild)){
					singleRotationLeft(toInsert);
				}
				else{
					doubleRotationLeft(toInsert);
				}
				break;
			}
			toInsert = toInsert->parent;
		}
		numElements++;
		return std::make_pair(find(t1.first), true);
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin(){
		if(empty()){
			Iterator dumb(dummy);
			return dumb;
		}
		Node* toRet = root;
		while(toRet->lChild != NULL){
			toRet = toRet->lChild;
		}
		Iterator ret(toRet);
		return ret;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const{
		if(empty()){
			ConstIterator dumb(dummy);
			return dumb;
		}
		Node* toRet = root;
		while(toRet->lChild != NULL){
			toRet = toRet->lChild;
		}
		ConstIterator ret(toRet);
		return ret;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin(){
		if(empty()) {return ReverseIterator(rdummy);}
		Iterator ret = end();
		--ret;
		ReverseIterator realRet(ret.thisNode);
		return realRet;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end(){
		Iterator ret(dummy);
		return ret;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const{
		ConstIterator ret(dummy);
		return ret;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend(){
		ReverseIterator realRet(rdummy);
		return realRet;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T& t1){
		Iterator ret = begin();
		while(ret != end()){
			if(ret.thisNode->val.first == t1){
				return ret;
			}
			else{
				ret.thisNode = ret.thisNode->successor;
			}
		}
		return ret;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T& t1) const{
		ConstIterator ret = begin();
		while(ret != end()){
			if(ret.thisNode->val.first == t1){
				return ret;
			}
			else{
				ret.thisNode = ret.thisNode->successor;
			}
		}
		return ret;
	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::~Map(){
		clear();
		free(dummy);
		free(rdummy);
	}

	template<typename Key_T, typename Mapped_T>
	const Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T& k) const{
		ConstIterator it = find(k);
		if(it == end()){
			throw std::out_of_range("Key value not in Map");
		}
		return it->second;
	}

	/*template<typename Key_T, typename Mapped_T>
	const Mapped_T& Map<Key_T, Mapped_T>::index(int index) const{
		if(index >= numElements){
			throw std::out_of_range("Index doesn't exist in Map");
		}
		return findIndex(root, index);
	}

	template<typename Key_T, typename Mapped_T>
	const Mapped_T& Map<Key_T, Mapped_T>::findIndex(Node* root, int index) const{
		int numLess = sizeOfSubtree(root->lChild); //numelements in subtree ya fuck
		if(numLess == index - 1){
			return root->val.second;
		}
		else if(numLess < index-1){
			return findIndex(root->rChild, )
		}
	}*/

	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T& k){
		Iterator it = find(k);
		if(it == end()){
			throw std::out_of_range("Key value not in Map");
		}
		return it->second;
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(Iterator it){
		erase(it->first);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(const Key_T& t1){
		Iterator finder = find(t1);
		if(finder == end()){
			throw std::out_of_range("Key value not in Map");
		}
		Node* toRemove = finder.thisNode;
		Node* balanceCheck = toRemove->parent;
		if(toRemove->lChild == NULL && toRemove->rChild == NULL){
			if(root == toRemove){
				root = dummy;
			}
			else{
				if(toRemove->parent->lChild == toRemove){
					toRemove->parent->lChild = NULL;
				}
				else{
					toRemove->parent->rChild = NULL;
				}
			}
		}
		else if(toRemove->rChild == NULL){
			Node* successor = toRemove->lChild;
			Node* parentNode = toRemove->parent;
			if(parentNode != NULL){
				if(parentNode->lChild == toRemove){
					parentNode->lChild = successor;
				}
				else{
					parentNode->rChild = successor;
				}
			}
			else{
				root = successor;
			}

			successor->parent = parentNode;
		}
		else if(toRemove->lChild == NULL){
			Node* successor = toRemove->rChild;
			Node* parentNode = toRemove->parent;
			if(parentNode != NULL){
				if(parentNode->rChild == toRemove){
					parentNode->rChild = successor;
				}
				else{
					parentNode->lChild = successor;
				}
			}
			else{
				root = successor;
			}
			successor->parent = parentNode;
		}
		else{
			Node* parentNode = toRemove->parent;
			Node* replacement = toRemove->rChild;
			Node* pOfReplacement = toRemove;
			while(replacement->lChild != NULL){
				pOfReplacement = replacement;
				replacement = replacement->lChild;
			}
			balanceCheck = pOfReplacement;
			if(pOfReplacement == toRemove){
				balanceCheck = replacement;
			}
			if(pOfReplacement != toRemove){
				pOfReplacement->lChild = replacement->rChild;
				if(replacement->rChild != NULL){replacement->rChild->parent = pOfReplacement;}
				replacement->rChild = toRemove->rChild; //dont want to change it for if pOf == nodeToDel
			}
			replacement->lChild = toRemove->lChild;
			replacement->parent = toRemove->parent;
			if(parentNode == NULL){root = replacement;}
			else if(parentNode->lChild == toRemove){
				parentNode->lChild = replacement;
			}
			else if(parentNode->rChild == toRemove){
				parentNode->rChild = replacement;
			}
			if(pOfReplacement != toRemove) {toRemove->rChild->parent = replacement;}
			toRemove->lChild->parent = replacement;

		}
		if(toRemove->predecessor != NULL) {toRemove->predecessor->successor = toRemove->successor;}
		if(toRemove->successor != NULL) {toRemove->successor->predecessor = toRemove->predecessor;}
		delete toRemove;
		numElements--;
		while(balanceCheck != NULL){
			if(sizeOfSubtree(balanceCheck->lChild) - sizeOfSubtree(balanceCheck->rChild) == 2){
				if(sizeOfSubtree(balanceCheck->lChild->lChild) > sizeOfSubtree(balanceCheck->lChild->rChild)){
					singleRotationRight(balanceCheck);
				}
				else{
					doubleRotationRight(balanceCheck);
				}
			}
			else if(sizeOfSubtree(balanceCheck->lChild) - sizeOfSubtree(balanceCheck->rChild) == -2){
				if(sizeOfSubtree(balanceCheck->rChild->rChild) > sizeOfSubtree(balanceCheck->rChild->lChild)){
					singleRotationLeft(balanceCheck);
				}
				else{
					doubleRotationLeft(balanceCheck);
				}
			}
			balanceCheck = balanceCheck->parent;
		}
	}



	//free standing operators
	template<typename Key_T, typename Mapped_T>
	bool operator==(const Map<Key_T, Mapped_T>& m1, const Map<Key_T, Mapped_T>& m2){
		if(m1.size() == m2.size()){
			typename Map<Key_T, Mapped_T>::ConstIterator it1 = m1.begin();
			typename Map<Key_T, Mapped_T>::ConstIterator it2 = m2.begin();
			while(it1 != m1.end()){
				if(!(it1->first == it2->first) || !(it1->second == it2->second)){
					return false;
				}
				++it1;
				++it2;
			}
			if(it1 == m1.end()){
				return true;
			}
		}
		return false;
	}

	template<typename Key_T, typename Mapped_T>
	bool operator!=(const Map<Key_T, Mapped_T>& m1, const Map<Key_T, Mapped_T>& m2){
		return !(m1 == m2);
	}

	template<typename Key_T, typename Mapped_T>
	bool operator<(const Map<Key_T, Mapped_T>& m1, const Map<Key_T, Mapped_T>& m2){
		typename Map<Key_T, Mapped_T>::ConstIterator it1 = m1.begin();
		typename Map<Key_T, Mapped_T>::ConstIterator it2 = m2.begin();
		while(it1 != m1.end() && it2 != m2.end()){
			if(it1->first < it2->first || (!(it1->first < it2->first) && it1->second < it2->second)){
				return true;
			}
			++it1;
			++it2;
		}
		if(it2 != m2.end()){
			return true;
		}
		return false;
	}
}