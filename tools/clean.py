import subprocess

# Mac Clean
subprocess.call("make config=debug clean", shell=True)
subprocess.call("make config=release clean", shell=True)