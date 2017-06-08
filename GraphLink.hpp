#pragma once

#include <stdio.h>
#include <vector>
#include <assert.h>

using namespace std;

//连接表结构
template<class W>
struct LinkEdge
{
	W _weight;
	size_t _srcIndex;
	size_t _desIndex;
	struct LinkEdge<W>* _pNext;

	LinkEdge(size_t srcIndex,size_t desIndex,const W& weight)
		: _srcIndex(srcIndex)
		, _desIndex(desIndex)
		, _weight(weight)
		, _pNext(NULL)
	{}
};

template <class V,class W>
class GraphLink
{
private:
	std::vector<V> _vex;
	std::vector<LinkEdge<W>*>_LinkTable;
	bool _isDirect;
public:
	GraphLink(const V* pvex, size_t size, bool isDirect = false)
		:_isDirect(isDirect)
	{
		_vex.resize(size);
		_LinkTable.resize(size,NULL);
		for (size_t idx = 0; idx < size; ++idx)
			_vex[idx] = pvex[idx];
	}

	//找结点的下标
	int GetIndexOfVertex(const V& v)
	{
		for (int i = 0; i < _vex.size(); ++i)
		{
			if (_vex[i] == v)
				return i;
		}
		assert(false);
		return 0;
	}

	void _AddEdge(size_t srcIdx, size_t desIdx,const W& weight)
	{
		//采用头插的方式
		LinkEdge<W>* pEdge = new LinkEdge<W>(srcIdx,desIdx,weight);
		pEdge->_pNext = _LinkTable[srcIdx];
		_LinkTable[srcIdx] = pEdge;
	}

	void AddEdge(const V& v1,const V& v2,const W& weight)
	{
		size_t srcIdx = GetIndexOfVertex(v1);
		size_t desIdx = GetIndexOfVertex(v2);

		assert(srcIdx != desIdx);

		//判断是否为有向图 有向图则只插入一次 无向图要插入两次
		_AddEdge(srcIdx, desIdx, weight);
		if (!_isDirect)
			_AddEdge(desIdx, srcIdx, weight);
	}

	//求某个节点的度 入度和出度的和
	int GetDev(const V& v)
	{
		int srcIdx = GetIndexOfVertex(v);

		//先求出度
		int Outcount = 0;
		LinkEdge<W>* pEdge = _LinkTable[srcIdx];
		while (pEdge)
		{
			Outcount++;
			pEdge = pEdge->_pNext;
		}

		//如果是无向图 再求入度
		int Incount = 0;
		if (!_isDirect)
		{
			for (size_t idx = 0; idx < _LinkTable.size(); ++idx)
			{
				if (srcIdx != idx)
				{
					//找到这个节点所在的链
					LinkEdge<W>* pEdge = LinkEdge[idx];
					while (pEdge)
					{
						//如果链的下一个为源 则入度加一
						if (pEdge->_desIndex == srcIdx)
							++Incount;
					}
				}
			}
			return Incount + Outcount;
		}
	}

	void Printf()
	{
		for (size_t idx = 0; idx < _LinkTable.size(); ++idx)
		{
			cout << idx;
			LinkEdge<W>* pEdge = _LinkTable[idx];
			while (pEdge)
			{
				cout << "--->" << pEdge->_desIndex << "[" << pEdge->_weight << "]";
				pEdge = pEdge->_pNext;
			}
			cout << "--->NULL" << endl;
		}
		cout << endl;
	}

	//广度优先遍历
	//用队列 访问过得结点需要变为true 
	void BFS(const V& v)
	{
		cout << "广度优先遍历"<<endl;
		vector<bool> visited(_vex.size(),false);
		_BFS(v, visited);

		//非连通结点的访问
		for (size_t i = 0; i < _vex.size();++i)
		{
			//他为false为没有访问
			if (!visited[i])
				_BFS(_vex[i],visited);
		}
		cout << "NULL"<<endl;
	}

	void DFS(const V& v)
	{
		cout << "深度优先遍历"<<endl;
		vector<bool> visited(_vex.size(),false);
		int idx = GetIndexOfVertex(v);
		_DFS(idx,visited);

		//非连通结点的访问
		for (size_t i = 0; i < _vex.size(); ++i)
		{
			//他为false为没有访问
			if (!visited[i])
				_BFS(_vex[i], visited);
		}
		cout << "NULL" << endl;
	}

protected:
	//访问过的节点需要变为true
	void _BFS(const V& v,vector<bool>& vistited)
	{
		queue<size_t> q;
		size_t idx = GetIndexOfVertex(v);
		q.push(idx);
		if (!vistited[idx])
		{
			cout << idx << "--->";
			vistited[idx] = true;
			q.pop();
			LinkEdge<W>* pCur = _LinkTable[idx];
			while (pCur)
			{
				q.push(pCur->_desIndex);
				pCur = pCur->_pNext;
			}
		}
	}

	void _DFS(size_t idx, vector<bool>& visited)
	{
		if (!visited[idx])
		{
			cout << idx << "--->";
			visited[idx] = true;

			LinkEdge<W>* pCur = _LinkTable[idx];
			while (pCur)
			{
				_DFS(pCur->_desIndex, visited);
				pCur = pCur->_pNext;
			}
		}
	}
};

void test()
{
	char buf[] = {'A','B','C','D'};
	GraphLink<char, int>Link(buf, 4, true);
	Link.AddEdge('A', 'B', 1);
	Link.AddEdge('B', 'C', 1);
	Link.AddEdge('C', 'D', 1);
	Link.AddEdge('A', 'D', 1);
	Link.Printf();
}
