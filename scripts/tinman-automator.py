import readline
from urllib.parse import urlencode
from urllib.request import Request, urlopen
import argparse
import json
import configparser
import random
from subprocess import Popen, PIPE

config = configparser.ConfigParser()

def main():
    config.read("tinman-automator.conf")
    parser = argparse.ArgumentParser()
    parser.add_argument("--inmtg", action="store_true", help="Set ticker to in meeting text scroll")
    parser.add_argument("--rain", action="store_true", help="Set ticker to matrix digital rain")
    parser.add_argument("--sparkle", action="store_true", help="Set ticker to sparkle")
    parser.add_argument("--fire", action="store_true", help="Set ticker to doom fire")
    parser.add_argument("--static", action="store_true", help="Set ticker to tv static")
    parser.add_argument("--kitt", action="store_true", help="Set ticker kitt car")
    parser.add_argument("--nyancat", action="store_true", help="Set ticker nyan cat")
    parser.add_argument("--checkmtg", action="store_true", help="Check if in mtg and set ticker to meeting text scroll")
    parser.add_argument("--clear", action="store_true", help="Clear text on ticker")
    parser.add_argument("--camevents", action="store_true", help="Attach to log and stream for camera events")
    args = parser.parse_args()
    if args.inmtg:
        in_mtg()
    elif args.rain:
        set_mode("digital_rain")
    elif args.sparkle:
        set_mode("sparkle")
    elif args.fire:
        set_mode("fire")
    elif args.static:
        set_mode("static")
    elif args.kitt:
        set_mode("kitt")
    elif args.nyancat:
        set_mode("nyancat")
    elif args.checkmtg:
        check_mtg()
    elif args.clear:
        set_mode("ticker", msg="")
    elif args.camevents:
        cam_events()
    else:
        parser.print_usage()
        exit(1)

def check_mtg():
    mode = get_mode()
    if (mode.lower() != "ticker"):
        mtg_status_url = config["DEFAULT"]["mtg_status_url"]
        req = Request(mtg_status_url)
        resp = urlopen(req)
        if resp.status == 200:
            json_resp = json.loads(resp.read().decode())
            if json_resp["data"]["inMtg"]:
                in_mtg()
            else:
                random_mode()

def random_mode():
    set_mode(random.choice(["digital_rain", "static", "nyancat", "sparkle", "fire"]))

def in_mtg():
    set_mode("TICKER", msg="in meeting", color="0xffff00")

def get_mode():
    ticker_url = config["DEFAULT"]["ticker_url"]
    url = ticker_url + "mode"
    req = Request(url)
    resp = urlopen(req)
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
    resp = urlopen(req)
    return resp.status == 200

def cam_events():
    print("Streaming")
    with Popen(["log", "stream", "--predicate", 'eventMessage contains "Post event kCameraStream"'], stdout=PIPE) as camlogs:
        while camlogs.poll() == None:
            camline = str(camlogs.stdout.readline())
            if ("kCameraStreamStart" in camline):
                in_mtg()
            elif ("kCameraStreamStop" in camline):
                random_mode()

if __name__ == "__main__":
    main()
