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

#include "VBox/VM/CoreDump.hpp"
#include "VBox/BinaryFileStream.hpp"
#include "VBox/BinaryDataStream.hpp"
#include "VBox/ELF/File.hpp"
#include "VBox/Casts.hpp"

namespace VBox
{
    namespace VM
    {
        class CoreDump::IMPL
        {
            public:
                
                IMPL( const std::string & path );
                IMPL( const IMPL & o );
                
                void _parse( void );
                
                std::string      _path;
                uint64_t         _memorySize;
                BinaryDataStream _memory;
        };
        
        CoreDump::CoreDump( const std::string & path ):
            impl( std::make_unique< IMPL >( path ) )
        {}
        
        CoreDump::CoreDump( const CoreDump & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        CoreDump::CoreDump( CoreDump && o ):
            impl( std::move( o.impl ) )
        {}
        
        CoreDump::~CoreDump( void )
        {}
        
        CoreDump & CoreDump::operator =( CoreDump o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        std::string CoreDump::path( void ) const
        {
            return this->impl->_path;
        }
        
        uint64_t CoreDump::memorySize( void ) const
        {
            return this->impl->_memorySize;
        }
        
        std::vector< uint8_t > CoreDump::readMemory( size_t offset, size_t size )
        {
            try
            {
                this->impl->_memory.Seek( numeric_cast< ssize_t >( offset ), BinaryStream::SeekDirection::Begin );
                
                return this->impl->_memory.Read( size );
            }
            catch( ... )
            {
                return {};
            }
        }
        
        void swap( CoreDump & o1, CoreDump & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        CoreDump::IMPL::IMPL( const std::string & path ):
            _path( path ),
            _memorySize( 0 )
        {
            this->_parse();
        }
        
        CoreDump::IMPL::IMPL( const IMPL & o ):
            _path(       o._path ),
            _memorySize( o._memorySize ),
            _memory(     o._memory )
        {
            this->_parse();
        }
        
        void CoreDump::IMPL::_parse( void )
        {
            BinaryFileStream                       stream( this->_path );
            ELF::File                              elf( stream );
            std::vector< ELF::ProgramHeaderEntry > entries( elf.programHeader() );
            
            if( entries.size() < 2 || entries[ 0 ].type() != 0x04 || entries[ 1 ].type() != 0x01 )
            {
                throw std::runtime_error( "Invalid core dump" );
            }
            
            {
                ELF::ProgramHeaderEntry mem( entries[ 1 ] );
                
                if( mem.offset() == 0 || mem.fileSize() == 0 || mem.fileSize() != mem.memorySize() )
                {
                    throw std::runtime_error( "Invalid core dump" );
                }
                
                stream.Seek( numeric_cast< ssize_t >( mem.offset() ), BinaryStream::SeekDirection::Begin );
                
                this->_memorySize = mem.fileSize();
                this->_memory     = BinaryDataStream( stream.Read( mem.fileSize() ) );
            }
        }
    }
}
