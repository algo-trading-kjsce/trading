import sys
import os

import shutil

build_dir = os.path.dirname(__file__) + "/../build/"
output_dir = os.path.dirname(__file__) + "/../out/"

if not os.path.exists(output_dir):
    os.makedirs(output_dir)


def files_in_dir(src : str):
    for file in os.listdir(src):
        if os.path.isfile(os.path.join(src, file)):
            yield file


def copy_files_of_type(src: str, exts: list):
    for file in files_in_dir(src):
        for ext in exts:
            if file.endswith(ext):
                print(f"-- Python script: Copying {file}")
                shutil.copy(os.path.join(src, file), output_dir)


def copy_python_stuff():
    python_dir = os.path.dirname(sys.argv[2])
    copy_files_of_type(python_dir, [".dll"])

    py_script_dir = os.path.dirname(__file__) + "/../python/"

    copy_files_of_type(py_script_dir, [".py"])


if __name__ == "__main__":
    if sys.platform == "win32":
        if sys.argv[1] == "copy_python_stuff":
            copy_python_stuff()
        elif sys.argv[1] == "copy_files_of_type":
            copy_files_of_type(sys.argv[2], sys.argv[3:])
