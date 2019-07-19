/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Jean-David Gadina - www.xs-labs.com
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

#ifndef VBOX_SCREEN_HPP
#define VBOX_SCREEN_HPP

#include <cstdlib>
#include <functional>
#include <memory>

namespace VBox
{
    class Screen
    {
        public:
            
            Screen( void );
            Screen( const Screen & o );
            Screen( Screen && o ) noexcept;
            ~Screen( void );
            
            Screen & operator =( Screen o );
            
            std::size_t width( void )  const;
            std::size_t height( void ) const;
            
            bool supportsColors( void ) const;
            bool isRunning( void )      const;
            void clear( void )          const;
            void refresh( void )        const;
            
            void start( void );
            void stop( void );
            
            void onResize( const std::function<   void( void ) > & f );
            void onKeyPress( const std::function< void( int key ) > & f );
            void onUpdate( const std::function<   void( void ) > & f );
            
            friend void swap( Screen & o1, Screen & o2 );
            
        private:
            
            class IMPL;
            
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* VBOX_SCREEN_HPP */
