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

#include "Manage.hpp"
#include "Process.hpp"
#include "String.hpp"
#include <optional>
#include <regex>
#include <iostream>

namespace VBox
{
    namespace Manage
    {
        VM::Registers registers( const std::string & vmName )
        {
            VM::Registers                reg;
            Process                      proc( "/usr/local/bin/VBoxManage" );
            std::optional< std::string > out;
            
            proc.arguments
            (
                {
                    "debugvm", vmName, "getregisters",
                    "rax", "rbx", "rcx", "rdx", "rdi", "rsi",
                    "r8",  "r9",  "r10", "r11", "r12", "r13",
                    "r14", "r15", "rbp", "rsp", "rip", "eflags"
                }
            );
            
            proc.start();
            proc.waitUntilExit();
            
            out = proc.output();
            
            if( out.has_value() == false )
            {
                return reg;
            }
            
            {
                std::regex  regex( "([^ ]+) = (0x[0-9a-f]+)" );
                std::smatch match;
                
                for( const auto & line: String::lines( out.value() ) )
                {
                    if( std::regex_match( line, match, regex ) )
                    {
                        {
                            std::string name(  match[ 1 ] );
                            std::string value( match[ 2 ] );
                            
                            if( name == "rax"    ) { reg.rax(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rbx"    ) { reg.rbx(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rcx"    ) { reg.rcx(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rdx"    ) { reg.rdx(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rdi"    ) { reg.rdi(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rsi"    ) { reg.rsi(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "r8"     ) { reg.r8(     String::fromHex< uint64_t >( value ) ); };
                            if( name == "r9"     ) { reg.r9(     String::fromHex< uint64_t >( value ) ); };
                            if( name == "r10"    ) { reg.r10(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "r11"    ) { reg.r11(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "r12"    ) { reg.r12(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "r13"    ) { reg.r13(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "r14"    ) { reg.r14(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "r15"    ) { reg.r15(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rbp"    ) { reg.rbp(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rsp"    ) { reg.rsp(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "rip"    ) { reg.rip(    String::fromHex< uint64_t >( value ) ); };
                            if( name == "eflags" ) { reg.eflags( String::fromHex< uint64_t >( value ) ); };
                        }
                    }
                }
            }
            
            return reg;
        }
    }
}
