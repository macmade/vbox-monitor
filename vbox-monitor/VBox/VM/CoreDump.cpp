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

#include "CoreDump.hpp"

namespace VBox
{
    namespace VM
    {
        class CoreDump::IMPL
        {
            public:
                
                IMPL( const std::string & path );
                IMPL( const IMPL & o );
                
                std::string _path;
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
        
        void swap( CoreDump & o1, CoreDump & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        CoreDump::IMPL::IMPL( const std::string & path ):
            _path( path )
        {}
        
        CoreDump::IMPL::IMPL( const IMPL & o ):
            _path( o._path )
        {}
    }
}
