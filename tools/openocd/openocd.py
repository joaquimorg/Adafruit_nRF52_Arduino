#!/usr/bin/env python3

import subprocess
import argparse
import os

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-path', help = 'path for OpenOCD.')
    parser.add_argument('-hex', help = 'Hex file to program via OpenOCD.')
    args = parser.parse_args()

    #print('\nopenOCD !\n')

    #print(args.hex)

    newHexPath = args.hex.replace(os.sep, '/')
    newPath = args.path.replace(os.sep, '/')
    newPath = newPath + '/tools/openocd/xpack-openocd-win32'

    #print(newPath)

    os.popen(newPath + '/bin/openocd.exe -c "tcl_port disabled" -c "gdb_port 3333" -c "telnet_port 4444" -f interface/stlink.cfg -c "transport select hla_swd" -f target/nrf52.cfg -c "program ' + newHexPath + '" -c reset -c shutdown')


if __name__ == "__main__":
    main()