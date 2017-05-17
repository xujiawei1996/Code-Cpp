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
		, _color(RED)//初始化为红色结点  
	{}

	V _value;
	K _key;
	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	Color _color;
};

template < class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:

	RBTree()
		:_pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		//判断根节点是否为空  
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			return false;
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
				}
			}
		}
		//最后要把根节点换成黑色的  
		_pRoot->_color = BLACK;
		return true;
	}

	bool CheckRBTree()
	{
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
				cout << "违反两个黑色结点应该相同原则" << endl;
				return false;
			}
		}

		return _CheckRBTree(pRoot->_pLeft, blackCount, k) && _CheckRBTree(pRoot->_pRight, blackCount, k);
	}

	void InOrder()
	{
		_InOrder(_pRoot);
	}

private:

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

		if (pRoot->_pParent == NULL)
			_pRoot = pRoot;
		else if (pRoot->_pParent->_key > pRoot->_key)
			pRoot->_pParent->_pLeft = pRoot;
		else if (pRoot->_pParent->_key < pRoot->_key)
			pRoot->_pParent->_pRight = pRoot;
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

		if (pRoot->_pParent == NULL)
			_pRoot = pRoot;
		else if (pRoot->_pParent->_key > pRoot->_key)
			pRoot->_pParent->_pLeft = pRoot;
		else if (pRoot->_pParent->_key < pRoot->_key)
			pRoot->_pParent->_pRight = pRoot;
	}

private:
	Node* _pRoot;
};
