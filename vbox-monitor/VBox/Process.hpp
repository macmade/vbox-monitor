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

#ifndef VBOX_PROCESS_HPP
#define VBOX_PROCESS_HPP

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace VBox
{
    class Process
    {
        public:
            
            Process( const std::string & path, const std::vector< std::string > & args = {}, const std::vector< std::string > & env = {} );
            ~Process( void );
            
            Process( const Process & ) = delete;
            Process( Process && )      = delete;
            
            Process & operator =( const Process & ) = delete;
            Process & operator =( Process && )      = delete;
            
            std::vector< std::string > arguments( void )   const;
            std::vector< std::string > environment( void ) const;
            
            void arguments( const std::vector< std::string > & args );
            void environment( const std::vector< std::string > & env );
            
            std::optional< pid_t >       pid( void )               const;
            std::optional< int >         terminationStatus( void ) const;
            std::optional< std::string > output( void )            const;
            std::optional< std::string > error( void )             const;
            
            void start( void );
            void waitUntilExit( void );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* VBOX_PROCESS_HPP */
