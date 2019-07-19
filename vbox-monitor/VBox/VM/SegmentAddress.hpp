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

#ifndef VBOX_VM_SEGMENT_ADDRESS_HPP
#define VBOX_VM_SEGMENT_ADDRESS_HPP

#include <cstdint>
#include <algorithm>
#include <memory>

namespace VBox
{
    namespace VM
    {
        class SegmentAddress
        {
            public:
                
                SegmentAddress( void );
                SegmentAddress( const SegmentAddress & o );
                SegmentAddress( SegmentAddress && o );
                ~SegmentAddress( void );
                
                SegmentAddress & operator =( SegmentAddress o );
                
                uint64_t segment( void ) const;
                uint64_t address( void ) const;
                
                void segment( uint64_t value );
                void address( uint64_t value );
                
                friend void swap( SegmentAddress & o1, SegmentAddress & o2 );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* VBOX_VM_SEGMENT_ADDRESS_HPP */
