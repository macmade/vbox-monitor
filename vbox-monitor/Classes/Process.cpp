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

#include "Process.hpp"
#include <unistd.h>

namespace VBox
{
    class Process::IMPL
    {
        public:
            
            IMPL( const std::string & path, const std::vector< std::string > & args, const std::vector< std::string > & env );
            
            std::string                _path;
            std::vector< std::string > _args;
            std::vector< std::string > _env;
            std::optional< pid_t >     _pid;
            std::optional< int >       _terminationStatus;
    };
    
    Process::Process( const std::string & path, const std::vector< std::string > & args, const std::vector< std::string > & env ):
        impl( std::make_unique< IMPL >( path, args, env ) )
    {}
    
    Process::~Process( void )
    {}
    
    std::vector< std::string > Process::arguments( void ) const
    {
        return this->impl->_args;
    }
    
    std::vector< std::string > Process::environment( void ) const
    {
        return this->impl->_env;
    }
    
    void Process::arguments( const std::vector< std::string > & args )
    {
        this->impl->_args = args;
    }
    
    void Process::environment( const std::vector< std::string > & env )
    {
        this->impl->_env = env;
    }
    
    std::optional< pid_t > Process::pid( void ) const
    {
        return this->impl->_pid;
    }
    
    std::optional< int > Process::terminationStatus( void ) const
    {
        return this->impl->_terminationStatus;
    }
    
    void Process::start( void )
    {
        if( this->impl->_pid.has_value() )
        {
            throw std::runtime_error( "Process has already been started" );
        }
        
        this->impl->_pid = fork();
        
        if( this->impl->_pid.value() == -1 )
        {
            throw std::runtime_error( "Cannot fork process" );
        }
        else if( this->impl->_pid.value() == 0 )
        {
            size_t  i;
            char ** args;
            char ** env;
            
            {
                args                                 = new char *[ this->impl->_args.size() + 2 ];
                args[ 0 ]                            = strdup( this->impl->_path.c_str() );
                args[ this->impl->_args.size() + 1 ] = nullptr;
                i                                    = 1;
                
                for( const auto & a: this->impl->_args )
                {
                    args[ ++i ] = strdup( a.c_str() );
                }
            }
            
            {
                env                            = new char *[ this->impl->_env.size() + 1 ];
                env[ this->impl->_env.size() ] = nullptr;
                i                              = 0;
                
                for( const auto & e: this->impl->_env )
                {
                    env[ ++i ] = strdup( e.c_str() );
                }
            }
            
            execve( this->impl->_path.c_str(), args, env );
        }
    }
    
    void Process::waitUntilExit( void )
    {
        int status;
        
        if( this->impl->_pid.has_value() == false || this->impl->_pid.value() <= 0 )
        {
            throw std::runtime_error( "Process is not running" );
        }
        
        waitpid( this->impl->_pid.value(), &status, 0 );
        
        this->impl->_terminationStatus = status;
    }
    
    Process::IMPL::IMPL( const std::string & path, const std::vector< std::string > & args, const std::vector< std::string > & env ):
        _path( path ),
        _args( args ),
        _env(  env )
    {}
}
