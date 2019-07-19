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

#include "Registers.hpp"

namespace VBox
{
    namespace VM
    {
        class Registers::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( const IMPL & o );
                
                uint64_t _rax;
                uint64_t _rbx;
                uint64_t _rcx;
                uint64_t _rdx;
                uint64_t _rdi;
                uint64_t _rsi;
                uint64_t _r8;
                uint64_t _r9;
                uint64_t _r10;
                uint64_t _r11;
                uint64_t _r12;
                uint64_t _r13;
                uint64_t _r14;
                uint64_t _r15;
                uint64_t _rbp;
                uint64_t _rsp;
                uint64_t _rip;
        };
        
        Registers::Registers( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        Registers::Registers( const Registers & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        Registers::Registers( Registers && o ):
            impl( std::move( o.impl ) )
        {}
        
        Registers::~Registers( void )
        {}
        
        Registers & Registers::operator =( Registers o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        uint64_t Registers::rax( void ) const
        {
            return this->impl->_rax;
        }
        
        uint64_t Registers::rbx( void ) const
        {
            return this->impl->_rbx;
        }
        
        uint64_t Registers::rcx( void ) const
        {
            return this->impl->_rcx;
        }
        
        uint64_t Registers::rdx( void ) const
        {
            return this->impl->_rdx;
        }
        
        uint64_t Registers::rdi( void ) const
        {
            return this->impl->_rdi;
        }
        
        uint64_t Registers::rsi( void ) const
        {
            return this->impl->_rsi;
        }
        
        uint64_t Registers::r8( void ) const
        {
            return this->impl->_r8;
        }
        
        uint64_t Registers::r9( void ) const
        {
            return this->impl->_r9;
        }
        
        uint64_t Registers::r10( void ) const
        {
            return this->impl->_r10;
        }
        
        uint64_t Registers::r11( void ) const
        {
            return this->impl->_r11;
        }
        
        uint64_t Registers::r12( void ) const
        {
            return this->impl->_r12;
        }
        
        uint64_t Registers::r13( void ) const
        {
            return this->impl->_r13;
        }
        
        uint64_t Registers::r14( void ) const
        {
            return this->impl->_r14;
        }
        
        uint64_t Registers::r15( void ) const
        {
            return this->impl->_r15;
        }
        
        uint64_t Registers::rbp( void ) const
        {
            return this->impl->_rbp;
        }
        
        uint64_t Registers::rsp( void ) const
        {
            return this->impl->_rsp;
        }
        
        uint64_t Registers::rip( void ) const
        {
            return this->impl->_rip;
        }
        
        void Registers::rax( uint64_t value )
        {
            this->impl->_rax = value;
        }
        
        void Registers::rbx( uint64_t value )
        {
            this->impl->_rbx = value;
        }
        
        void Registers::rcx( uint64_t value )
        {
            this->impl->_rcx = value;
        }
        
        void Registers::rdx( uint64_t value )
        {
            this->impl->_rdx = value;
        }
        
        void Registers::rdi( uint64_t value )
        {
            this->impl->_rdi = value;
        }
        
        void Registers::rsi( uint64_t value )
        {
            this->impl->_rsi = value;
        }
        
        void Registers::r8(  uint64_t value )
        {
            this->impl->_r8 = value;
        }
        
        void Registers::r9(  uint64_t value )
        {
            this->impl->_r9 = value;
        }
        
        void Registers::r10( uint64_t value )
        {
            this->impl->_r10 = value;
        }
        
        void Registers::r11( uint64_t value )
        {
            this->impl->_r11 = value;
        }
        
        void Registers::r12( uint64_t value )
        {
            this->impl->_r12 = value;
        }
        
        void Registers::r13( uint64_t value )
        {
            this->impl->_r13 = value;
        }
        
        void Registers::r14( uint64_t value )
        {
            this->impl->_r14 = value;
        }
        
        void Registers::r15( uint64_t value )
        {
            this->impl->_r15 = value;
        }
        
        void Registers::rbp( uint64_t value )
        {
            this->impl->_rbp = value;
        }
        
        void Registers::rsp( uint64_t value )
        {
            this->impl->_rsp = value;
        }
        
        void Registers::rip( uint64_t value )
        {
            this->impl->_rip = value;
        }
        
        void swap( Registers & o1, Registers & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        Registers::IMPL::IMPL( void ):
            _rax( 0 ),
            _rbx( 0 ),
            _rcx( 0 ),
            _rdx( 0 ),
            _rdi( 0 ),
            _rsi( 0 ),
            _r8(  0 ),
            _r9(  0 ),
            _r10( 0 ),
            _r11( 0 ),
            _r12( 0 ),
            _r13( 0 ),
            _r14( 0 ),
            _r15( 0 ),
            _rbp( 0 ),
            _rsp( 0 ),
            _rip( 0 )
        {}
        
        Registers::IMPL::IMPL( const IMPL & o ):
            _rax( o._rax ),
            _rbx( o._rbx ),
            _rcx( o._rcx ),
            _rdx( o._rdx ),
            _rdi( o._rdi ),
            _rsi( o._rsi ),
            _r8(  o._r8 ),
            _r9(  o._r9 ),
            _r10( o._r10 ),
            _r11( o._r11 ),
            _r12( o._r12 ),
            _r13( o._r13 ),
            _r14( o._r14 ),
            _r15( o._r15 ),
            _rbp( o._rbp ),
            _rsp( o._rsp ),
            _rip( o._rip )
        {}
    }
}
