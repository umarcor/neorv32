// #################################################################################################
// # << NEORV32 - Newlib Demo/Test Program >>                                                      #
// # ********************************************************************************************* #
// # BSD 3-Clause License                                                                          #
// #                                                                                               #
// # Copyright (c) 2022, Stephan Nolting. All rights reserved.                                     #
// #                                                                                               #
// # Redistribution and use in source and binary forms, with or without modification, are          #
// # permitted provided that the following conditions are met:                                     #
// #                                                                                               #
// # 1. Redistributions of source code must retain the above copyright notice, this list of        #
// #    conditions and the following disclaimer.                                                   #
// #                                                                                               #
// # 2. Redistributions in binary form must reproduce the above copyright notice, this list of     #
// #    conditions and the following disclaimer in the documentation and/or other materials        #
// #    provided with the distribution.                                                            #
// #                                                                                               #
// # 3. Neither the name of the copyright holder nor the names of its contributors may be used to  #
// #    endorse or promote products derived from this software without specific prior written      #
// #    permission.                                                                                #
// #                                                                                               #
// # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS   #
// # OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               #
// # MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    #
// # COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     #
// # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE #
// # GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED    #
// # AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     #
// # NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED  #
// # OF THE POSSIBILITY OF SUCH DAMAGE.                                                            #
// # ********************************************************************************************* #
// # The NEORV32 Processor - https://github.com/stnolting/neorv32              (c) Stephan Nolting #
// #################################################################################################


/**********************************************************************//**
 * @file newlib_demo/main.c
 * @author Stephan Nolting
 * @brief Demo/test program for NEORV32's newlib C standard library support.
 **************************************************************************/
#include <neorv32.h>
#include <unistd.h>
#include <stdlib.h>


/**********************************************************************//**
 * @name User configuration
 **************************************************************************/
/**@{*/
/** UART BAUD rate */
#define BAUD_RATE 19200
/**@}*/


/**********************************************************************//**
 * Main function: Check some of newlib's core functions.
 *
 * @note This program requires UART.
 *
 * @return 0 if execution was successful
 **************************************************************************/
int main() {

  // setup NEORV32 runtime environment to keep us safe
  // -> catch all traps and give debug information via UART0
  neorv32_rte_setup();

  // setup UART0 at default baud rate, no parity bits, no HW flow control
  neorv32_uart0_setup(BAUD_RATE, PARITY_NONE, FLOW_CONTROL_NONE);

  // check if UART0 is implemented at all
  if (neorv32_uart0_available() == 0) {
    neorv32_uart0_printf("Error! UART0 not synthesized!\n");
    return 1;
  }


  // say hello
  neorv32_uart0_printf("<<< Newlib demo/test program >>>\n\n");


  // check if newlib is really available
#ifndef __NEWLIB__
  neorv32_uart0_printf("ERROR! Seems like the compiler toolchain does not support newlib...\n");
  return -1;
#endif

  neorv32_uart0_printf("newlib version %i.%i\n\n", (int32_t)__NEWLIB__, (int32_t)__NEWLIB_MINOR__);


  char *char_buffer; // pointer for dynamic memory allocation

  neorv32_uart0_printf("<MALLOC> test...\n");
  char_buffer = (char *) malloc(4 * sizeof(char)); // 4 bytes

  neorv32_uart0_printf("<READ> test... (waiting for 4 chars via UART0)\n");
  read((int)STDIN_FILENO, char_buffer, 4 * sizeof(char)); // get 4 chars from "STDIN" (UART0.RX)

  neorv32_uart0_printf("<WRITE> test... (outputting the chars you have send)\n");
  write((int)STDOUT_FILENO, char_buffer, 4 * sizeof(char)); // send 4 chars to "STDOUT" (UART0.TX)
  neorv32_uart0_printf("\n");

  neorv32_uart0_printf("<FREE> test...\n");
  free(char_buffer);


  neorv32_uart0_printf("<EXIT> test...\n");
  exit(0);

  return 0; // should never be reached
}


/**********************************************************************//**
 * "after-main" handler that is executed after the application's
 * main function returns (called by crt0.S start-up code)
 *
 * @note This function should be never executed as we are leaving "main" via "exit" and not "return".
 **************************************************************************/
int __neorv32_crt0_after_main(int32_t return_code) {

  neorv32_uart0_printf("\n<RTE> main function returned with exit code %i. </RTE>\n", return_code);

  return 0;
}