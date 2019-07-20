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

#include "File.hpp"
#include "Casts.hpp"
#include "BinaryDataStream.hpp"

namespace VBox
{
    namespace ELF
    {
        class File::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( BinaryStream & stream );
                IMPL( const IMPL & o );
                
                Header                            _header;
                std::vector< ProgramHeaderEntry > _programHeader;
        };
        
        File::File( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        File::File( BinaryStream & stream ):
            impl( std::make_unique< IMPL >( stream ) )
        {}
        
        File::File( const File & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        File::File( File && o ):
            impl( std::move( o.impl ) )
        {}
        
        File::~File( void )
        {}
        
        File & File::operator =( File o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        Header File::header( void ) const
        {
            return this->impl->_header;
        }
        
        std::vector< ProgramHeaderEntry > File::programHeader( void ) const
        {
            return this->impl->_programHeader;
        }
        
        void swap( File & o1, File & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        std::ostream & operator <<( std::ostream & os, const File & o )
        {
            os << "{"             << std::endl
               << "    Header:"   << std::endl
               << o.impl->_header << std::endl;
            
            if( o.impl->_programHeader.size() > 0 )
            {
                os << "    Program header entries:" << std::endl
                   << "    {"                       << std::endl;
                
                for( const auto & entry: o.impl->_programHeader )
                {
                    os << entry << std::endl;
                }
                
                os << "    }" << std::endl;
            }
            
            os << "}";
               
            return os;
        }
        
        File::IMPL::IMPL( void )
        {}
        
        File::IMPL::IMPL( BinaryStream & stream ):
            _header( stream )
        {
            if
            (
                   this->_header.programHeaderOffset()     != 0
                && this->_header.programHeaderEntrySize()  != 0
                && this->_header.programHeaderEntryCount() != 0
            )
            {
                stream.Seek( numeric_cast< ssize_t >( this->_header.programHeaderOffset() ), BinaryStream::SeekDirection::Begin );
                
                for( size_t i = 0; i < this->_header.programHeaderEntryCount(); i++ )
                {
                    this->_programHeader.push_back( ProgramHeaderEntry( stream ) );
                }
            }
        }
        
        File::IMPL::IMPL( const IMPL & o ):
            _header(        o._header ),
            _programHeader( o._programHeader )
        {}
    }
}
