/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "verilog/dv/caravel/defs.h"

/*
	IO Test:
		- Configures MPRJ lower 8-IO pins as outputs
		- Observes counter value through the MPRJ lower 8 IO pins (in the testbench)
*/

void main()
{
	/* 
	IO Control Registers
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 3-bits | 1-bit | 1-bit | 1-bit  | 1-bit  | 1-bit | 1-bit   | 1-bit   | 1-bit | 1-bit | 1-bit   |

	Output: 0000_0110_0000_1110  (0x1808) = GPIO_MODE_USER_STD_OUTPUT
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 110    | 0     | 0     | 0      | 0      | 0     | 0       | 1       | 0     | 0     | 0       |
	
	 
	Input: 0000_0001_0000_1111 (0x0402) = GPIO_MODE_USER_STD_INPUT_NOPULL
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 001    | 0     | 0     | 0      | 0      | 0     | 0       | 0       | 0     | 1     | 0       |

	*/

    // 8 inputs for Z80 Address Bus
	reg_mprj_io_8 =   GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_9 =   GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_10 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_11 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_12 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_13 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_14 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_15 =  GPIO_MODE_USER_STD_INPUT_NOPULL;

	// 8 I/O pins for Z80 Data Bus
	reg_mprj_io_16 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_17 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_18 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_19 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_20 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_21 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_22 = GPIO_MODE_USER_STD_BIDIRECTIONAL;
	reg_mprj_io_23 = GPIO_MODE_USER_STD_BIDIRECTIONAL;

	// 3 control signals

    // 3 outputs for PWM, starting at 8
	reg_mprj_io_24 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_25 =  GPIO_MODE_USER_STD_INPUT_NOPULL;
	reg_mprj_io_26 =  GPIO_MODE_USER_STD_OUTPUT;

    /* Apply configuration */
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);

    // reset design with 0bit of 1st bank of LA
    reg_la0_data = 1;
    reg_la0_oenb = 0;
    reg_la0_iena = 0;
    reg_la0_data = 0;
    reg_la0_data = 1;

    // Write to wishbone address

    volatile uint32_t* const p_z80_base_address = (volatile uint32_t*) 0x30000000;
    volatile uint32_t* const p_data = (volatile uint32_t*) 0x30000004;
    volatile uint32_t* const p_status = (volatile uint32_t*) 0x30000008;

    *p_z80_base_address = 0x81;

    *p_data = 0x55;
    *p_status = 0x01;
}

