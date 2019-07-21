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

#include "VBox/ELF/Header.hpp"
#include "VBox/String.hpp"

namespace VBox
{
    namespace ELF
    {
        class Header::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( BinaryStream & stream );
                IMPL( const IMPL & o );
                
                std::vector< uint8_t > _ident;
                uint16_t               _type;
                uint16_t               _machine;
                uint32_t               _version;
                uint64_t               _entry;
                uint64_t               _programHeaderOffset;
                uint64_t               _sectionHeaderOffset;
                uint32_t               _flags;
                uint16_t               _elfHeaderSize;
                uint16_t               _programHeaderEntrySize;
                uint16_t               _programHeaderEntryCount;
                uint16_t               _sectionHeaderEntrySize;
                uint16_t               _sectionHeaderEntryCount;
                uint16_t               _sectionNameStringTableIndex;
        };
        
        Header::Header( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        Header::Header( BinaryStream & stream ):
            impl( std::make_unique< IMPL >( stream ) )
        {}
        
        Header::Header( const Header & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        Header::Header( Header && o ):
            impl( std::move( o.impl ) )
        {}
        
        Header::~Header( void )
        {}
        
        Header & Header::operator =( Header o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        std::vector< uint8_t > Header::ident( void ) const
        {
            return this->impl->_ident;
        }
        
        uint16_t Header::type( void ) const
        {
            return this->impl->_type;
        }
        
        uint16_t Header::machine( void ) const
        {
            return this->impl->_machine;
        }
        
        uint32_t Header::version( void ) const
        {
            return this->impl->_version;
        }
        
        uint64_t Header::entry( void ) const
        {
            return this->impl->_entry;
        }
        
        uint64_t Header::programHeaderOffset( void ) const
        {
            return this->impl->_programHeaderOffset;
        }
        
        uint64_t Header::sectionHeaderOffset( void ) const
        {
            return this->impl->_sectionHeaderOffset;
        }
        
        uint32_t Header::flags( void ) const
        {
            return this->impl->_flags;
        }
        
        uint16_t Header::elfHeaderSize( void ) const
        {
            return this->impl->_elfHeaderSize;
        }
        
        uint16_t Header::programHeaderEntrySize( void ) const
        {
            return this->impl->_programHeaderEntrySize;
        }
        
        uint16_t Header::programHeaderEntryCount( void ) const
        {
            return this->impl->_programHeaderEntryCount;
        }
        
        uint16_t Header::sectionHeaderEntrySize( void ) const
        {
            return this->impl->_sectionHeaderEntrySize;
        }
        
        uint16_t Header::sectionHeaderEntryCount( void ) const
        {
            return this->impl->_sectionHeaderEntryCount;
        }
        
        uint16_t Header::sectionNameStringTableIndex( void ) const
        {
            return this->impl->_sectionNameStringTableIndex;
        }
        
        void swap( Header & o1, Header & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        std::ostream & operator <<( std::ostream & os, const Header & o )
        {
            os << "    {" << std::endl
               << "        Type:                            " << String::toHex( o.impl->_type )                << std::endl
               << "        Machine:                         " << String::toHex( o.impl->_machine )             << std::endl
               << "        Version:                         " << String::toHex( o.impl->_version )             << std::endl
               << "        Entry:                           " << String::toHex( o.impl->_entry )               << std::endl
               << "        Program header offset:           " << String::toHex( o.impl->_programHeaderOffset ) << std::endl
               << "        Section header offset:           " << String::toHex( o.impl->_sectionHeaderOffset ) << std::endl
               << "        Flags:                           " << String::toHex( o.impl->_flags )               << std::endl
               << "        ELF header size:                 " << o.impl->_elfHeaderSize                        << std::endl
               << "        Program header entry size:       " << o.impl->_programHeaderEntrySize               << std::endl
               << "        Program header entry count:      " << o.impl->_programHeaderEntryCount              << std::endl
               << "        Section header entry size:       " << o.impl->_sectionHeaderEntrySize               << std::endl
               << "        Section header entry count:      " << o.impl->_sectionHeaderEntryCount              << std::endl
               << "        Section name string table index: " << o.impl->_sectionNameStringTableIndex          << std::endl
               << "    }";
            
            return os;
        }
        
        Header::IMPL::IMPL( void ):
            _type(                        0 ),
            _machine(                     0 ),
            _version(                     0 ),
            _entry(                       0 ),
            _programHeaderOffset(         0 ),
            _sectionHeaderOffset(         0 ),
            _flags(                       0 ),
            _elfHeaderSize(               0 ),
            _programHeaderEntrySize(      0 ),
            _programHeaderEntryCount(     0 ),
            _sectionHeaderEntrySize(      0 ),
            _sectionHeaderEntryCount(     0 ),
            _sectionNameStringTableIndex( 0 )
        {}
        
        Header::IMPL::IMPL( BinaryStream & stream ):
            IMPL()
        {
            this->_ident.resize( 16 );
            stream.Read( &( this->_ident[ 0 ] ), 16 );
            
            this->_type                        = stream.ReadLittleEndianUInt16();
            this->_machine                     = stream.ReadLittleEndianUInt16();
            this->_version                     = stream.ReadLittleEndianUInt32();
            this->_entry                       = stream.ReadLittleEndianUInt64();
            this->_programHeaderOffset         = stream.ReadLittleEndianUInt64();
            this->_sectionHeaderOffset         = stream.ReadLittleEndianUInt64();
            this->_flags                       = stream.ReadLittleEndianUInt32();
            this->_elfHeaderSize               = stream.ReadLittleEndianUInt16();
            this->_programHeaderEntrySize      = stream.ReadLittleEndianUInt16();
            this->_programHeaderEntryCount     = stream.ReadLittleEndianUInt16();
            this->_sectionHeaderEntrySize      = stream.ReadLittleEndianUInt16();
            this->_sectionHeaderEntryCount     = stream.ReadLittleEndianUInt16();
            this->_sectionNameStringTableIndex = stream.ReadLittleEndianUInt16();
        }
        
        Header::IMPL::IMPL( const IMPL & o ):
            _type(                        o._type ),
            _machine(                     o._machine ),
            _version(                     o._version ),
            _entry(                       o._entry ),
            _programHeaderOffset(         o._programHeaderOffset ),
            _sectionHeaderOffset(         o._sectionHeaderOffset ),
            _flags(                       o._flags ),
            _elfHeaderSize(               o._elfHeaderSize ),
            _programHeaderEntrySize(      o._programHeaderEntrySize ),
            _programHeaderEntryCount(     o._programHeaderEntryCount ),
            _sectionHeaderEntrySize(      o._sectionHeaderEntrySize ),
            _sectionHeaderEntryCount(     o._sectionHeaderEntryCount ),
            _sectionNameStringTableIndex( o._sectionNameStringTableIndex )
        {}
    }
}
