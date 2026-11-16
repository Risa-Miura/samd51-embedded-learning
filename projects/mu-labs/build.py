"""Dependency-free bare-metal C build. Does not program a device.

Python 3.10+, Arm GNU toolchain, Microchip SAMD51_DFP 3.10.283 and CMSIS 6.4.0.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
LABS = [f"lab-{n}" for n in range(1, 6)]


def check_image(elf, binary):
    raw = elf.read_bytes()
    if raw[:7] != b"\x7fELF\x01\x01\x01":
        raise ValueError("Expected a little-endian ELF32 image")
    if struct.unpack_from("<H", raw, 18)[0] != 40:
        raise ValueError("ELF is not ARM")
    phoff = struct.unpack_from("<I", raw, 28)[0]
    phsize, phcount = struct.unpack_from("<HH", raw, 42)
    regions = []
    segments = []
    for i in range(phcount):
        kind, offset, vaddr, paddr, size, memsize, flags, alignment = struct.unpack_from("<8I", raw, phoff+i*phsize)
        if kind == 1 and size:
            if paddr < 0x4000 or paddr+size > 0x100000:
                raise ValueError(f"Load image overlaps bootloader, configuration, or invalid memory: {paddr:#x}")
            regions.append({"load_address":hex(paddr), "size":size})
            segments.append((paddr,raw[offset:offset+size]))
    image = binary.read_bytes()
    if not regions or min(int(r["load_address"],16) for r in regions) != 0x4000:
        raise ValueError("Application must start at 0x4000")
    end=max(address+len(content) for address,content in segments)
    expected=bytearray(end-0x4000)
    for address,content in segments:
        expected[address-0x4000:address-0x4000+len(content)]=content
    if image != expected:
        raise ValueError("BIN does not match the ELF load segments")
    sp, reset = struct.unpack_from("<II", image)
    if not (0x20000000 < sp <= 0x2003fff8 and sp % 8 == 0):
        raise ValueError("Invalid initial stack")
    if not (reset & 1 and 0x4000 <= (reset & ~1) < 0x4000+len(image)):
        raise ValueError("Invalid Thumb reset vector")
    return {"load_regions":regions, "initial_sp":hex(sp), "reset_vector":hex(reset),
            "bin_bytes":len(image), "bin_sha256":hashlib.sha256(image).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("lab", choices=LABS+["all"])
    local_gcc=ROOT/".tools/arm-gnu/bin/arm-none-eabi-gcc.exe"
    parser.add_argument("--gcc", default=os.environ.get("ARM_GCC", str(local_gcc) if local_gcc.exists() else "arm-none-eabi-gcc"))
    parser.add_argument("--packs", type=Path, default=Path.home()/".mchp_packs")
    parser.add_argument("--closed-loop", action="store_true")
    parser.add_argument("--button-gated", action="store_true", help="Lab 1: blink only while the debounced button is held")
    args = parser.parse_args()
    gcc = shutil.which(args.gcc)
    if not gcc:
        parser.error("Arm GNU compiler not found. Install it or pass --gcc <full path to arm-none-eabi-gcc.exe>.")
    gcc = Path(gcc).resolve()
    suffix = ".exe" if gcc.suffix.lower()==".exe" else ""
    objcopy = gcc.with_name("arm-none-eabi-objcopy"+suffix)
    size = gcc.with_name("arm-none-eabi-size"+suffix)
    dfp = args.packs/"Microchip/SAMD51_DFP/3.10.283/samd51a/include"
    cmsis = args.packs/"ARM/CMSIS/6.4.0/CMSIS/Core/Include"
    for path in [objcopy, size, dfp/"samd51p20a.h", cmsis/"core_cm4.h"]:
        if not path.exists(): parser.error(f"Missing dependency: {path}")
    version = subprocess.check_output([str(gcc), "--version"], text=True).splitlines()[0]
    commands = []
    for lab in LABS if args.lab=="all" else [args.lab]:
        variant = "closed" if args.closed_loop else "open"
        if args.button_gated and lab=="lab-1": variant += "-button"
        out = ROOT/"build"/lab/variant
        out.mkdir(parents=True,exist_ok=True)
        sources = [ROOT/"shared/startup.c", ROOT/"shared/workbench.c", ROOT/lab/"main.c"]
        flags = ["-mcpu=cortex-m4", "-mthumb", "-mfloat-abi=soft", "-std=c11", "-Og", "-g3",
                 "-Wall", "-Wextra", "-Werror", "-ffreestanding", "-fno-builtin",
                 "-ffunction-sections", "-fdata-sections", "-fno-unwind-tables", "-fno-asynchronous-unwind-tables",
                 "-D__SAMD51P20A__", f"-DLAB_CLOSED_LOOP={int(args.closed_loop)}",
                 f"-DLAB_BUTTON_GATED={int(args.button_gated)}", "-I"+str(dfp), "-I"+str(cmsis), "-I"+str(ROOT/"shared")]
        objects = []
        for src in sources:
            obj = out/(src.stem+".o")
            command = [str(gcc), *flags, "-c", str(src), "-o", str(obj)]
            commands.append({"directory":str(ROOT), "file":str(src), "arguments":command})
            subprocess.run(command,check=True)
            objects.append(str(obj))
        elf = out/"firmware.elf"
        binary = out/"firmware.bin"
        subprocess.run([str(gcc), "-mcpu=cortex-m4", "-mthumb", "-mfloat-abi=soft", "-nostdlib",
                        *objects, "-T"+str(ROOT/"shared/bootloader-app.ld"),
                        "-Wl,--gc-sections,--fatal-warnings,--build-id=none,-Map="+str(out/"firmware.map"),
                        "-lgcc", "-o", str(elf)], check=True)
        subprocess.run([str(objcopy), "-O", "binary", str(elf), str(binary)],check=True)
        subprocess.run([str(size), str(elf)],check=True)
        record = {"lab":lab, "variant":variant, "compiler":version, "dfp":"3.10.283", "cmsis":"6.4.0",
                  "hardware_tested":False, "image":check_image(elf,binary),
                  "source_sha256":{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}}
        (out/"build-record.json").write_text(json.dumps(record,indent=2)+"\n",encoding="utf-8")
        print(f"PASS: {lab}/{variant}: image begins at 0x4000, valid vectors, no bootloader/user-row load segment")
    (ROOT/"compile_commands.json").write_text(json.dumps(commands,indent=2)+"\n",encoding="utf-8")


if __name__=="__main__":
    try: main()
    except (subprocess.CalledProcessError, ValueError) as error:
        sys.exit(str(error))
