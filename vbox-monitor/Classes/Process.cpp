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
            
            std::string                  _path;
            std::vector< std::string >   _args;
            std::vector< std::string >   _env;
            std::optional< pid_t >       _pid;
            std::optional< int >         _terminationStatus;
            std::optional< std::string > _output;
            int                          _fd[ 2 ];
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
        
        if( pipe( this->impl->_fd ) == -1 )
        {
            throw std::runtime_error( "Cannot create pipe" );
        }
        
        this->impl->_pid = fork();
        
        if( this->impl->_pid.value() == -1 )
        {
            throw std::runtime_error( "Cannot fork process" );
        }
        else if( this->impl->_pid.value() > 0 )
        {
            close( this->impl->_fd[ 1 ] );
        }
        else if( this->impl->_pid.value() == 0 )
        {
            std::vector< char * > args;
            std::vector< char * > env;
            
            dup2(  this->impl->_fd[ 1 ], STDOUT_FILENO );
            close( this->impl->_fd[ 0 ] );
            close( this->impl->_fd[ 1 ] );
            
            args.push_back( strdup( this->impl->_path.c_str() ) );
            
            for( const auto & s: this->impl->_args )
            {
                args.push_back( strdup( s.c_str() ) );
            }
            
            for( const auto & s: this->impl->_env )
            {
                args.push_back( strdup( s.c_str() ) );
            }
            
            args.push_back( nullptr );
            env.push_back( nullptr );
            
            execve( this->impl->_path.c_str(), &( args[ 0 ] ), &( args[ 0 ] ) );
            
            for( auto cp: args )
            {
                free( cp );
            }
            
            for( auto cp: env )
            {
                free( cp );
            }
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
    
    std::optional< std::string > Process::output( void ) const
    {
        if( this->impl->_terminationStatus.has_value() == false )
        {
            return {};
        }
        
        if( this->impl->_output.has_value() )
        {
            return this->impl->_output.value();
        }
        
        std::string out;
        char        buf[ 1024 ];
        ssize_t     n;
        
        while( ( n = read( this->impl->_fd[ 0 ], buf, sizeof( buf ) ) ) > 0 )
        {
            out += buf;
        }
        
        close( this->impl->_fd[ 0 ] );
        
        this->impl->_output = out;
        
        return out;
    }
    
    Process::IMPL::IMPL( const std::string & path, const std::vector< std::string > & args, const std::vector< std::string > & env ):
        _path( path ),
        _args( args ),
        _env(  env )
    {}
}
