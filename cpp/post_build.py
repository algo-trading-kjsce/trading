import sys
import os

import shutil

if sys.platform == "win32":

    # Copy Python dll
    python_dir = os.path.dirname(sys.argv[1])

    for file in os.listdir(python_dir):

        if file.endswith(".dll"):
            print(f"Copying {file}")

            output_dir = os.path.dirname(__file__) + "/../build/dll/"

            if os.path.exists(output_dir):
                shutil.rmtree(output_dir)

            os.makedirs(output_dir)

            shutil.copy2(os.path.join(python_dir, file), output_dir)
