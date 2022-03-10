import readline
from urllib.parse import urlencode
from urllib.request import Request, urlopen
import argparse
import json
import configparser
import random
from subprocess import Popen, PIPE

config = configparser.ConfigParser()

modes = ['digital_rain', 'sparkle', 'fire', 'static', 'kitt', 'nyancat']

def main():
    config.read("tinman-automator.conf")
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--inmtg", action="store_true", help="Set ticker to in meeting text scroll")
    group.add_argument("--checkmtg", action="store_true", help="Check if in mtg and set ticker to meeting text scroll")
    group.add_argument("--clear", action="store_true", help="Clear text on ticker")
    group.add_argument("--camevents", action="store_true", help="Attach to log and stream for camera events")
    group.add_argument("--ticker", action="store", help="Set ticker with message", metavar='<text>')
    group.add_argument("--mode", choices=modes, help="Set ticker mode. Allowed values are " + ", ".join(modes), metavar="<mode>")
    args = parser.parse_args()
    if args.inmtg:
        in_mtg()
    elif args.checkmtg:
        check_mtg()
    elif args.clear:
        set_mode("ticker", msg="")
    elif args.camevents:
        cam_events()
    elif args.mode != None:
        set_mode(args.mode)
    elif args.ticker != None:
        set_mode("ticker", args.ticker)
    else:
        parser.print_usage()
        exit(1)

def check_mtg():
    mode = get_mode()
    if (mode.lower() != "ticker"):
        mtg_status_url = config["DEFAULT"]["mtg_status_url"]
        with urlopen(mtg_status_url) as resp:
            if resp.status == 200:
                json_resp = json.loads(resp.read().decode())
                if json_resp["data"]["inMtg"]:
                    in_mtg()
                else:
                    random_mode()

def random_mode():
    set_mode(random.choice(modes))

def in_mtg():
    set_mode("ticker", msg="in meeting", color="0xffff00")

def get_mode():
    ticker_url = config["DEFAULT"]["ticker_url"]
    url = ticker_url + "mode"
    with urlopen(url) as resp:
        if resp.status == 200:
            return resp.read().decode().strip()
        else:
            return ""

def set_mode(mode, msg=None, color=None, speed=None):
    ticker_url = config["DEFAULT"]["ticker_url"]
    url = ticker_url + "mode"
    fields = {"mode": mode}
    if (msg != None):
        fields["msg"] = msg
    if (color != None):
        fields["clr"] = color
    if (speed != None):
        fields["speed"] = speed
    req = Request(url, urlencode(fields).encode())
    with urlopen(req) as resp:
        return resp.status == 200

def cam_events():
    log_cmd = ["log", "stream", "--predicate", 'eventMessage contains "Post event kCameraStream"']
    with Popen(log_cmd, stdout=PIPE) as cam_events:
        while cam_events.poll() == None:
            cam_event = str(cam_events.stdout.readline())
            if ("kCameraStreamStart" in cam_event):
                in_mtg()
            elif ("kCameraStreamStop" in cam_event):
                random_mode()

if __name__ == "__main__":
    main()
