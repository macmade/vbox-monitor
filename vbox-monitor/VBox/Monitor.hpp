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

#ifndef VBOX_MONITOR_HPP
#define VBOX_MONITOR_HPP

#include <string>
#include <memory>
#include <algorithm>
#include <chrono>
#include <vector>
#include <optional>
#include "VBox/VM/Registers.hpp"
#include "VBox/VM/StackEntry.hpp"
#include "VBox/VM/CoreDump.hpp"

namespace VBox
{
    class Monitor
    {
        public:
            
            Monitor( const std::string & vmName );
            Monitor( const Monitor & o );
            Monitor( Monitor && o );
            ~Monitor( void );
            
            Monitor & operator =( Monitor o );
            
            std::optional< VM::Registers >  registers( void ) const;
            std::vector< VM::StackEntry >   stack( void )     const;
            std::shared_ptr< VM::CoreDump > dump( void )      const;
            
            void start( void );
            void stop( void );
            
            friend void swap( Monitor & o1, Monitor & o2 );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* VBOX_MONITOR_HPP */
