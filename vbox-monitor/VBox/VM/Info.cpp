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

#include "VBox/VM/Info.hpp"
#include "VBox/String.hpp"

namespace VBox
{
    namespace VM
    {
        class Info::IMPL
        {
            public:
                
                IMPL( const std::string & name, const std::string & uid );
                IMPL( const IMPL & o );
                
                std::string _name;
                std::string _uid;
        };
        
        Info::Info( void ):
            Info( "", "" )
        {}
        
        Info::Info( const std::string & name, const std::string & uid ):
            impl( std::make_unique< IMPL >( name, uid ) )
        {}
        
        Info::Info( const Info & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        Info::Info( Info && o ):
            impl( std::move( o.impl ) )
        {}
        
        Info::~Info( void )
        {}
        
        Info & Info::operator =( Info o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        std::string Info::name( void ) const
        {
            return this->impl->_name;
        }
        
        std::string Info::uid( void ) const
        {
            return this->impl->_uid;
        }
        
        void Info::name( const std::string & value )
        {
            this->impl->_name = value;
        }
        
        void Info::uid( const std::string & value )
        {
            this->impl->_uid = value;
        }
        
        void swap( Info & o1, Info & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        Info::IMPL::IMPL( const std::string & name, const std::string & uid ):
            _name( name ),
            _uid(  uid )
        {}
        
        Info::IMPL::IMPL( const IMPL & o ):
            _name( o._name ),
            _uid(  o._uid )
        {}
    }
}
