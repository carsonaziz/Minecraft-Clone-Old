# cli.py version
# cli.py gen
# cli.py build
# cli.py run

import sys
import subprocess

for i in range(1, len(sys.argv)):
    cmd = sys.argv[i]

    print("\n==============================")
    print("Executing: ", cmd)
    subprocess.call("python3 tools/{}.py".format(cmd), shell=True)