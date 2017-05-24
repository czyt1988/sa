#ifndef SORTINDEX_H
#define SORTINDEX_H
#include <vector>
#include <algorithm>
//专门用来排序的类,T必须是有重载大于小于号的类型
template<typename T>
class CSortIndex
{
public:
    CSortIndex(){}
    ~CSortIndex(){}
private:
    template<typename T>
    struct struct_dataIndex
    {
        unsigned nIndex;
        T ddata;
    };
public:
    //比较函数，用来比较a和b的值,用于qsort排序的
    static int CompareT(const struct_dataIndex<T>& A,const struct_dataIndex<T>& B);
public:
    //排序，根据传入的指针p,排序，把排好的顺序的原始索引返回sortedIndex
    //如double[3]{1,5,3,6},返回的sortedIndex是{0,2,1,3}，就是说返回原来数值的序号的顺序
    //次函数不改变p的内容
    bool SortIndex(T* p,unsigned nLength,std::vector<unsigned>& vSortedIndex);
    ///
    /// \brief GetTop 获取最大的n个数值和最小的n个数值
    /// \param p 数组指针
    /// \param nLength 数组长度
    /// \param nTopCount 需要获取的最大值数目
    /// \param vTop 排序后最大的n个值
    /// \param nLowCount 需要获取的最小值数目
    /// \param vLow 排序后最小的n个值
    ///
    /// 函数将改变p的内容，使用前先复制
    void GetTopLow(T* p
                   ,unsigned nLength
                   ,unsigned nLowCount
                   ,std::vector<T>& vLow
                   ,unsigned nTopCount
                   ,std::vector<T>& vTop);
    ///
    /// \brief GetTopLowAndIndex 获取最大的n个数值和最小的n个数值,同时获得它们的原来数组的序号位置
    /// \param p 数组指针
    /// \param nLength 数组长度
    /// \param nTopCount 需要获取的最大值数目
    /// \param vTop 排序后最大的n个值
    /// \param vTopIndex 排序后最大的n个值原来所在数组的索引
    /// \param nLowCount 需要获取的最小值数目
    /// \param vLow 排序后最小的n个值
    /// \param vTopIndex 排序后最小的n个值原来所在数组的索引
    ///
    bool GetTopLowWidthIndex(const T* p
                   ,unsigned nLength
                   ,unsigned nLowCount
                   ,std::vector<T>& vLow
                   ,std::vector<unsigned>& vLowIndex
                   ,unsigned nTopCount
                   ,std::vector<T>& vTop
                   ,std::vector<unsigned>& vTopIndex);
	///
	/// \brief GetTopLowAndIndex 获取最大的n个数值和最小的n个数值,同时获得它们的原来数组的序号位置
	/// \param p 数组指针
	/// \param nLength 数组长度
	/// \param nTopCount 需要获取的最大值数目
	/// \param vTop 排序后最大的n个值
	/// \param vTopIndex 排序后最大的n个值原来所在数组的索引
	/// \param nLowCount 需要获取的最小值数目
	/// \param vLow 排序后最小的n个值
	/// \param vTopIndex 排序后最小的n个值原来所在数组的索引
	///
	bool GetTopLowWidthIndex(const std::vector<T>& p
		,unsigned nLowCount
		,std::vector<T>& vLow
		,std::vector<unsigned>& vLowIndex
		,unsigned nTopCount
		,std::vector<T>& vTop
		,std::vector<unsigned>& vTopIndex);
};

//排序，根据传入的指针p,排序，把排好的顺序返回sortedIndex
//如double[3]{1,5,3,6},返回的sortedIndex是{0,2,1,3}，就是说返回原来数值的序号的顺序
template <typename T>
bool CSortIndex<T>::SortIndex(T* p,unsigned nLength,std::vector<unsigned>& vSortedIndex)
{
    struct_dataIndex<T>* pdataIndex = nullptr;
    try
    {
        pdataIndex = new struct_dataIndex<T>[nLength];
        for (unsigned i(0);i<nLength;++i)
        {
            pdataIndex[i].nIndex = i;
            pdataIndex[i].ddata = p[i];
        }
        //快速排序
        std::sort(pdataIndex,pdataIndex+nLength,CSortIndex<T>::CompareT);
        vSortedIndex.clear();
        vSortedIndex.resize(nLength);
        for (unsigned i(0);i<nLength;++i)
        {
            vSortedIndex[i] = pdataIndex[i].nIndex;
        }
    }
    catch (...)
    {
        if(pdataIndex)
            delete[] pdataIndex;
        return false;
    }
    if(pdataIndex)
        delete[] pdataIndex;
    return true;
}

//比较函数，用来比较a和b的值
template <typename T>
int CSortIndex<T>::CompareT(const struct_dataIndex<T>& A,const struct_dataIndex<T>& B)
{
    return A.ddata < B.ddata;
}

template <typename T>
void CSortIndex<T>::GetTopLow(T* p
	,unsigned nLength
	,unsigned nLowCount
	,std::vector<T>& vLow
	,unsigned nTopCount
	,std::vector<T>& vTop)
{
    vTop.resize(nTopCount);
    vLow.resize(nTopCount);
    //快速排序 -- 默认升序
    std::sort(p,p+nLength);
    nTopCount = (nTopCount < nLength ? nTopCount : nLength);
	nLowCount = (nLowCount < nLength ? nLowCount : nLength);
    for(int i(0);i<nLowCount;++i){
        vLow[i] = (p[i]);
    }
	int nTopIndex = nLength - nTopCount;
	for(int i(nLength-1),j(0);i>=nTopIndex;--i,++j)//此处必须用int，否则用unsigned会死循环，因为unsigned 0 减减后还是正数
	{
		vTop[j] = (p[i]);
	}
}

template <typename T>
bool CSortIndex<T>::GetTopLowWidthIndex(const T* p
	,unsigned nLength
	,unsigned nLowCount
	,std::vector<T>& vLow
	,std::vector<unsigned>& vLowIndex
	,unsigned nTopCount
	,std::vector<T>& vTop
	,std::vector<unsigned>& vTopIndex)
{
    struct_dataIndex<T>* pdataIndex = nullptr;
    try
    {
        pdataIndex = new struct_dataIndex<T>[nLength];
        for (unsigned i(0);i<nLength;++i)
        {
            pdataIndex[i].nIndex = i;
            pdataIndex[i].ddata = p[i];
        }
        //快速排序
        std::sort(pdataIndex,pdataIndex+nLength,CSortIndex<T>::CompareT);

        vTop.resize(nTopCount);
        vLow.resize(nLowCount);
        vTopIndex.resize(nTopCount);
        vLowIndex.resize(nLowCount);
        nTopCount = (nTopCount < nLength ? nTopCount : nLength);
        nLowCount = (nLowCount < nLength ? nLowCount : nLength);
        for (int i(0);i<nLowCount;++i)
        {
            vLow[i] = (pdataIndex[i].ddata);
            vLowIndex[i] = (pdataIndex[i].nIndex);
        }
		int nTopIndex = nLength - nTopCount;
        for(int i(nLength-1),j(0);i>=nTopIndex;--i,++j)//此处必须用int，否则用unsigned会死循环，因为unsigned 0 减减后还是正数
        {
            vTop[j] = (pdataIndex[i].ddata);
            vTopIndex[j] = (pdataIndex[i].nIndex);
        }
    }
    catch (...)
    {
        if(pdataIndex)
            delete[] pdataIndex;
        return false;
    }
    if(pdataIndex)
        delete[] pdataIndex;
    return true;
}

template <typename T>
bool CSortIndex<T>::GetTopLowWidthIndex(const std::vector<T>& p
	,unsigned nLowCount
	,std::vector<T>& vLow
	,std::vector<unsigned>& vLowIndex
	,unsigned nTopCount
	,std::vector<T>& vTop
	,std::vector<unsigned>& vTopIndex)
{
	struct_dataIndex<T>* pdataIndex = nullptr;
	try
	{
		unsigned int nLength = p.size();
		pdataIndex = new struct_dataIndex<T>[nLength];
		for (unsigned int i(0);i<nLength;++i)
		{
			pdataIndex[i].nIndex = i;
			pdataIndex[i].ddata = p[i];
		}
		//快速排序
		std::sort(pdataIndex,pdataIndex+nLength,CSortIndex<T>::CompareT);

		vTop.resize(nTopCount);
		vLow.resize(nLowCount);
		vTopIndex.resize(nTopCount);
		vLowIndex.resize(nLowCount);
		nTopCount = (nTopCount < nLength ? nTopCount : nLength);
		nLowCount = (nLowCount < nLength ? nLowCount : nLength);
		for (int i(0);i<nLowCount;++i)
		{
			vLow[i] = pdataIndex[i].ddata;
			vLowIndex[i] = pdataIndex[i].nIndex;
		}
		int nTopIndex = nLength - nTopCount;
		for(int i(nLength-1),j(0);i>=nTopIndex;--i,++j)//此处必须用int，否则用unsigned会死循环，因为unsigned 0 减减后还是正数
		{
			vTop[j] = (pdataIndex[i].ddata);
			vTopIndex[j] = (pdataIndex[i].nIndex);
		}
	}
	catch (...)
	{
		if(pdataIndex)
			delete[] pdataIndex;
		return false;
	}
	if(pdataIndex)
		delete[] pdataIndex;
	return true;

}
#endif // SORTINDEX_H
