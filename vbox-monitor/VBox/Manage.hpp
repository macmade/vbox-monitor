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

#ifndef VBOX_MANAGE_HPP
#define VBOX_MANAGE_HPP

#include "VBox/VM/Registers.hpp"
#include "VBox/VM/StackEntry.hpp"
#include "VBox/VM/CoreDump.hpp"
#include "VBox/VM/Info.hpp"
#include <string>
#include <vector>
#include <optional>

namespace VBox
{
    namespace Manage
    {
        bool registerVM( const std::string & path );
        bool unregisterVM( const std::string & vmName );
        bool startVM( const std::string & vmName );
        bool powerOffVM( const std::string & vmName );
        
        std::vector< VM::Info > runningVMs( void );
        
        namespace Debug
        {
            std::optional< VM::Registers >  registers( const std::string & vmName );
            std::vector< VM::StackEntry >   stack( const std::string & vmName );
            std::shared_ptr< VM::CoreDump > dump( const std::string & vmName, const std::string & path );
        }
    };
}

#endif /* VBOX_MANAGE_HPP */
