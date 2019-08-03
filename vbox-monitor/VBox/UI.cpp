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

#include "VBox/UI.hpp"
#include "VBox/Screen.hpp"
#include "VBox/Window.hpp"
#include "VBox/String.hpp"
#include "VBox/Monitor.hpp"
#include "VBox/Casts.hpp"
#include "VBox/Capstone.hpp"
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
            void _drawDisassembly( void );
            void _drawMemory( void );
            
            void _memoryScrollUp( size_t n = 1 );
            void _memoryScrollDown( size_t n = 1 );
            void _memoryPageUp( void );
            void _memoryPageDown( void );
            
            bool                            _running;
            bool                            _paused;
            std::string                     _vmName;
            Monitor                         _monitor;
            size_t                          _memoryOffset;
            size_t                          _memoryBytesPerLine;
            size_t                          _memoryLines;
            size_t                          _totalMemory;
            std::optional< VM::Registers >  _registers;
            std::vector< VM::StackEntry >   _stack;
            std::shared_ptr< VM::CoreDump > _dump;
            std::optional< std::string >    _memoryAddressPrompt;
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
        Screen::shared().start();
    }
    
    void swap( UI & o1, UI & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    UI::IMPL::IMPL( const std::string & vmName ):
        _running(            false ),
        _paused(             false ),
        _vmName(             vmName ),
        _monitor(            vmName ),
        _memoryOffset(       0 ),
        _memoryBytesPerLine( 0 ),
        _memoryLines(        0 ),
        _totalMemory(        0 )
    {
        this->_setup();
    }
    
    UI::IMPL::IMPL( const IMPL & o ):
        _running(            false ),
        _paused(             o._paused ),
        _vmName(             o._vmName ),
        _monitor(            o._monitor ),
        _memoryOffset(       o._memoryOffset ),
        _memoryBytesPerLine( o._memoryBytesPerLine ),
        _memoryLines(        o._memoryLines ),
        _totalMemory(        o._totalMemory ),
        _registers(          o._registers ),
        _stack(              o._stack ),
        _dump(               o._dump )
    {
        this->_setup();
    }
    
    void UI::IMPL::_setup( void )
    {
        Screen::shared().onUpdate
        (
            [ & ]( void )
            {
                if( this->_paused == false )
                {
                    this->_registers = this->_monitor.registers();
                    this->_stack     = this->_monitor.stack();
                    this->_dump      = this->_monitor.dump();
                }
                
                this->_drawTitle();
                this->_drawRegisters();
                this->_drawStack();
                this->_drawDisassembly();
                this->_drawMemory();
                
                if( this->_monitor.live() == false )
                {
                    this->_monitor.stop();
                    Screen::shared().stop();
                }
            }
        );
        
        Screen::shared().onKeyPress
        (
            [ & ]( int key )
            {
                if( key == 'q' )
                {
                    this->_monitor.stop();
                    Screen::shared().stop();
                }
                else if( key == 'm' )
                {
                    if( this->_memoryAddressPrompt.has_value() )
                    {
                        this->_memoryAddressPrompt = {};
                    }
                    else
                    {
                        this->_memoryAddressPrompt = "";
                    }
                }
                else if( ( key == 10 || key == 13 ) && this->_memoryAddressPrompt.has_value() )
                {
                    std::string prompt( this->_memoryAddressPrompt.value() );
                    
                    if( prompt.length() > 0 )
                    {
                        this->_memoryOffset = String::fromHex< size_t >( prompt );
                    }
                    
                    this->_memoryAddressPrompt = {};
                }
                else if( key == 127 && this->_memoryAddressPrompt.has_value() )
                {
                    std::string prompt( this->_memoryAddressPrompt.value() );
                    
                    if( prompt.length() > 0 )
                    {
                        this->_memoryAddressPrompt = prompt.substr( 0, prompt.length() - 1 );
                    }
                }
                else
                {
                    if( this->_memoryAddressPrompt.has_value() && key >= 0 && key < 128 && isprint( key ) )
                    {
                        this->_memoryAddressPrompt = this->_memoryAddressPrompt.value() + numeric_cast< char >( key );
                    }
                    else if( key == 'a' )
                    {
                        this->_memoryScrollUp();
                    }
                    else if( key == 's' )
                    {
                        this->_memoryScrollDown();
                    }
                    else if( key == 'd' )
                    {
                        this->_memoryPageUp();
                    }
                    else if( key == 'f' )
                    {
                        this->_memoryPageDown();
                    }
                    else if( key == 'g' )
                    {
                        this->_memoryOffset = 0;
                    }
                    else if( key == 'p' )
                    {
                        this->_paused = ( this->_paused ) ? false : true;
                    }
                }
            }
        );
    }
    
    void UI::IMPL::_drawTitle( void )
    {
        Window win( 0, 0, Screen::shared().width(), 3 );
        
        {
            win.box();
            win.move( 2, 1 );
            win.print( "VirtualBox: ");
            win.print( this->_vmName );
            
            if( this->_paused )
            {
                win.print( Color::red(), " [PAUSED]" );
            }
        }
        
        Screen::shared().refresh();
        win.refresh();
    }
    
    void UI::IMPL::_drawRegisters( void )
    {
        if( Screen::shared().width() < 30 || Screen::shared().height() < 25 )
        {
            return;
        }
        
        {
            Window win( 0, 3, 30, 22 );
            
            {
                win.box();
                win.move( 2, 1 );
                win.print( Color::blue(), "CPU Registers:" );
                win.move( 1, 2 );
                win.addHorizontalLine( 28 );
            }
            
            {
                
                std::optional< VM::Registers > regs( this->_registers );
                
                if( regs.has_value() )
                {
                    size_t y( 3 );
                    
                    for( const auto & p: regs.value().all() )
                    {
                        std::string reg( String::toUpper( p.first ) );
                        
                        for( size_t i = reg.size(); i < 6; i++ )
                        {
                            reg = " " + reg;
                        }
                        
                        win.move( 2, y );
                        win.print( Color::cyan(), reg );
                        win.print( ": " );
                        win.print( Color::yellow(), String::toHex( p.second ) );
                        
                        y++;
                    }
                }
            }
            
            Screen::shared().refresh();
            win.refresh();
        }
    }
    
    void UI::IMPL::_drawStack( void )
    {
        if( Screen::shared().width() < 180 || Screen::shared().height() < 25 )
        {
            return;
        }
        
        {
            Window win( 30, 3, 150, 22 );
            
            {
                win.box();
                win.move( 2, 1 );
                win.print( Color::blue(), "Stack:" );
                win.move( 1, 2 );
                win.addHorizontalLine( 148 );
            }
            
            {
                win.move( 2, 3 );
                win.print( Color::blue(), "SS:BP:                " );
                win.print( "| " );
                win.print( Color::blue(), "Ret SS:BP:            " );
                win.print( "| " );
                win.print( Color::blue(), "Ret CS:EIP:           " );
                win.print( "| " );
                win.print( Color::blue(), "Arg 0:     " );
                win.print( "| " );
                win.print( Color::blue(), "Arg 1:     " );
                win.print( "| " );
                win.print( Color::blue(), "Arg 2:     " );
                win.print( "| " );
                win.print( Color::blue(), "Arg 3:     " );
                win.print( "| " );
                win.print( Color::blue(), "CS:EIP:" );
                win.move( 1, 4 );
                win.addHorizontalLine( 148 );
            }
            
            {
                std::vector< VM::StackEntry > stack( this->_stack );
                size_t                        y( 5 );
                
                for( size_t i = 0; i < stack.size(); i++ )
                {
                    if( i == 16 )
                    {
                        break;
                    }
                    
                    win.move( 2, y );
                    
                    win.print( Color::cyan(), String::toHex( stack[ i ].bp().segment() ) );
                    win.print( ":" );
                    win.print( Color::yellow(), String::toHex( stack[ i ].bp().address() ) );
                    win.print( " | " );
                    
                    win.print( Color::cyan(), String::toHex( stack[ i ].retBP().segment() ) );
                    win.print( ":" );
                    win.print( Color::yellow(), String::toHex( stack[ i ].retBP().address() ) );
                    win.print( " | " );
                    
                    win.print( Color::cyan(), String::toHex( stack[ i ].retIP().segment() ) );
                    win.print( ":" );
                    win.print( Color::yellow(), String::toHex( stack[ i ].retIP().address() ) );
                    win.print( " | " );
                    
                    win.print( Color::yellow(), String::toHex( stack[ i ].arg0() ) );
                    win.print( " | " );
                    win.print( Color::yellow(), String::toHex( stack[ i ].arg1() ) );
                    win.print( " | " );
                    win.print( Color::yellow(), String::toHex( stack[ i ].arg2() ) );
                    win.print( " | " );
                    win.print( Color::yellow(), String::toHex( stack[ i ].arg3() ) );
                    win.print( " | " );
                    
                    win.print( Color::cyan(), String::toHex( stack[ i ].ip().segment() ) );
                    win.print( ":" );
                    win.print( Color::yellow(), String::toHex( stack[ i ].ip().address() ) );
                    
                    y++;
                }
            }
            
            Screen::shared().refresh();
            win.refresh();
        }
    }
    
    void UI::IMPL::_drawDisassembly( void )
    {
        if( Screen::shared().width() < 220 || Screen::shared().height() < 25 )
        {
            return;
        }
        
        {
            Window win( 180, 3, Screen::shared().width() - 180, 22 );
            
            {
                win.box();
                win.move( 2, 1 );
                win.print( Color::blue(), "Disassembly:" );
                win.move( 1, 2 );
                win.addHorizontalLine( Screen::shared().width() - 182 );
            }
            
            {
                std::shared_ptr< VM::CoreDump > dump( this->_dump );
                std::optional< VM::Registers >  regs( this->_registers );
                
                if( dump != nullptr && dump->memorySize() > 0 && regs.has_value() )
                {
                    std::vector< uint8_t > code( dump->readMemory( regs.value().rip(), 512 ) );
                    
                    if( code.size() > 0 )
                    {
                        size_t y( 2 );
                        
                        for( const auto & p: Capstone::disassemble( code, regs.value().rip() ) )
                        {
                            if( y > 19 )
                            {
                                break;
                            }
                            
                            win.move( 2, ++y );
                            win.print( Color::cyan(), p.first );
                            win.print( ": " );
                            win.print( Color::yellow(), p.second );
                        }
                    }
                }
            }
            
            Screen::shared().refresh();
            win.refresh();
        }
    }
    
    void UI::IMPL::_drawMemory( void )
    {
        if( Screen::shared().width() < 30 || Screen::shared().height() < 35 )
        {
            return;
        }
        
        {
            Window win( 0, 25, Screen::shared().width(), Screen::shared().height() - 25 );
            
            {
                win.box();
                win.move( 2, 1 );
                win.print( Color::blue(), "Memory:" );
                win.move( 1, 2 );
                win.addHorizontalLine( Screen::shared().width() - 2 );
            }
            
            if( this->_memoryAddressPrompt.has_value() )
            {
                win.move( 2, 3 );
                win.print( Color::cyan(), "Enter a memory address:" );
                win.move( 2, 4 );
                win.print( Color::yellow(), this->_memoryAddressPrompt.value() );
            }
            else
            {
                std::shared_ptr< VM::CoreDump > dump( this->_dump );
                
                if( dump != nullptr && dump->memorySize() > 0 )
                {
                    size_t y( 2 );
                    size_t cols(  Screen::shared().width()  - 4 );
                    size_t lines( Screen::shared().height() - 29 );
                    
                    this->_totalMemory        = dump->memorySize();
                    this->_memoryBytesPerLine = ( cols / 4 ) - 5;
                    this->_memoryLines        = lines;
                    
                    {
                        size_t                 size(  this->_memoryBytesPerLine * lines );
                        size_t                 offset( this->_memoryOffset );
                        std::vector< uint8_t > mem(   dump->readMemory( offset, size ) );
                        
                        for( size_t i = 0; i < mem.size(); i++ )
                        {
                            if( i % this->_memoryBytesPerLine == 0 )
                            {
                                win.move( 2, ++y );
                                win.print( Color::yellow(), "%016X: ", offset );
                                
                                offset += this->_memoryBytesPerLine;
                            }
                            
                            win.print( Color::cyan(), "%02X ", numeric_cast< int >( mem[ i ] ) );
                        }
                        
                        y = 2;
                        
                        win.move( ( this->_memoryBytesPerLine * 3 ) + 4 + 16, y + 1 );
                        win.addVerticalLine( lines );
                        
                        for( size_t i = 0; i < mem.size(); i++ )
                        {
                            char c = static_cast< char >( mem[ i ] );
                            
                            if( i % this->_memoryBytesPerLine == 0 )
                            {
                                win.move( ( this->_memoryBytesPerLine * 3 ) + 4 + 18, ++y );
                            }
                            
                            if( isprint( c ) == false || isspace( c ) )
                            {
                                win.print( Color::blue(), "." );
                            }
                            else
                            {
                                win.print( "%c", c );
                            }
                        }
                    }
                }
            }
            
            Screen::shared().refresh();
            win.refresh();
        }
    }
    
    void UI::IMPL::_memoryScrollUp( size_t n )
    {
        if( this->_memoryOffset > ( this->_memoryBytesPerLine * n ) )
        {
            this->_memoryOffset -= ( this->_memoryBytesPerLine * n );
        }
        else
        {
            this->_memoryOffset = 0;
        }
    }
    
    void UI::IMPL::_memoryScrollDown( size_t n )
    {
        if( ( this->_memoryOffset + ( this->_memoryBytesPerLine * n ) ) < this->_totalMemory )
        {
            this->_memoryOffset += ( this->_memoryBytesPerLine * n );
        }
    }
    
    void UI::IMPL::_memoryPageUp( void )
    {
        this->_memoryScrollUp( this->_memoryLines );
    }
    
    void UI::IMPL::_memoryPageDown( void )
    {
        this->_memoryScrollDown( this->_memoryLines );
    }
}
