#!/usr/bin/python3

import os
from re import search
import sys, subprocess, pathlib, pprint, platform

class FatalError (RuntimeError):
    def __init__(self, message):
        super(FatalError, self).__init__(message)

def rip(message, exit_code = -1):
    print("\n\n[FATAL] {0}\n\n".format(message))
    sys.exit(exit_code)

def loge(message):
    print("[ERROR] {0}".format(message))

def logw(message):
    print("[WARN]  {0}".format(message))

def logi(message):
    print("[INFO]  {0}".format(message))

def kill_android_proc(procname, adb = ['adb']):
    try:
        proc = subprocess.check_output(adb + ["shell", f"ps -A|grep {procname}"]).decode("utf-8").splitlines()
        for p in proc:
            items = p.split()
            if len(items) > 1:
                pid = items[1]
                logi(f"kill remote process {pid}")
                subprocess.run(adb + ["shell", f"kill -9 {pid}"])
    except subprocess.CalledProcessError:
        # Ignore subprocess.CalledProcessError exception. It happens when the process in question is not running.
        pass

def run_cmd(args, capture_output = False, check=True, verbose=False):
    if verbose: print(" ".join(args))
    r = subprocess.run(args, check=check, capture_output=capture_output)
    if capture_output:
        return r.stdout.decode("utf-8").strip()
    else:
        return r.returncode

class AndroidDebugBridge:
    cmd = None
    verbose = False

    def __init__(self, device_serial_number = None):
        self.cmd = ["adb"]
        if "usb" == device_serial_number:
            self.cmd += ["-d"]
        elif not device_serial_number is None:
            self.cmd += ["-s", device_serial_number]
        # verify the selected device is connected
        logi("Waiting for device to be connected...")
        subprocess.check_output(self.cmd + ["wait-for-device"])
        logi("Device connected.")

    def run(self, args, capture_output = False, check = True, verbose = False):
        if isinstance(args, str): args = args.split()
        if check is None: check = self.check
        if verbose is None: verbose = self.verbose
        return run_cmd(self.cmd + args, capture_output, check, verbose or self.verbose)

    def capture_output(self, args, check = True, verbose = False):
        return self.run(args, capture_output = True, check = check, verbose = verbose)

    def shell(self, command, capture_output = False, check = True, verbose = False):
        return self.run(["shell", command], capture_output, check, verbose)

def compare_file_timestamp(path, latest, chosen):
    if not path.exists(): return latest, chosen
    timestamp = path.stat().st_mtime
    # print(f"{path} : {timestamp}, {timestamp} < {latest} = {timestamp < latest}")
    if timestamp < latest: return latest, chosen
    return timestamp, path

def get_root_folder():
    return pathlib.Path(__file__).resolve().parent.parent.parent.absolute()

def search_for_the_latest_binary_ex(path_template):
    variants = [".d", ".p", ".r"]
    platforms = ["mswin", "linux", "darwin"]
    compilers = [".clang", ".gcc", ".xcode"]
    # Loop through all candidates
    latest = 0
    chosen = None
    sdk_root_dir = get_root_folder()
    searched = []
    for var in variants:
        for pla in platforms:
            if "mswin" == pla and "Windows" != platform.system(): continue
            for com in compilers:
                c = path_template.format(variant=f"{pla}{com}{var}")
                p = pathlib.Path(c)
                if not p.is_absolute(): p = sdk_root_dir / p
                if "mswin" == pla:
                    v = "Debug" if ".d" == var else "Release" if ".r" == var else "RelWithDebInfo"
                    p = pathlib.Path(c).with_suffix(".exe")
                    p = p.parent / v / p.name
                searched.append(p)
                latest, chosen = compare_file_timestamp(p, latest, chosen)
                latest, chosen = compare_file_timestamp(p.with_suffix(".exe"), latest, chosen)
    return chosen, searched

def search_for_the_latest_binary(path_template):
    chosen, searched = search_for_the_latest_binary_ex(path_template)
    if chosen is None:
        pp = pprint.PrettyPrinter(indent=4)
        print(f"[ERROR] binary _NOT_ found: {path_template}. The following locations are searched:\n{pp.pformat(searched)}")
    return chosen

def run_the_latest_binary(path_template, argv, check = True):
    # search for the latest binary
    chosen, searched = search_for_the_latest_binary_ex(path_template)
    if chosen is None:
        pp = pprint.PrettyPrinter(indent=4)
        print(f"[ERROR] binary _NOT_ found: {path_template}. The following locations are searched:\n{pp.pformat(searched)}")
        sys.exit(1)
    # Invoke the binary
    cmdline = [str(chosen)] + argv
    print(' '.join(cmdline))
    return subprocess.run(cmdline, check=check)
