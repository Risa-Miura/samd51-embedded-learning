"""Meaningful negative checks for the pre-programming image boundary guard."""
import importlib.util
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('lab_build',ROOT/'build.py')
build=importlib.util.module_from_spec(spec)
spec.loader.exec_module(build)


class ImageGuards(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.elf=ROOT/'build/lab-1/open/firmware.elf'
        cls.bin=cls.elf.with_suffix('.bin')
        if not cls.bin.exists():raise unittest.SkipTest('Build lab-1/open first')

    def test_valid_application(self):
        record=build.check_image(self.elf,self.bin)
        self.assertEqual(record['load_regions'][0]['load_address'],'0x4000')

    def test_bootloader_segment_is_rejected(self):
        content=bytearray(self.elf.read_bytes())
        phoff=struct.unpack_from('<I',content,28)[0]
        struct.pack_into('<I',content,phoff+12,0)
        with tempfile.TemporaryDirectory() as folder:
            bad=Path(folder)/'bad.elf';bad.write_bytes(content)
            with self.assertRaisesRegex(ValueError,'overlaps bootloader'):
                build.check_image(bad,self.bin)

    def test_user_row_segment_is_rejected(self):
        content=bytearray(self.elf.read_bytes())
        phoff=struct.unpack_from('<I',content,28)[0]
        struct.pack_into('<I',content,phoff+12,0x00804000)
        with tempfile.TemporaryDirectory() as folder:
            bad=Path(folder)/'bad.elf';bad.write_bytes(content)
            with self.assertRaisesRegex(ValueError,'overlaps bootloader, configuration'):
                build.check_image(bad,self.bin)

    def test_stale_or_modified_bin_is_rejected(self):
        content=bytearray(self.bin.read_bytes());content[-1]^=1
        with tempfile.TemporaryDirectory() as folder:
            bad=Path(folder)/'bad.bin';bad.write_bytes(content)
            with self.assertRaisesRegex(ValueError,'does not match'):
                build.check_image(self.elf,bad)

    def test_program_requires_explicit_application_replacement(self):
        result=subprocess.run([sys.executable,str(ROOT/'program.py'),str(self.elf)],capture_output=True,text=True)
        self.assertEqual(result.returncode,2)
        self.assertIn('--replace-application',result.stderr)


if __name__=='__main__':unittest.main()
