#pragma once

#include <atlcoll.h>
#include <atlstr.h>
template< typename K, typename V, class KTraits = CElementTraits< K >, class VTraits = CElementTraits< V > >
class MBHashMap : public CAtlMap< K, V, KTraits, VTraits >
{
public:
	class value_type
	{
	public:
		POSITION m_Position;
		MBHashMap* m_pOwner;

		__declspec(property(get=GetFirst)) const K& first;
		__declspec(property(get=GetSecond,put=SetSecond)) V& second;

		inline const K& GetFirst() const { return m_pOwner->GetKeyAt(m_Position); }
		inline V& GetSecond() const { return m_pOwner->GetValueAt(m_Position); }
		inline void SetSecond(const V& val) const { return m_pOwner->SetValueAt(m_Position,val); }
	};

	class iterator;
	friend iterator;
	class const_iterator;
	friend const_iterator;

	class const_iterator:public value_type
	{
	public:
		inline const_iterator() {}
		inline const_iterator(const iterator& src):value_type(src) {}

		const_iterator& operator++()
		{
			// preincrement
			m_pOwner->GetNext(m_Position);
			return (*this);
		}

		const_iterator operator++(int)
		{
			// postincrement
			const_iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		inline bool operator==(const const_iterator& a) const 
		{
			return m_Position==a.m_Position && m_pOwner==a.m_pOwner;
		}

		inline bool operator!=(const const_iterator& a) const 
		{
			return !(m_Position==a.m_Position && m_pOwner==a.m_pOwner);
		}

		inline const value_type& operator*() const 
		{
			return *this;
		}

		inline const value_type* operator->() const 
		{
			return this;
		}
	};

	class iterator:public value_type
	{
	public:
		inline iterator() {}
		inline iterator(const const_iterator& src):value_type(src) {}

		inline bool operator==(const iterator& a) const 
		{
			return m_Position==a.m_Position && m_pOwner==a.m_pOwner;
		}

		inline bool operator!=(const iterator& a) const 
		{
			return !(m_Position==a.m_Position && m_pOwner==a.m_pOwner);
		}

		inline iterator& operator++()
		{	
			// preincrement
			m_pOwner->GetNext(m_Position);
			return (*this);
		}

		inline iterator operator++(int)
		{	
			// postincrement
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		inline const value_type& operator*() const 
		{
			return *this;
		}

		inline const value_type* operator->() const 
		{
			return this;
		}
	};
	MBHashMap( UINT nBins = 17, float fOptimalLoad = 0.75f, float fLoThreshold = 0.25f, float fHiThreshold = 2.25f, UINT nBlockSize = 10 ) : 
	CAtlMap( nBins, fOptimalLoad, fLoThreshold, fHiThreshold, nBlockSize )
	{}

	MBHashMap( const MBHashMap& a) 
	{
		RemoveAll();

		for(iterator i=a.begin();i!=a.end();i++)
		{
			SetAt(i->first,i->second);
		}
	}

	MBHashMap& operator=( const MBHashMap& a)
	{
		RemoveAll();

		for(iterator i=a.begin();i!=a.end();i++)
		{
			SetAt(i->first,i->second);
		}

		return *this;
	}


	inline iterator begin() 
	{
		iterator ret;
		ret.m_Position = GetStartPosition();
		ret.m_pOwner=this;

		return ret;
	}

	inline iterator end()
	{
		iterator ret;
		ret.m_Position=NULL;
		ret.m_pOwner=this;

		return ret;
	}

	inline value_type front() 
	{
		//		if(Count==0)
		//			return *begin();

		return *begin();
	}

	inline const_iterator begin() const
	{
		const_iterator ret;
		ret.m_Position = GetStartPosition();
		ret.m_pOwner = (MBHashMap*)this;

		return ret;
	}

	inline const_iterator end() const
	{
		const_iterator ret;
		ret.m_Position=NULL;
		ret.m_pOwner=const_cast<MBHashMap*>(this);

		return ret;
	}

	inline iterator erase(iterator iter)
	{
		if(iter.m_pOwner!=this) {
			return iter;
		}
		iterator ret = iter;
		ret++;
		RemoveAtPos(iter.m_Position);

		return ret;
	}

	inline void erase( const K &key )
	{
		RemoveKey( key );
	}

	inline iterator find(const K& key)
	{
		ATL::CAtlMap< K, V, KTraits, VTraits >::CPair* pair = CAtlMap<K,V,KTraits,VTraits>::Lookup( key );
		iterator ret;
		ret.m_pOwner=this;

		if( pair!=NULL ) {
			ret.m_Position=POSITION(pair);
		}
		else {
			ret.m_Position=NULL;
		}

		return ret;
	}

	inline const_iterator find(const K& key) const
	{
		const ATL::CAtlMap< K, V, KTraits, VTraits >::CPair* pair = CAtlMap<K,V,KTraits,VTraits>::Lookup( key );
		const_iterator ret;
		ret.m_pOwner=(MBHashMap*)this;
		if( pair!=NULL ) {
			ret.m_Position=POSITION(pair);
		}
		else {
			ret.m_Position=NULL;
		}
		return ret;
	}

	inline void clear()
	{
		RemoveAll();
	}

	inline void insert( const std::pair< K, V >& obj )
	{
		SetAt( obj.first, obj.second );
	}

	inline bool empty() const
	{
		return IsEmpty();
	}

	inline size_t size() const
	{
		return CAtlMap::GetCount();
	}

private:
};

typedef ATL::CString MBString;
typedef ATL::CStringA MBStringA;