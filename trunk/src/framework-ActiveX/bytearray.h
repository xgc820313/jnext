#ifndef _BYTEARRAY_H_
#define _BYTEARRAY_H_
#include <afxcoll.h>

class tByteArray : public CByteArray
{
	public:

	tByteArray& operator=( tByteArray& rhs )
	{
		RemoveAll();
		Append( rhs.GetData(), rhs.GetSize() );
		return *this;
	}

	void InitSize( int nInitSize )
	{
		RemoveAll();
		/*delete[] (BYTE*)m_pData;
		m_pData = NULL;*/
		m_pData = (BYTE*) new BYTE[nInitSize * sizeof(BYTE)];
		memset(m_pData, 0, nInitSize * sizeof(BYTE));  // zero fill
		m_nMaxSize = nInitSize;
	}
	
	INT_PTR Append( BYTE* pBuf, INT_PTR nLen )
	{
		ASSERT_VALID(this);
	
		INT_PTR nOldSize = m_nSize;
		SetSize( m_nSize + nLen );
	
		memcpy(m_pData + nOldSize, pBuf, nLen * sizeof(BYTE));
	
		return nOldSize;
	}
	
	INT_PTR ShiftLeft( INT_PTR nBytes )
	{
		INT_PTR newlen = m_nSize - nBytes;
		ASSERT( newlen >= 0 );
		memmove( m_pData , m_pData + nBytes, newlen );
		m_nSize = newlen;
		return newlen;
	}
};

#endif
