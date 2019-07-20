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

#include "UI.hpp"
#include "Screen.hpp"
#include "String.hpp"
#include "Monitor.hpp"
#include "Casts.hpp"
#include <ncurses.h>

namespace VBox
{
    class UI::IMPL
    {
        public:
            
            IMPL( const std::string & vmName );
            IMPL( const IMPL & o );
            
            void _setup( void );
            void _drawTitle( void );
            void _drawRegisters( void );
            void _drawStack( void );
            void _drawMemory( void );
            
            bool        _running;
            std::string _vmName;
            Screen      _screen;
            Monitor     _monitor;
    };
    
    UI::UI( const std::string & vmName ):
        impl( std::make_unique< IMPL >( vmName ) )
    {}
    
    UI::UI( const UI & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    UI::UI( UI && o ):
        impl( std::move( o.impl ) )
    {}
    
    UI::~UI( void )
    {}
    
    UI & UI::operator =( UI o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    void UI::run( void )
    {
        if( this->impl->_running )
        {
            return;
        }
        
        this->impl->_monitor.start();
        this->impl->_screen.start();
    }
    
    void swap( UI & o1, UI & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    UI::IMPL::IMPL( const std::string & vmName ):
        _running( false ),
        _vmName( vmName ),
        _monitor( vmName )
    {
        this->_setup();
    }
    
    UI::IMPL::IMPL( const IMPL & o ):
        _running( false ),
        _vmName( o._vmName ),
        _screen( o._screen ),
        _monitor( o._monitor )
    {
        this->_setup();
    }
    
    void UI::IMPL::_setup( void )
    {
        this->_screen.onUpdate
        (
            [ & ]( void )
            {
                this->_drawTitle();
                this->_drawRegisters();
                this->_drawStack();
                this->_drawMemory();
            }
        );
        
        this->_screen.onKeyPress
        (
            [ & ]( int key )
            {
                if( key == 'q' )
                {
                    this->_monitor.stop();
                    this->_screen.stop();
                }
            }
        );
    }
    
    void UI::IMPL::_drawTitle( void )
    {
        ::move( 0, 0 );
        ::hline( 0, numeric_cast< int >( this->_screen.width() ) );
        ::move( 1, 0 );
        ::printw( "VirtualBox: %s", this->_vmName.c_str() );
        ::move( 2, 0 );
        ::hline( 0, numeric_cast< int >( this->_screen.width() ) );
    }
    
    void UI::IMPL::_drawRegisters( void )
    {
        if( this->_screen.width() < 30 || this->_screen.height() < 25 )
        {
            return;
        }
        
        {
            ::WINDOW * win( ::newwin( 22, 30, 3, 0 ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "CPU Registers:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, 28 );
            }
            
            {
                std::optional< VM::Registers > regs( this->_monitor.registers() );
                
                if( regs.has_value() )
                {
                    int y( 3 );
                    
                    for( const auto & p: regs.value().all() )
                    {
                        std::string reg( String::toUpper( p.first ) );
                        
                        for( size_t i = reg.size(); i < 6; i++ )
                        {
                            reg = " " + reg;
                        }
                        
                        reg += ": ";
                        reg += String::toHex( p.second );
                        
                        ::wmove( win, y, 2 );
                        ::wprintw( win, reg.c_str() );
                        
                        y++;
                    }
                }
            }
            
            this->_screen.refresh();
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_drawStack( void )
    {
        if( this->_screen.width() < 190 || this->_screen.height() < 25 )
        {
            return;
        }
        
        {
            ::WINDOW * win( ::newwin( 22, numeric_cast< int >( this->_screen.width() ) - 30, 3, 30 ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Stack:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, numeric_cast< int >( this->_screen.width() ) - 32 );
            }
            
            {
                ::wmove( win, 3, 2 );
                ::wprintw( win, "SS:BP:                | Ret SS:BP:            | Ret CS:EIP:           | Arg 0:     | Arg 1:     | Arg 2:     | Arg 3:     | CS:EIP:" );
                ::wmove( win, 4, 1 );
                ::whline( win, 0, numeric_cast< int >( this->_screen.width() ) - 32 );
            }
            
            {
                std::vector< VM::StackEntry > stack( this->_monitor.stack() );
                int                           y( 5 );
                
                for( size_t i = 0; i < stack.size(); i++ )
                {
                    if( i == 16 )
                    {
                        break;
                    }
                    
                    ::wmove( win, y, 2 );
                    ::wprintw
                    (
                        win,
                        "%s | %s | %s | %s | %s | %s | %s | %s",
                        stack[ i ].bp().string().c_str(),
                        stack[ i ].retBP().string().c_str(),
                        stack[ i ].retIP().string().c_str(),
                        String::toHex( stack[ i ].arg0() ).c_str(),
                        String::toHex( stack[ i ].arg1() ).c_str(),
                        String::toHex( stack[ i ].arg2() ).c_str(),
                        String::toHex( stack[ i ].arg3() ).c_str(),
                        stack[ i ].ip().string().c_str()
                    );
                    
                    y++;
                }
            }
            
            this->_screen.refresh();
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_drawMemory( void )
    {
        if( this->_screen.height() < 35 )
        {
            return;
        }
        
        {
            ::WINDOW * win( ::newwin( numeric_cast< int >( this->_screen.height() ) - 25, numeric_cast< int >( this->_screen.width() ), 25, 0 ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Memory:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, numeric_cast< int >( this->_screen.width() ) - 2 );
            }
            
            {
                std::shared_ptr< VM::CoreDump > dump( this->_monitor.dump() );
                
                if( dump != nullptr )
                {
                    int y( 3 );
                    
                    ::wmove( win, y, 2 );
                    ::wprintw( win, "Core dump is available..." );
                }
            }
            
            this->_screen.refresh();
            ::wrefresh( win );
            ::delwin( win );
        }
    }
}
