#pragma once
//连接图
#include <stdio.h>
#include <vector>
#include <assert.h>
using namespace std;

template <class V,class W>
class GraphMatrix
{
private:
	std::vector<V> _ver;//存放节点 
	std::vector<vector<W> > _edge;//存放节点的权值
	bool _IsDirect;

public:
	GraphMatrix(const V* pVer, size_t Size,bool isDirect = false)
		:_IsDirect(false)
	{
		_ver.resize(Size);
		_edge.resize(Size);
		for (int idx = 0; idx < Size; ++idx)
		{
			_ver[idx] = pVer[idx];
			_edge[idx].resize(Size);
		}
	}

	//找结点的下标
	int GetIndexOfVertex(const V& v)
	{
		for (int i = 0; i < _ver.size(); ++i)
		{
			if (_ver[i] == v)
				return i;
		}
		assert(false);
		return 0;
	}

	//添加边
	void AddEdge(const V& v1,const V& v2,const W& weight)
	{
		int row = GetIndexOfVertex(v1);
		int col = GetIndexOfVertex(v2);

		_edge[row][col] = weight;
		if(!_IsDirect)//无向图
		{
			_edge[col][row] = weight;
		}
	}

	void Printf()
	{
		for (int i = 0; i < _edge.size();++i)
		{
			for (int j = 0; j < _edge[i].size(); ++j)
			{
				cout << _edge[i][j] << "   ";
			}
			cout << endl;
		}
	}
};

void test()
{
	int arr[] = { 0, 1, 2};
	GraphMatrix<int, int> m(arr, 3, true);
	m.AddEdge(0, 1, 1);
	m.AddEdge(1, 0, 1);
	m.AddEdge(1, 2, 1);
	m.Printf();
}
