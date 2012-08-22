#include "stdafx.h"
#include "DSMemory.h"

namespace ds_utils
{
    namespace ds_memory
    {
        std::vector<CDSDataBlock*> CDSDataBlock::ms_Allocations;
        std::map<void*, int>       CDSDataBlock::ms_References;

        void CleanUp()
        {
            CDSDataBlock::CleanUp();
        }

        CDSDataBlock::CDSDataBlock( void ) :
            m_pData(0),
            m_size(0)
        {
        }

        CDSDataBlock::CDSDataBlock( const CDSDataBlock& blk ) :
            m_pData(0),
            m_size(0)
        {
            Assign(blk);
        }

        CDSDataBlock::~CDSDataBlock( void )
        {
            Free();
            EraseRecord(this, false);
        }

        CDSDataBlock& CDSDataBlock::Assign( const CDSDataBlock& blk )
        {
            Free();

            m_size = blk.m_size;
            m_pData = blk.m_pData;

            EraseRecord(this, false);

            ms_Allocations.push_back(dynamic_cast<CDSDataBlock*>(this));
            ms_References[m_pData]++;

            return (*this);
        }

        unsigned char* CDSDataBlock::AllocateAndCopyFrom( CDSDataBlock& blk )
        {
            Free();

            Allocate(blk.Size());
            memcpy(m_pData, blk, blk.Size());

            return m_pData;
        }

        unsigned char* CDSDataBlock::AllocateAndCopyFrom( unsigned char* pPtr, size_t size )
        {
            Free();

            Allocate(size);
            memcpy(m_pData, pPtr, size);

            return m_pData;
        }

        CDSDataBlock& CDSDataBlock::operator=( const CDSDataBlock& blk )
        {
            return Assign(blk);
        }

        CDSDataBlock::operator unsigned char*()
        {
            return m_pData;
        }

        CDSDataBlock::operator void*()
        {
            return (void*)m_pData;
        }

        unsigned char& CDSDataBlock::operator[]( int pos )
        {
            return m_pData[pos];
        }

        bool CDSDataBlock::operator ==( CDSDataBlock& blk )
        {
            if(this->m_size == blk.m_size && memcmp(&this->m_pData, &blk.m_pData, this->m_size))
                return true;
            else
                return false;
        }

        bool CDSDataBlock::operator !=( CDSDataBlock& blk )
        {
            return !( *this == blk );
        }

        unsigned char* CDSDataBlock::Data()
        {
            return m_pData;
        }

        size_t CDSDataBlock::Size()
        {
            return m_size;
        }

        unsigned char* CDSDataBlock::Allocate( size_t size )
        {
            Free();

            if(size == 0)
                return 0;

            m_size = size;
            m_pData = new unsigned char[m_size]();

            ms_Allocations.push_back(dynamic_cast<CDSDataBlock*>(this));
            ms_References[m_pData]++;

            return m_pData;
        }

        unsigned char* CDSDataBlock::Reallocate( size_t newsize )
        {
            if(newsize == 0)
                return 0;

            size_t size = m_size;
            unsigned char* pTempData = new unsigned char[m_size];

            memcpy(pTempData, m_pData, size);

            Allocate(newsize);

            if(newsize > size)
                memcpy(m_pData, pTempData, size);
            else
                memcpy(m_pData, pTempData, newsize);

            return m_pData;
        }


        void CDSDataBlock::Free()
        {
            if(m_pData != 0 )
            {
                int refs = GetRefCount();

                assert(refs > 0);

                Dereference();

                if(refs == 1)
                    delete[] m_pData;

                EraseRecord(this, (refs == 1));

                m_pData = 0;
                m_size = 0;
            } 
        }

        void CDSDataBlock::CleanUp()
        {
            while( !ms_Allocations.empty() )
            {
                int refs = ms_Allocations.back()->GetRefCount();

                if (refs == -1)
                {
                    ms_Allocations.pop_back();
                    continue;
                }

                if(refs > 1)
                    ms_Allocations.back()->Dereference();

                ms_Allocations.back()->Free();
            }

            ms_References.clear();
        }

        void CDSDataBlock::Reference()
        {
            ms_References[m_pData]++;
        }

        void CDSDataBlock::Dereference()
        {
            if(GetRefCount() != -1)
                ms_References[m_pData]--;
        }

        int CDSDataBlock::GetRefCount()
        {
            if(ms_References.find(m_pData) != ms_References.end())
                return ms_References[m_pData];
            else
                return -1;
        }

        void CDSDataBlock::StoreRecord( CDSDataBlock* ptr )
        {
            ms_Allocations.push_back(ptr);
        }

        void CDSDataBlock::EraseRecord( CDSDataBlock* ptr, bool bDereference/*=true*/ )
        {
            for(unsigned int i=0; i<ms_Allocations.size(); i++)
                if(ms_Allocations[i] == ptr)
                {
                    ms_Allocations.erase(ms_Allocations.begin()+i);
                    break;
                }

            if(bDereference && ptr->GetRefCount() != -1)
                ms_References.erase(ptr->m_pData);
        }
    }
}