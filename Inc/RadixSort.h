/**
* @file RadixSort.h
* @author Matt Rudder
* @date Created Apr 28, 2006
*
* Wrapper class for radix sorting of arbitrary objects.
* Radix sorting allow us to break the theoretical lower bounds of sorting algorithms,
* as it does not rely on comparisons to do the sorting.
* Special thanks to the OGRE team (http://www.ogre3d.org) for the algorithm.
*/

/**
* Wrapper class for radix sorting of arbitrary objects.
*
* @author Matt Rudder
* @date Created Apr 28, 2006
* @date Modified Apr 28, 2006
*/
template<typename container_type, typename value_type, typename comp_type>
class CRadixSort
{
public:
	typedef typename container_type::iterator SortIterator;
protected:
	//! Size of the sort area.
	int m_nSortSize;
	//! Number of passes to sort by
	int m_nNumPasses;
	//! Alpha Pass Vars
	int m_nCounters[4][256];
	//! Beta Pass Vars
	int m_nOffsets[256];

	struct SSortValue
	{
		comp_type oKey;
		SortIterator itPos;
		SSortValue(void){}
		SSortValue(comp_type key, SortIterator it) : oKey(key), itPos(it) {}
	};

	//! Sorting storage
	std::vector<SSortValue> m_vSortArea1;
	std::vector<SSortValue> m_vSortArea2;
	std::vector<SSortValue>* m_pSource;
	std::vector<SSortValue>* m_pDestination;
	container_type m_oInitialCopy;

	void sortPass(int nIndex)
	{
		// Calculate offsets 
		m_nOffsets[0] = 0;
		for(int i = 1; i < 256; ++i)
			m_nOffsets[i] = m_nOffsets[i-1] + m_nCounters[nIndex][i-1];

		// Sort pass
		for(int i = 0; i < m_nSortSize; ++i)
		{
			unsigned char ucByte = getByte(nIndex, (*m_pSource)[i].oKey);
			(*m_pDestination)[m_nOffsets[ucByte]++] = (*m_pSource)[i];
		}
	}

	template<typename T>
	void finalPass(int nIndex, T oVal)
	{
		sortPass(nIndex);
	}

	// Special case int
	void finalPass(int nIndex, int nVal)
	{
		int nNeg = 0;
		for(int i = 128; i < 256; ++i)
			nNeg += m_nCounters[nIndex][i];

		// Calc offsets
		m_nOffsets[0] = nNeg;
		for(int i = 1; i < 128; ++i)
			m_nOffsets[i] = m_nOffsets[i-1] + m_nCounters[nIndex][i-1];

		m_nOffsets[128] = 0;
		for(int i = 129; i < 256; ++i)
			m_nOffsets[i] m_nOffsets[i-1] + m_nCounters[nIndex][i-1];

		unsigned char ucByte;
		for(int i = 0; i < m_nSortSize; ++i)
		{
			ucByte = getByte(nIndex, (*m_pSource)[i].oKey);
			(*m_pDestination)[m_nOffsets[ucByte]++] = (*m_pSource)[i];
		}
	}

	// Special case float
	void finalPass(int nIndex, float fValue)
	{
		int nNeg = 0;
		for(int i = 128; i < 256; ++i)
			nNeg += m_nCounters[nIndex][i];

		// Calc offsets
		m_nOffsets[0] = nNeg;
		for(int i = 1; i < 128; ++i)
			m_nOffsets[i] = m_nOffsets[i-1] + m_nCounters[nIndex][i-1];

		m_nOffsets[255] = m_nCounters[nIndex][255];
		for(int i = 254; i > 127; --i)
			m_nOffsets[i] = m_nOffsets[i+1] + m_nCounters[nIndex][i];

		unsigned char ucByte;
		for(int i = 0; i < m_nSortSize; ++i)
		{
			ucByte = getByte(nIndex, (*m_pSource)[i].oKey);
			if(ucByte > 127)
				(*m_pDestination)[--m_nOffsets[ucByte]] = (*m_pSource)[i];
			else
				(*m_pDestination)[m_nOffsets[ucByte]++] = (*m_pSource)[i];
		}
	}

	inline unsigned char getByte(int nIndex, comp_type oVal)
	{
		return ((unsigned char*)(&oVal))[nIndex];
	}
public:

	CRadixSort(void) {}
	virtual ~CRadixSort(void) 
	{
		m_vSortArea1.clear();
		m_vSortArea2.clear();
		m_oInitialCopy.clear();
	}

	template <typename func_type>
	void sort(container_type& container, func_type func)
	{
		if(container.empty())
			return;

		m_nSortSize = (int)container.size();
		m_vSortArea1.resize(container.size());
		m_vSortArea2.resize(container.size());

		m_oInitialCopy = container;

		m_nNumPasses = sizeof(comp_type);

		// Counter pass
		int p;
		for(p = 0; p < m_nNumPasses; ++p)
			memset(m_nCounters[p], 0, sizeof(int) * 256);

		// Perform alpha pass
		SortIterator i = m_oInitialCopy.begin();
		comp_type oPrev = func.operator()(*i);
		bool bNeedsSorting = false;
		for(int u = 0; i != m_oInitialCopy.end(); ++i, ++u)
		{
			// Get sort value
			comp_type oVal = func.operator()(*i);

			if(!bNeedsSorting && oVal < oPrev)
				bNeedsSorting = true;

			// Create sort entry
			m_vSortArea1[u].oKey = oVal;
			m_vSortArea1[u].itPos = i;

			// increase counters
			unsigned char ucByte;
			for(p = 0; p < m_nNumPasses; ++p)
			{
				ucByte = getByte(p, oVal);
				m_nCounters[p][ucByte]++;
			}

			oPrev = oVal;
		}

		// Return early is sorting is not needed.
		if(!bNeedsSorting)
			return;

		// Sort passes
		m_pSource = &m_vSortArea1;
		m_pDestination = &m_vSortArea2;

		for(p = 0; p < m_nNumPasses - 1; ++p)
		{
			sortPass(p);

			// Swap source and destination
			std::vector<SSortValue>* tmp = m_pSource;
			m_pSource = m_pDestination;
			m_pDestination = tmp;
		}

		finalPass(p, oPrev);

		// Copy back to container
		int c = 0;
		for(i = container.begin(); i != container.end(); ++i, ++c)
		{
			*i = *((*m_pDestination)[c].itPos);
		}
	}
};
