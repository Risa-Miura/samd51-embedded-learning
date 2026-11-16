"""Optional pinned Windows x64 tool setup; does not install drivers or access hardware.

Downloads into this folder's ignored .tools. Microchip/CMSIS packs are installed
separately with Pack Manager, as documented in the workbench setup guide.
"""
from pathlib import Path
import hashlib
import shutil
import sys
import tempfile
import urllib.request
import zipfile

ROOT=Path(__file__).resolve().parent
TOOLS=ROOT/'.tools'
ARCHIVES=[
    ('arm-gnu','https://github.com/adafruit/arduino-board-index/releases/download/build-tools/gcc-arm-none-eabi-9-2019-q4-major-win32.zip',
     '54e0879b78937cf74a548f9fd242cdfd703bec9fe135d2bb4f56acf5c05d6d61','arm-none-eabi-gcc.exe'),
    ('xpack-openocd-0.12.0-7','https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-7/xpack-openocd-0.12.0-7-win32-x64.zip',
     '6bfd3c97135aafef8affc9af1acf34fd0e2b9ca26044506f6abd7f95b7630052','openocd.exe'),
]


def main():
    if sys.platform!='win32':sys.exit('This optional helper is for Windows. Install equivalent GNU/OpenOCD tools for your platform.')
    TOOLS.mkdir(exist_ok=True)
    for name,url,digest,executable in ARCHIVES:
        destination=TOOLS/name
        if destination.exists():
            if not (destination/'bin'/executable).is_file():
                sys.exit(f'Existing partial/unexpected directory retained: {destination}. Inspect it before retrying.')
            print('Retained existing tool:',destination)
            continue
        with tempfile.TemporaryDirectory(dir=TOOLS,prefix='download-') as folder:
            stage=Path(folder)
            archive=stage/'tool.zip'
            print('Downloading:',url)
            urllib.request.urlretrieve(url,archive)
            if hashlib.sha256(archive.read_bytes()).hexdigest()!=digest:
                sys.exit('Archive SHA-256 mismatch; nothing installed.')
            unpack=stage/'unpack';unpack.mkdir()
            with zipfile.ZipFile(archive) as z:
                for entry in z.namelist():
                    if not (unpack/entry).resolve().is_relative_to(unpack.resolve()):
                        sys.exit('Unsafe archive path; nothing installed.')
                z.extractall(unpack)
            candidates=list(unpack.rglob(executable))
            if len(candidates)!=1 or candidates[0].parent.name!='bin':
                sys.exit('Unexpected tool archive layout; nothing installed.')
            shutil.copytree(candidates[0].parent.parent,destination)
        print('Verified and installed locally:',destination)
    print('No drivers, fuses, Flash contents or system PATH were changed.')


if __name__=='__main__':main()
