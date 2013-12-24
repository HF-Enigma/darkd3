#ifndef _DS_OBJECT_H_ 
#define _DS_OBJECT_H_

#include "DSMemory.h"

namespace ds_utils
{
    namespace ds_memory
    {
        template<class _Type, size_t _N = 1>
        class CDSObject : public CDSDataBlock
        {
        public:
            typedef _Type _TypeN[_N];

        public:
            CDSObject(void):
                CDSDataBlock(),
                m_pObjArray(0),
                m_bVarSize(false)
            {
            };

            CDSObject(_Type* ptr):
                CDSDataBlock(),
                m_pObjArray(0),
                m_bVarSize(false)
            {
                Attach(ptr);
            };

            CDSObject(const CDSObject<_Type>& obj):
                CDSDataBlock(),
                m_pObjArray(0),
                m_bVarSize(false)
            {
                Assign(obj);
            };

            template<class _Type2, size_t _M>
            CDSObject(const CDSObject<_Type2, _M>& obj):
                CDSDataBlock(),
                m_pObjArray(0),
                m_bVarSize(false)
            {
                Assign<_Type2, _M>(obj);
            };

            virtual ~CDSObject(void)
            {
                Free();
            };

            _TypeN* Attach(_TypeN* pPointer)
            {
                assert(m_pObjArray == 0);

                m_pObjArray = reinterpret_cast<_TypeN*>(pPointer);
                m_pData = reinterpret_cast<unsigned char*>(pPointer);

                m_size = sizeof(*pPointer);

                EraseRecord(this, false);
                StoreRecord(dynamic_cast<CDSDataBlock*>(this));
                Reference();

                m_bVarSize = false;

                return m_pObjArray;
            };

            template<class _Type2>
            _TypeN* Attach(_Type2* pPointer)
            {
                return Attach(reinterpret_cast<CDSObject<_Type, _N>::_TypeN*>(pPointer));
            };

            CDSObject<_Type, _N>& Assign(const CDSObject<_Type, _N>& obj)
            {
                Free();

                m_pData     = obj.m_pData;
                m_pObjArray = obj.m_pObjArray;
                m_bVarSize  = obj.m_bVarSize;
                m_size      = obj.m_size;

                EraseRecord(this, false);
                StoreRecord(dynamic_cast<CDSDataBlock*>(this));
                Reference();   

                return (*this);
            };

            template<class _Type2, size_t _M>
            CDSObject<_Type, _N>& Assign(const CDSObject<_Type2, _M>& obj)
            {
                assert(_M > _N);

                return Assign(reinterpret_cast<const CDSObject<_Type, _N>&>(obj));
            };

            _TypeN* Reset(_Type* pPointer)
            {
                Free();

                return Attach(pPointer);
            };

            _Type* Object()
            {
                return reinterpret_cast<_Type*>(m_pObjArray);
            };

            _Type* operator ->()
            {
                return Object();
            };

            operator _TypeN*()
            {
                return m_pObjArray;
            };

            _Type&  operator[](int pos)
            {
                return (*m_pObjArray)[pos];
            };

            CDSObject<_Type, _N>& operator =(const CDSObject<_Type, _N>& obj)
            {
                return Assign(obj);
            };

            template<class _Type2, size_t _M>
            CDSObject<_Type, _N>& operator =(const CDSObject<_Type2, _M>& obj)
            {
                return Assign<_Type2, _M>(obj);
            };

            template<class C>
            C* CastObject()
            {
                return reinterpret_cast<C*>(m_pObjArray);
            };

            template<class C>
            operator C*()
            {
                return CastObject<C>();
            };

            virtual unsigned char* Allocate(size_t size)
            {
                CDSDataBlock::Allocate(size);

                m_pObjArray = reinterpret_cast<_TypeN*>(m_pData);
                m_bVarSize = true;

                return m_pData;
            };

            virtual void Free()
            {
                if(m_pObjArray != 0 )
                {
                    int refs = GetRefCount();

                    assert(refs > 0);

                    if(m_bVarSize)
                    {
                        CDSDataBlock::Free();
                    }
                    else 
                    {
                        Dereference();

                        if(refs == 1)
                            delete[] m_pObjArray;
                    }   

                    EraseRecord(this, (refs == 1));

                    m_pData     = 0;
                    m_size      = 0; 
                    m_pObjArray = reinterpret_cast<_TypeN*>(0);
                }                  
            };

        private:
            _TypeN*     m_pObjArray;
            bool        m_bVarSize;
        };
    }
}


#endif //_DS_OBJECT_H_