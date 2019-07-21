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

#include "Arguments.hpp"
#include "UI.hpp"
#include <iostream>
#include <cstdlib>

void ShowHelp( void );

int main( int argc, const char * argv[] )
{
    VBox::Arguments args( argc, argv );
    
    if( args.showHelp() || args.vmName().length() == 0 )
    {
        ShowHelp();
        
        return EXIT_SUCCESS;
    }
    
    VBox::UI( args.vmName() ).run();
    
    return EXIT_SUCCESS;
}

void ShowHelp( void )
{
    std::cout << "Usage: vbox-monitor VM_NAME"
              << std::endl
              << std::endl
              << "Shortcuts:"
              << std::endl
              << "    - p: Pause/Resume"
              << std::endl
              << "    - a: Scroll memory up (one line)"
              << std::endl
              << "    - s: Scroll memory down (one line)"
              << std::endl
              << "    - d: Scroll memory up (one page)"
              << std::endl
              << "    - f: Scroll memory down (one page)"
              << std::endl
              << "    - g: Scroll memory top top"
              << std::endl
              << "    - m: Enter a memory location"
              << std::endl;
}
