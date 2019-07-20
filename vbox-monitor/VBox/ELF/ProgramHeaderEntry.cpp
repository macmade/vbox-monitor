/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Jean-David Gadina - www.xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "ProgramHeaderEntry.hpp"
#include "String.hpp"

namespace VBox
{
    namespace ELF
    {
        class ProgramHeaderEntry::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( BinaryStream & stream );
                IMPL( const IMPL & o );
                
                uint32_t _type;
                uint32_t _flags;
                uint64_t _offset;
                uint64_t _vaddress;
                uint64_t _paddress;
                uint64_t _fileSize;
                uint64_t _memorySize;
                uint64_t _alignment;
        };
        
        ProgramHeaderEntry::ProgramHeaderEntry( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        ProgramHeaderEntry::ProgramHeaderEntry( BinaryStream & stream ):
            impl( std::make_unique< IMPL >( stream ) )
        {}
        
        ProgramHeaderEntry::ProgramHeaderEntry( const ProgramHeaderEntry & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        ProgramHeaderEntry::ProgramHeaderEntry( ProgramHeaderEntry && o ):
            impl( std::move( o.impl ) )
        {}
        
        ProgramHeaderEntry::~ProgramHeaderEntry( void )
        {}
        
        ProgramHeaderEntry & ProgramHeaderEntry::operator =( ProgramHeaderEntry o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        uint32_t ProgramHeaderEntry::type( void ) const
        {
            return this->impl->_type;
        }
        
        uint32_t ProgramHeaderEntry::flags( void ) const
        {
            return this->impl->_flags;
        }
        
        uint64_t ProgramHeaderEntry::offset( void ) const
        {
            return this->impl->_offset;
        }
        
        uint64_t ProgramHeaderEntry::vaddress( void ) const
        {
            return this->impl->_vaddress;
        }
        
        uint64_t ProgramHeaderEntry::paddress( void ) const
        {
            return this->impl->_paddress;
        }
        
        uint64_t ProgramHeaderEntry::fileSize( void ) const
        {
            return this->impl->_fileSize;
        }
        
        uint64_t ProgramHeaderEntry::memorySize( void ) const
        {
            return this->impl->_memorySize;
        }
        
        uint64_t ProgramHeaderEntry::alignment( void ) const
        {
            return this->impl->_alignment;
        }
        
        void swap( ProgramHeaderEntry & o1, ProgramHeaderEntry & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        std::ostream & operator <<( std::ostream & os, const ProgramHeaderEntry & o )
        {
            ( void )o;
            
            os << "        {" << std::endl
               << "            Type:        " << String::toHex( o.impl->_type )       << std::endl
               << "            Flags:       " << String::toHex( o.impl->_flags )      << std::endl
               << "            Offset:      " << String::toHex( o.impl->_offset )     << std::endl
               << "            VAddress:    " << String::toHex( o.impl->_vaddress )   << std::endl
               << "            PAddress:    " << String::toHex( o.impl->_paddress )   << std::endl
               << "            File size:   " << o.impl->_fileSize                    << std::endl
               << "            Memory size: " << o.impl->_memorySize                  << std::endl
               << "            Alignment:   " << o.impl->_alignment                   << std::endl
               << "        }";
            
            return os;
        }
        
        ProgramHeaderEntry::IMPL::IMPL( void ):
            _type(       0 ),
            _flags(      0 ),
            _offset(     0 ),
            _vaddress(   0 ),
            _paddress(   0 ),
            _fileSize(   0 ),
            _memorySize( 0 ),
            _alignment(  0 )
        {}
        
        ProgramHeaderEntry::IMPL::IMPL( BinaryStream & stream ):
            _type(       stream.ReadLittleEndianUInt32() ),
            _flags(      stream.ReadLittleEndianUInt32() ),
            _offset(     stream.ReadLittleEndianUInt64() ),
            _vaddress(   stream.ReadLittleEndianUInt64() ),
            _paddress(   stream.ReadLittleEndianUInt64() ),
            _fileSize(   stream.ReadLittleEndianUInt64() ),
            _memorySize( stream.ReadLittleEndianUInt64() ),
            _alignment(  stream.ReadLittleEndianUInt64() )
        {}
        
        ProgramHeaderEntry::IMPL::IMPL( const IMPL & o ):
            _type(       o._type ),
            _flags(      o._flags ),
            _offset(     o._offset ),
            _vaddress(   o._vaddress ),
            _paddress(   o._paddress ),
            _fileSize(   o._fileSize ),
            _memorySize( o._memorySize ),
            _alignment(  o._alignment )
        {}
    }
}
