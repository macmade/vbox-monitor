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

#ifndef VBOX_VM_CORE_DUMP_HPP
#define VBOX_VM_CORE_DUMP_HPP

#include <algorithm>
#include <memory>
#include <string>

namespace VBox
{
    namespace VM
    {
        class CoreDump
        {
            public:
                
                CoreDump( const std::string & path );
                CoreDump( const CoreDump & o );
                CoreDump( CoreDump && o );
                ~CoreDump( void );
                
                CoreDump & operator =( CoreDump o );
                
                std::string path( void )       const;
                uint64_t    memorySize( void ) const;
                
                std::vector< uint8_t > readMemory( size_t offset, size_t size );
                
                friend void swap( CoreDump & o1, CoreDump & o2 );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* VBOX_VM_CORE_DUMP_HPP */
