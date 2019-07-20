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

#include "Monitor.hpp"
#include "Manage.hpp"
#include <mutex>
#include <thread>
#include <optional>

namespace VBox
{
    class Monitor::IMPL
    {
        public:
            
            IMPL( const std::string & vmName );
            IMPL( const IMPL & o );
            IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l );
            
            void _updateRegisters( void );
            void _updateStack( void );
            void _updateMemory( void );
            
            std::string                   _vmName;
            VM::Registers                 _registers;
            std::vector< VM::StackEntry > _stack;
            mutable std::recursive_mutex  _rmtx;
            bool                          _running;
            bool                          _stop;
            std::vector< std::thread >    _threads;
    };
    
    Monitor::Monitor( const std::string & vmName ):
        impl( std::make_unique< IMPL >( vmName ) )
    {}
    
    Monitor::Monitor( const Monitor & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    Monitor::Monitor( Monitor && o )
    {
        std::lock_guard< std::recursive_mutex >( o.impl->_rmtx );
        
        this->impl = std::move( o.impl );
    }
    
    Monitor::~Monitor( void )
    {}
    
    Monitor & Monitor::operator =( Monitor o )
    {
        std::lock( this->impl->_rmtx, o.impl->_rmtx );
        
        {
            std::lock_guard< std::recursive_mutex > l1( this->impl->_rmtx, std::adopt_lock );
            std::lock_guard< std::recursive_mutex > l2( o.impl->_rmtx,     std::adopt_lock );
            
            swap( *( this ), o );
            
            return *( this );
        }
    }
    
    VM::Registers Monitor::registers( void ) const
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_registers;
    }
    
    std::vector< VM::StackEntry > Monitor::stack( void )const
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_stack;
    }
    
    void Monitor::start( void )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        if( this->impl->_running )
        {
            return;
        }
        
        this->impl->_stop    = false;
        this->impl->_running = true;
        
        {
            std::thread t1( [ this ] { this->impl->_updateRegisters();  } );
            std::thread t2( [ this ] { this->impl->_updateStack(); } );
            std::thread t3( [ this ] { this->impl->_updateMemory(); } );
            
            this->impl->_threads.push_back( std::move( t1 ) );
            this->impl->_threads.push_back( std::move( t2 ) );
            this->impl->_threads.push_back( std::move( t3 ) );
        }
    }
    
    void Monitor::stop( void )
    {
        std::vector< std::thread > threads;
        
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            threads           = std::move( this->impl->_threads );
            this->impl->_stop = true;
        }
        
        for( auto & t: threads )
        {
            t.join();
        }
        
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            this->impl->_running = false;
        }
    }
    
    void swap( Monitor & o1, Monitor & o2 )
    {
        using std::swap;
        
        std::lock( o1.impl->_rmtx, o2.impl->_rmtx );
        
        {
            std::lock_guard< std::recursive_mutex > l1( o1.impl->_rmtx, std::adopt_lock );
            std::lock_guard< std::recursive_mutex > l2( o2.impl->_rmtx, std::adopt_lock );
            
            swap( o1.impl, o2.impl );
        }
    }
    
    Monitor::IMPL::IMPL( const std::string & vmName ):
        _vmName(         vmName ),
        _running(        false ),
        _stop(           false )
    {}
    
    Monitor::IMPL::IMPL( const IMPL & o ):
        IMPL( o, std::lock_guard< std::recursive_mutex >( o._rmtx ) )
    {}
    
    Monitor::IMPL::IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l ):
        _vmName(         o._vmName ),
        _registers(      o._registers ),
        _stack(          o._stack ),
        _running(        false ),
        _stop(           false )
    {
        ( void )l;
    }
    
    void Monitor::IMPL::_updateRegisters( void )
    {
        while( 1 )
        {
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                if( this->_stop )
                {
                    return;
                }
            }
            
            {
                VM::Registers regs( Manage::registers( this->_vmName ) );
                
                {
                    std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                    
                    this->_registers = regs;
                }
            }
        }
    }
    
    void Monitor::IMPL::_updateStack( void )
    {
        while( 1 )
        {
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                if( this->_stop )
                {
                    return;
                }
            }
            
            {
                std::vector< VM::StackEntry > stack( Manage::stack( this->_vmName ) );
                
                {
                    std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                    
                    this->_stack = stack;
                }
            }
        }
    }
    
    void Monitor::IMPL::_updateMemory( void )
    {
        while( 1 )
        {
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                if( this->_stop )
                {
                    return;
                }
            }
        }
    }
}
