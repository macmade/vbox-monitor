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

#ifndef VBOX_VM_STACK_ENTRY_HPP
#define VBOX_VM_STACK_ENTRY_HPP

#include "SegmentAddress.hpp"
#include <cstdint>
#include <memory>
#include <algorithm>
#include <ostream>
#include <vector>

namespace VBox
{
    namespace VM
    {
        class StackEntry
        {
            public:
                
                StackEntry( void );
                StackEntry( const StackEntry & o );
                StackEntry( StackEntry && o );
                ~StackEntry( void );
                
                StackEntry & operator =( StackEntry o );
                
                SegmentAddress bp( void )    const;
                SegmentAddress retBP( void ) const;
                SegmentAddress retIP( void ) const;
                uint64_t       arg0( void )  const;
                uint64_t       arg1( void )  const;
                uint64_t       arg2( void )  const;
                uint64_t       arg3( void )  const;
                SegmentAddress ip( void )    const;
                
                void bp(    const SegmentAddress & value );
                void retBP( const SegmentAddress & value );
                void retIP( const SegmentAddress & value );
                void arg0(  uint64_t value );
                void arg1(  uint64_t value );
                void arg2(  uint64_t value );
                void arg3(  uint64_t value );
                void ip(    const SegmentAddress & value );
                
                friend void swap( StackEntry & o1, StackEntry & o2 );
                
                friend std::ostream & operator <<( std::ostream & os, const StackEntry & o );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* VBOX_VM_STACK_ENTRY_HPP */
