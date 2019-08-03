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

#include "VBox/VM/StackEntry.hpp"
#include "VBox/String.hpp"

namespace VBox
{
    namespace VM
    {
        class StackEntry::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( const IMPL & o );
                
                SegmentAddress _bp;
                SegmentAddress _retBP;
                SegmentAddress _retIP;
                uint32_t       _arg0;
                uint32_t       _arg1;
                uint32_t       _arg2;
                uint32_t       _arg3;
                SegmentAddress _ip;
        };
        
        StackEntry::StackEntry( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        StackEntry::StackEntry( const StackEntry & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        StackEntry::StackEntry( StackEntry && o ):
            impl( std::move( o.impl ) )
        {}
        
        StackEntry::~StackEntry( void )
        {}
        
        StackEntry & StackEntry::operator =( StackEntry o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        SegmentAddress StackEntry::bp( void ) const
        {
            return this->impl->_bp;
        }
        
        SegmentAddress StackEntry::retBP( void ) const
        {
            return this->impl->_retBP;
        }
        
        SegmentAddress StackEntry::retIP( void ) const
        {
            return this->impl->_retIP;
        }
        
        uint32_t StackEntry::arg0( void ) const
        {
            return this->impl->_arg0;
        }
        
        uint32_t StackEntry::arg1( void ) const
        {
            return this->impl->_arg1;
        }
        
        uint32_t StackEntry::arg2( void ) const
        {
            return this->impl->_arg2;
        }
        
        uint32_t StackEntry::arg3( void ) const
        {
            return this->impl->_arg3;
        }
        
        SegmentAddress StackEntry::ip( void ) const
        {
            return this->impl->_ip;
        }
        
        void StackEntry::bp( const SegmentAddress & value )
        {
            this->impl->_bp = value;
        }
        
        void StackEntry::retBP( const SegmentAddress & value )
        {
            this->impl->_retBP = value;
        }
        
        void StackEntry::retIP( const SegmentAddress & value )
        {
            this->impl->_retIP = value;
        }
        
        void StackEntry::arg0( uint32_t value )
        {
            this->impl->_arg0 = value;
        }
        
        void StackEntry::arg1( uint32_t value )
        {
            this->impl->_arg1 = value;
        }
        
        void StackEntry::arg2( uint32_t value )
        {
            this->impl->_arg2 = value;
        }
        
        void StackEntry::arg3( uint32_t value )
        {
            this->impl->_arg3 = value;
        }
        
        void StackEntry::ip( const SegmentAddress & value )
        {
            this->impl->_ip = value;
        }
        
        void swap( StackEntry & o1, StackEntry & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        StackEntry::IMPL::IMPL( void ):
            _arg0( 0 ),
            _arg1( 0 ),
            _arg2( 0 ),
            _arg3( 0 )
        {}
        
        StackEntry::IMPL::IMPL( const IMPL & o ):
            _bp(    o._bp ),
            _retBP( o._retBP ),
            _retIP( o._retIP ),
            _arg0(  o._arg0 ),
            _arg1(  o._arg1 ),
            _arg2(  o._arg2 ),
            _arg3(  o._arg3 ),
            _ip(    o._ip )
        {}
    }
}
