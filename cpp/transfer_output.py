import os
import sys

from shutil import copyfile

def main(input: str, output: str, ext: str, transfer: str):
    """
    Transfers the files from their folder to the final location
    from where the C++ executable can access it easily
    """

    if not os.path.exists(output):
        os.makedirs(output)

    i = 0

    for file in os.listdir(input):
        if file.endswith(ext):
            i += 1
            print(f"   {i}. {transfer} {file}...")
            copyfile(os.path.join(input, file), os.path.join(output, file))



if __name__ == "__main__":
    transfer_str = "Moving" if sys.argv[1] == "move" else "Copying"

    print(f"-- {transfer_str} files...")
    main(os.path.expanduser(sys.argv[2]), os.path.expanduser(sys.argv[3]), sys.argv[4], transfer_str)
    print("")
