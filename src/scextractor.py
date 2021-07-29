# -*- coding: utf-8 -*-

import pefile
import sys
from pathlib import Path


def extract_shellcode(sc_path : Path) -> Path:
    image = pefile.PE(str(sc_path))
    if not image.is_exe() and not image.is_dll():
        raise Exception("The file is not a valid PE file.")
    
    for symbol in image.DIRECTORY_ENTRY_EXPORT.symbols:
        if symbol.name.decode() == "SCBegin":
            sc_begin = symbol.address
        elif symbol.name.decode() == "SCEnd":
            sc_end = symbol.address
    if "sc_begin" not in locals().keys() or "sc_end" not in locals().keys():
        raise Exception("Not found shellcode in the PE file.")
     
    sc_path = sc_path.parent.joinpath(sc_path.stem).with_suffix(".sc")
    sc_path.write_bytes(image.get_data(sc_begin, sc_end - sc_begin))
    return sc_path


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise Exception("Invalid parameters.")
    sc_path = Path(sys.argv[1])
    if not sc_path.is_file():
        raise Exception("Invalid parameters.")
    sc_path = extract_shellcode(sc_path)
    print(f"------ The shellcode extracted successfully and saved {sc_path} ------")
