import cocotb
from cocotb.clock import Clock
from cocotb.binary import BinaryValue
from cocotb.triggers import RisingEdge, FallingEdge, ClockCycles, with_timeout
import random
import test_zube

clocks_per_phase = 10


# takes ~60 seconds on my PC
@cocotb.test()
async def test_start(dut):
    clock = Clock(dut.clk, 25, units="ns")
    cocotb.fork(clock.start())

    dut.RSTB <= 0
    dut.power1 <= 0;
    dut.power2 <= 0;
    dut.power3 <= 0;
    dut.power4 <= 0;

    await ClockCycles(dut.clk, 8)
    dut.power1 <= 1;
    await ClockCycles(dut.clk, 8)
    dut.power2 <= 1;
    await ClockCycles(dut.clk, 8)
    dut.power3 <= 1;
    await ClockCycles(dut.clk, 8)
    dut.power4 <= 1;

    await ClockCycles(dut.clk, 80)
    dut.RSTB <= 1

    dut.z80_address_bus <= 0x0000
    dut.z80_write_strobe_b <= 1
    dut.z80_read_strobe_b <= 1
    dut.z80_data_bus_in <= BinaryValue("zzzzzzzz")

    # wait for reset
    await RisingEdge(dut.RSTB)

@cocotb.test()
async def test_all(dut):
    clock = Clock(dut.clk, 25, units="ns")

    cocotb.fork(clock.start())

    # wait for the reset signal - time out if necessary - should happen around 165us
    await with_timeout(FallingEdge(dut.uut.mprj.zube_wrapper0.reset_b), 1000, 'us')
    await RisingEdge(dut.uut.mprj.zube_wrapper0.reset_b)

    await ClockCycles(dut.clk, 1000)

    # RV32 core resets Z80 address to 0x81 on start-up and writes to both registers

    data_contents = await test_zube.test_z80_get(dut, 0x81)
    assert data_contents == 0x55

    status_contents = await test_zube.test_z80_get(dut, 0x82)
    assert status_contents == 0x01

    print("Test complete")
