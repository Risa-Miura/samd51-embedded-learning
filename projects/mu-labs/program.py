"""Explicit SWD application programming with bootloader/user-row comparison.

Does nothing without --replace-application. Never use a chip erase/unlock command.
The AD2 must be disconnected as a signal source while the board is unpowered.
"""
import argparse
import hashlib
from pathlib import Path
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from build import check_image

ROOT=Path(__file__).resolve().parent


def tcl_path(path):
    value=path.resolve().as_posix()
    if any(c in value for c in "{}\n\r"):
        raise ValueError("Path contains Tcl delimiter characters")
    return "{"+value+"}"


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("image",type=Path,help="Built firmware.elf; matching .bin is required")
    local_openocd=ROOT/".tools/xpack-openocd-0.12.0-7/bin/openocd.exe"
    parser.add_argument("--openocd",default=str(local_openocd) if local_openocd.exists() else "openocd")
    parser.add_argument("--replace-application",action="store_true",help="Acknowledge replacement of the existing application, not the bootloader")
    args=parser.parse_args()
    if not args.replace_application:
        parser.error("Read the Atmel-ICE guide and supply --replace-application only after checking wiring, power, and the memory layout.")
    tool=shutil.which(args.openocd)
    if not tool: parser.error("OpenOCD not found. Install OpenOCD 0.12.x or pass --openocd <path>.")
    elf=args.image.resolve()
    binary=elf.with_suffix(".bin")
    result=check_image(elf,binary)
    if result["bin_bytes"] > 0x100000-0x4000:
        parser.error("Application exceeds target Flash")
    stamp=datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S%fZ")
    backup=ROOT/"backups"/stamp
    backup.mkdir(parents=True,exist_ok=False)
    boot_before=backup/"bootloader-before.bin"
    boot_after=backup/"bootloader-after.bin"
    user_before=backup/"user-row-before.bin"
    user_after=backup/"user-row-after.bin"
    # Erase only sectors intersecting the application BIN at the aligned 0x4000
    # boundary. Do not add `unlock`, `mass_erase`, `chip-erase`, `protect`, or
    # `atsame5 bootloader <size>` to this sequence.
    commands=["init", "reset halt", "flash probe 0", "check_grand_central_target",
              f"dump_image {tcl_path(boot_before)} 0x0 0x4000",
              f"dump_image {tcl_path(user_before)} 0x00804000 0x200",
              f"flash write_image erase {tcl_path(binary)} 0x4000 bin",
              f"verify_image {tcl_path(binary)} 0x4000 bin",
              f"dump_image {tcl_path(boot_after)} 0x0 0x4000",
              f"dump_image {tcl_path(user_after)} 0x00804000 0x200", "shutdown"]
    command=[tool,"-f",str(ROOT/"debug/atmel-ice.cfg")]
    for item in commands: command += ["-c",item]
    subprocess.run(command,check=True)
    for before,after,expected in [(boot_before,boot_after,0x4000),(user_before,user_after,0x200)]:
        if len(before.read_bytes()) != expected or before.read_bytes()!=after.read_bytes():
            raise RuntimeError(f"Preservation comparison failed: {before}. Keep the target halted and investigate.")
        print(before.name,"unchanged",hashlib.sha256(before.read_bytes()).hexdigest())
    print("Application verified. Bootloader and user row unchanged. Target left halted.")
    print("Start the documented attach/debug session, or reset the board to enter its existing bootloader.")


if __name__=="__main__":
    try: main()
    except (subprocess.CalledProcessError, ValueError, RuntimeError) as error:sys.exit(str(error))
