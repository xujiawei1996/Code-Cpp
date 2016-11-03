#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

typedef int DateType;

struct Node
{
	Node(const DateType &data = 0)
		:_data(data)
		,_pNext(NULL)
		,_pPre(NULL)
	{}
	

	DateType _data;
	Node *_pNext;
	Node *_pPre;
};

class List
{
public:
	List()
		:_pHead(NULL)
		,_pTail(NULL)
		,_size(0)
	{}

	
	List(DateType arr[],size_t size)
		:_size(size)//这里换成size也是对的
	{
		for (size_t idx = 0; idx<size ; ++idx)
		{
			PushBack(arr[idx]);
		}
	}
	
	List(const List& l)
		:_pHead(NULL)
		,_pTail(NULL)
		,_size(0)
	{	
		Node *pCurNode = l._pHead;
		while (pCurNode)
		{
			PushBack(pCurNode->_data);
			pCurNode = pCurNode->_pNext;
		}
	}

	List& operator=(const List &l)
	{
		if (&l != this)
		{
			Clear();
			Node *pCurNode = l._pHead;
			while (pCurNode)
			{
				PushBack(pCurNode->_data);
				pCurNode = pCurNode->_pNext;
			}
		}
		return *this;
	}

	~List()
	{
		Clear();
	}


	Node* BuyNode(const DateType& data)
	{
		return new Node(data);
	}

	//尾插 
	//两种情况 如果为空 如果不为空
	void PushBack(const DateType& data)
	{
		if (NULL == _pHead)
		{
			_pHead = _pTail = BuyNode(data);
		}
		else 
		{
			Node* pNewNode = BuyNode(data);
			_pTail->_pNext = pNewNode;
			pNewNode->_pPre = _pTail;
			_pTail = pNewNode;
		}
		_size++;
	}

	//尾删
	//三种情况 一种为空 一种只有一个结点 一种有大于两个的结点
	void PopBack()
	{
		if (NULL == _pHead)
		{
			return;
		}
		else if (_pHead == _pTail)
		{
			delete _pHead;
			_pHead = _pTail = NULL;
		}
		else
		{
			_pTail = _pTail->_pPre;
			delete _pTail->_pNext;
			_pTail->_pNext = NULL;
		}
		--_size;
	}
	
	//头插
	//两种情况 一种为空 一种不为空
	void PushFront(const DateType &data)
	{
		if(_pHead == NULL)
		{
			_pHead = _pTail = BuyNode(data);
		}
		else
		{
			Node *pNewNode = BuyNode(data);
			pNewNode->_pNext = _pHead;
			_pHead->_pPre = pNewNode;
			_pHead = pNewNode;
		}
		_size++;
	}

	//头删
	//三种情况 第一种情况链表为空 第二种情况只有一个结点 第三种情况
	void PopFront()
	{
		if (_pHead == NULL)
		{
			return ;
		}
		else if (_pHead == _pTail)
		{
			delete _pHead;
			_pHead = _pTail = NULL;
		}
		else
		{
			_pHead = _pHead->_pNext;
			delete _pHead->_pPre;
			_pHead->_pPre = NULL;
		}
		--_size;
	}
	
	//清理
	void Clear()
	{
		Node *pCurNode = _pHead;
		Node *pCurNext = _pHead;
		while (pCurNode)
		{
			pCurNext = pCurNode->_pNext;
			delete pCurNode;
			pCurNode = pCurNext;
		}
		_size = 0;
		_pHead = _pTail = NULL;
	}

	//查找一个结点
	Node* Find(const DateType &data)const
	{
		Node *pCurNode = _pHead;
		while (pCurNode)
		{
			if (pCurNode->_data == data)
			{
				return pCurNode;
			}
			else
				pCurNode = pCurNode->_pNext;
		}
		return NULL;
	}

	//插入
	void Insert(Node *pos,DateType &data)
	{
		if (pos == NULL)
		{
			return;
		}
		if(pos == _pTail)
		{
			PushBack(data);	
			++_size;
		}
		else
		{
			Node *pNewNode = BuyNode(data);
			pNewNode->_pNext = pos->_pNext;
			pos->_pNext->_pPre = pNewNode;
			pos->_pNext = pNewNode;
			pNewNode->_pPre = pos;
			++_size;
		}
	}


	//删除某一个结点
	void Erase(Node *pos)
	{
		if (NULL == pos)
		{
			return;
		}
		if (pos == _pTail)
		{
			PopBack();
		}
		else if (pos == _pHead)
		{
			PopFront();
		}
		else
		{
			pos->_pPre->_pNext = pos->_pNext;
			pos->_pNext->_pPre = pos->_pPre;
			delete pos;
		}
		--_size;
	}

	bool Empty()const
	{
		return 0==_size;
	}

	size_t size()const
	{
		return _size;
	}

	Node &Front()
	{
		return *_pHead;
	}

	const Node &Front() const
	{
		return *_pHead;
	}

	Node &Back()
	{
		return *_pTail;
	}

	const Node &Back() const
	{
		return *_pTail;
	}

private:
	Node *_pHead;
	Node *_pTail;
	size_t _size;
};