import sys
import os

from shutil import copy2

# Copy Python dll
python_dir = os.path.dirname(sys.argv[1])

for file in os.listdir(python_dir):

    if file.endswith("python39.dll"):
        output_dir = os.path.dirname(__file__) + "/../build/dll/"

        os.makedirs(output_dir)

        copy2(os.path.join(python_dir, file), output_dir)
