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

#include "SegmentAddress.hpp"

namespace VBox
{
    namespace VM
    {
        class SegmentAddress::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( const IMPL & o );
                
                uint64_t _segment;
                uint64_t _address;
        };
        
        SegmentAddress::SegmentAddress( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        SegmentAddress::SegmentAddress( const SegmentAddress & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        SegmentAddress::SegmentAddress( SegmentAddress && o ):
            impl( std::move( o.impl ) )
        {}
        
        SegmentAddress::~SegmentAddress( void )
        {}
        
        SegmentAddress & SegmentAddress::operator =( SegmentAddress o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        uint64_t SegmentAddress::segment( void ) const
        {
            return this->impl->_segment;
        }
        
        uint64_t SegmentAddress::address( void ) const
        {
            return this->impl->_address;
        }
        
        void SegmentAddress::segment( uint64_t value )
        {
            this->impl->_segment = value;
        }
        
        void SegmentAddress::address( uint64_t value )
        {
            this->impl->_address = value;
        }
        
        void swap( SegmentAddress & o1, SegmentAddress & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        SegmentAddress::IMPL::IMPL( void ):
            _segment( 0 ),
            _address( 0 )
        {}
        
        SegmentAddress::IMPL::IMPL( const IMPL & o ):
            _segment( o._segment ),
            _address( o._address )
        {}
    }
}
