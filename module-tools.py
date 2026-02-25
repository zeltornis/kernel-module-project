import argparse
import subprocess
import re

def run_cmd(cmd):
    print(f"Running: {cmd}")
    subprocess.run(cmd, shell=True, check=True)

def load_module(module_path):
    run_cmd(f"sudo insmod {module_path}")

def unload_module(module_name):
    run_cmd(f"sudo rmmod {module_name}")

def show_logs():
    run_cmd("dmesg | tail -n 20")

def monitor_logs(keyword):
    MODULE_PATTERN = re.compile(keyword)
    print("Monitoring kernel logs for keyword: '{keyword}'")
    process = subprocess.Popen(["dmesg", "--follow"],
                               stdout=subprocess.PIPE, text=True)
    for line in process.stdout:
        if MODULE_PATTERN.search(line):
            print(f"[MODULE EVENT] {line.strip()}")

parser = argparse.ArgumentParser(description="Kernel Module Toolkit")
parser.add_argument("command",
                    choices=["load", "unload", "logs", "monitor"])
parser.add_argument("--module",
                    help="Path to module (.ko) or module name")

args = parser.parse_args()

if args.command == "load":
    if not args.module:
        print("Error: --module is required for load")
    else:
        load_module(args.module)
elif args.command == "unload":
    if not args.module:
        print("Error: --module is required for unload")
    else:
        unload_module(args.module)
elif args.command == "logs":
    show_logs()
elif args.command == "monitor":
    monitor_logs()
