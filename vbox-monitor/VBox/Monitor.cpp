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
#include <mutex>
#include <thread>
#include <optional>

namespace VBox
{
    class Monitor::IMPL
    {
        public:
            
            IMPL( const std::string & vmName, std::chrono::milliseconds updateInterval );
            IMPL( const IMPL & o );
            IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l );
            
            void _update( void );
            
            std::string                   _vmName;
            std::chrono::milliseconds     _updateInterval;
            VM::Registers                 _registers;
            std::vector< VM::StackEntry > _stack;
            mutable std::recursive_mutex  _rmtx;
            bool                          _stop;
    };
    
    Monitor::Monitor( const std::string & vmName, std::chrono::milliseconds updateInterval ):
        impl( std::make_unique< IMPL >( vmName, updateInterval ) )
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
    }
    
    void Monitor::stop( void )
    {
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            this->impl->_stop = true;
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
    
    Monitor::IMPL::IMPL( const std::string & vmName, std::chrono::milliseconds updateInterval ):
        _vmName(         vmName ),
        _updateInterval( updateInterval ),
        _stop(           false )
    {}
    
    Monitor::IMPL::IMPL( const IMPL & o ):
        IMPL( o, std::lock_guard< std::recursive_mutex >( o._rmtx ) )
    {}
    
    Monitor::IMPL::IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l ):
        _vmName(         o._vmName ),
        _updateInterval( o._updateInterval ),
        _registers(      o._registers ),
        _stack(          o._stack ),
        _stop(           false )
    {
        ( void )l;
    }
    
    void Monitor::IMPL::_update( void )
    {}
}
