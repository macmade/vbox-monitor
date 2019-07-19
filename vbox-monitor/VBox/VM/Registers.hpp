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

#ifndef VBOX_VM_REGISTERS_HPP
#define VBOX_VM_REGISTERS_HPP

#include <cstdint>
#include <memory>
#include <algorithm>
#include <chrono>
#include <ostream>
#include <vector>

namespace VBox
{
    namespace VM
    {
        class Registers
        {
            public:
                
                Registers( void );
                Registers( const Registers & o );
                Registers( Registers && o );
                ~Registers( void );
                
                Registers & operator =( Registers o );
                
                uint64_t rax( void )    const;
                uint64_t rbx( void )    const;
                uint64_t rcx( void )    const;
                uint64_t rdx( void )    const;
                uint64_t rdi( void )    const;
                uint64_t rsi( void )    const;
                uint64_t r8( void )     const;
                uint64_t r9( void )     const;
                uint64_t r10( void )    const;
                uint64_t r11( void )    const;
                uint64_t r12( void )    const;
                uint64_t r13( void )    const;
                uint64_t r14( void )    const;
                uint64_t r15( void )    const;
                uint64_t rbp( void )    const;
                uint64_t rsp( void )    const;
                uint64_t rip( void )    const;
                uint64_t eflags( void ) const;
                
                void rax( uint64_t value );
                void rbx( uint64_t value );
                void rcx( uint64_t value );
                void rdx( uint64_t value );
                void rdi( uint64_t value );
                void rsi( uint64_t value );
                void r8( uint64_t value );
                void r9( uint64_t value );
                void r10( uint64_t value );
                void r11( uint64_t value );
                void r12( uint64_t value );
                void r13( uint64_t value );
                void r14( uint64_t value );
                void r15( uint64_t value );
                void rbp( uint64_t value );
                void rsp( uint64_t value );
                void rip( uint64_t value );
                void eflags( uint64_t value );
                
                std::vector< std::pair< std::string, uint64_t > > all( void ) const;
                
                friend void swap( Registers & o1, Registers & o2 );
                
                friend std::ostream & operator <<( std::ostream & os, const Registers & o );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* VBOX_VM_REGISTERS_HPP */
