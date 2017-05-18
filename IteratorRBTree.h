#pragma once

#include <iostream>
using namespace std;

enum Color
{
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode
{
public:
	RBTreeNode(const V&value, const K& key)
		:_value(value)
		, _key(value)
		, _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _color(RED)
	{}

	V _value;
	K _key;
	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	Color _color;
};

template<class K,class V,class Ref,class Ptr>
class Iterator
{
	typedef Iterator<K, V, Ref, Ptr> Self;
	typedef RBTreeNode<K, V> Node;

public:
	Iterator()
		:_pNode(NULL)
	{}

	Iterator(const Self& it)
		:_pNode(it._pNode)
	{}

	Iterator(Node* node)
		:_pNode(node)
	{}

	Ref operator*()
	{
		return _pNode->_key;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	//前置++
	Self& operator++()
	{
		Increment();
		return *this;
	}

	//后置++
	Self operator++(int)
	{
		Self temp(*this);
		Increment();
		return temp;
	}

	//前置--
	Self& operator--()
	{
		Decrement();
		return *this;
	}

	//后置--
	Self operator--(int)
	{
		Self temp(*this);
		Decrement();
		return temp;
	}

	bool operator!=(const Self& sf)
	{
		return _pNode != sf._pNode;
	}

	bool operator==(const Self& sf)
	{
		return _pNode == sf._pNode;
	}
protected:
	//指针自增操作（按照中序遍历的结果输出）
	//如图1所示
	void Increment()
	{
		//如果此结点的右子树存在 则应该找右子树最小的结点 也就为右子树最左边的结点
		if (_pNode->_pRight)
		{
			_pNode = _pNode->_pRight;
			while (_pNode->_pLeft)
				_pNode = _pNode->_pLeft;
		}
		//若不存在 
		//若此结点为父亲结点的右结点 则应该一直向上找（因为上面的都比下面的小 不为++的结果）
		//否则 为左节点时 此时的父亲结点比pNode大 此时父亲结点就为++的下一个节点
		else
		{
			Node* pParent = _pNode->_pParent;
			while (pParent->_pRight == _pNode)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			//这里要特殊处理 因为如果此时是跟 跟的右子树不存在 则应该再加就赋给跟的parent 如图3
			if (pParent->_pRight != _pNode)
				_pNode = pParent;
		}
	}

	void Decrement()
	{
		//如果是end()为起始结点的话 起始结点自减就应该为根节点右子树的第一个结点 
		if (_pNode->_color == RED && _pNode->_pParent->_pParent == _pNode)
			_pNode = _pNode->_pRight;
		//否则此结点存在左子树 就应该找左子树最右边的结点 找左子树最大的结点
		else if (_pNode->_pLeft)
		{
			_pNode = _pNode->_pLeft;
			while (_pNode->_pRight)
				_pNode = _pNode->_pRight;
		}
		//否则 不是根结点也没有左子树 如图4 就应该找此子树的根节点
		else
		{
			Node* pParent = _pNode->_pParent;
			while (pParent->_pLeft == _pNode)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			//此时就不用再判断如上加加的条件了 因为走到这一步pParent一定是减减后最小的
			_pNode = pParent;
		}
	}

private:
	Node* _pNode;
};

template < class K, class V>
class RBTree
{
	typedef Iterator<K, V, K&, V*> Iterator;
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
	{
		_pHead = new Node(K(),V());
		_pHead->_color = RED;
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = NULL;
	}

	bool Insert(const K& key, const V& value)
	{
		//判断根节点是否为空
		Node* _pRoot = GetRoot();
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key,value);
			_pRoot->_pParent = _pHead;
			_pHead->_pParent = _pRoot;
			_pRoot->_color = BLACK;
			return true;
		}
			Node* pCur = _pRoot;
			Node* pParent = NULL;
			//找插入位置
			while (pCur)
			{
				if (key > pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_pRight;
				}
				else if (key < pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_pLeft;
				}
				else
					return false;
			}

			//插入结点
			pCur = new Node(key, value);
			if (key < pParent->_key)
				pParent->_pLeft = pCur;
			else if (key > pParent->_key)
				pParent->_pRight = pCur;

			pCur->_pParent = pParent;

			//对结点的颜色进行处理
			//从下往上修改颜色 到根节点且根节点为红结束
			while (_pRoot != pCur && pParent->_color == RED)
			{
				Node* grandFather = pParent->_pParent;
				if (pParent == grandFather->_pLeft)
				{
					Node* uncle = grandFather->_pRight;
					//当前插得为红，双亲为红，叔叔也为红
					//此时应该把祖先变为红，把双亲叔叔变为黑
					if (uncle && uncle->_color == RED)
					{
						grandFather->_color = RED;
						pParent->_color = BLACK;
						uncle->_color = BLACK;

						pCur = grandFather;
						pParent = pCur->_pParent;
					}
					//否则叔叔不存在 或者叔叔为黑
					else
					{
						//如果cur在父母的右边 则左单旋 再右单旋
						if (pParent->_pRight == pCur)
						{
							RotateLeft(pParent);
							std::swap(pParent, pCur);
						}
						//否则只右单旋
						pParent->_color = BLACK;
						grandFather->_color = RED;
						RotateRight(grandFather);
						break;
					}
				}
				//否则父母结点在祖先节点的右边
				else
				{
					Node* uncle = grandFather->_pLeft;
					if (uncle && uncle->_color == RED)
					{
						pParent->_color = BLACK;
						uncle->_color = BLACK;
						grandFather->_color = RED;

						pCur = grandFather;
						pParent = pCur->_pParent;
					}
					else
					{
						if (pCur == pParent->_pLeft)
						{
							RotateRight(pParent);
							std::swap(pParent, pCur);
						}
						grandFather->_color = RED;
						pParent->_color = BLACK;
						RotateLeft(grandFather);	
						break;
					}
				}
			}
		//旋转后要把根节点重新获取一下
		_pRoot = GetRoot();

		//最后要把根节点换成黑色的
		_pRoot->_color = BLACK;
		_pHead->_pLeft = GetMin();
		_pHead->_pRight = GetMax();
		return true;
	}

	bool CheckRBTree()
	{
		Node* _pRoot = GetRoot();
		if (_pRoot == NULL)
			return true;

		if (_pRoot->_color == RED)
		{
			cout << "根为红色不满足" << endl;
			return false;
		}

		//计算黑色结点的个数 应该每一条路径上数目都相同
		int BlackCount = 0;
		Node* pCur = _pRoot;
		//只走最左边的那一条路
		while (pCur)
		{
			if (pCur->_color == BLACK)
				BlackCount++;
			pCur = pCur->_pLeft;
		}
		int k = 0;
		return _CheckRBTree(_pRoot, BlackCount, k);
	}

	bool _CheckRBTree(Node* pRoot, const size_t blackCount, size_t k)
	{
		if (pRoot == NULL)
			return true;

		//如果两个连续的红色 就违反规则
		Node* Parent = pRoot->_pParent;
		if (Parent && Parent->_color == RED && pRoot->_color == RED)
		{
			cout << "两个红色不能相连接" << endl;
			return false;
		}

		//判断是否k=blackCount
		//如果此时为黑色结点 k要++
		if (pRoot->_color == BLACK)
			k++;

		if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
		{
			if (k != blackCount)
			{
				cout << "违反两个黑色结点相同原则" << endl;
				return false;
			}
		}
		return _CheckRBTree(pRoot->_pLeft, blackCount, k) && _CheckRBTree(pRoot->_pRight, blackCount, k);
	}

	void InOrder()
	{
		cout << "中序遍历：";
		Node* _pRoot = GetRoot();
		_InOrder(_pRoot);
	}

	Iterator Begin()
	{
		return Iterator(_pHead->_pLeft);
	}

	Iterator End()
	{
		return Iterator(_pHead);
	}

private:
	//获取根
	Node*& GetRoot()
	{
		return _pHead->_pParent;
	}

	Node* GetMin()
	{
		Node* pRoot = _pHead->_pParent;
		while (pRoot->_pLeft)
		{
			pRoot = pRoot->_pLeft;
		}
		return pRoot;
	}

	Node* GetMax()
	{
		Node* pRoot = _pHead->_pParent;
		while (pRoot->_pRight)
		{
			pRoot = pRoot->_pRight;
		}
		return pRoot;
	}

	void _InOrder(Node* pRoot)
	{
		if (pRoot == NULL)
			return;
		_InOrder(pRoot->_pLeft);
		cout << pRoot->_key << "  ";
		_InOrder(pRoot->_pRight);
	}

	void RotateLeft(Node*& pRoot)
	{
		Node* SubR = pRoot->_pRight;
		Node* SubRL = SubR->_pLeft;
		pRoot->_pRight = SubRL;

		if (SubRL)
			SubRL->_pParent = pRoot;
		SubR->_pLeft = pRoot;
		SubR->_pParent = pRoot->_pParent;

		pRoot->_pParent = SubR;
		pRoot = SubR;

		if (pRoot->_pParent == _pHead)
			_pHead->_pParent = pRoot;
		else
		{
			if (pRoot->_pParent->_key > pRoot->_key)
			pRoot->_pParent->_pLeft = pRoot;
			else
			pRoot->_pParent->_pRight = pRoot;
		}
	}

	void RotateRight(Node*& pRoot)
	{
		Node* SubL = pRoot->_pLeft;
		Node* SubLR = SubL->_pRight;
		pRoot->_pLeft = SubLR;
		if (SubLR)
			SubLR->_pParent = pRoot;
		SubL->_pRight = pRoot;
		SubL->_pParent = pRoot->_pParent;

		pRoot->_pParent = SubL;
		pRoot = SubL;

		if (pRoot->_pParent == _pHead)
			_pHead->_pParent = pRoot;
		else{
			if (pRoot->_pParent->_key > pRoot->_key)
				pRoot->_pParent->_pLeft = pRoot;
			else
				pRoot->_pParent->_pRight = pRoot;
		}
	}

private:
	Node* _pHead;
	//int _Size;
};
