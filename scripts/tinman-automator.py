import readline
from urllib.parse import urlencode
from urllib.request import Request, urlopen
import argparse
import json
import configparser
import random
from subprocess import Popen, PIPE
from datetime import datetime
import sys

class TinmanApi:
    config_file_name = "tinman-automator.conf"
    modes = ['digital_rain', 'sparkle', 'fire', 'static', 'kitt', 'nyancat']
    check_mtg_days = [day for day in range(1,6)]
    check_mtg_hours = [hour for hour in range(7, 16)]

    def __init__(self, args):
        self.config = configparser.ConfigParser()
        self.getConfig()
        self.ticker_url = args.url if (args.url) else self.config["urls"]["ticker_url"]

    def getConfig(self):
        self.config.read(self.config_file_name)
        return self.config

    def writeConfig(self):
        with open(self.config_file_name, "w") as file:
            self.config.write(file)

    def set_sleep(self, sleep):
        url = self.ticker_url + "sleep"
        fields = {"sleep": "true" if (sleep) else "false"}
        req = Request(url, urlencode(fields).encode())
        with urlopen(req) as resp:
            return resp.status == 200

    def working_hour(self):
        now = datetime.now()
        return (now.isoweekday() in self.check_mtg_days) and (now.hour in self.check_mtg_hours)

    def check_mtg(self):
        if (self.working_hour()):
            mode = self.get_mode()
            can_write = not (mode.lower() == "ticker" and self.config["state"]["last_write"] == self.cam_events.__name__)
            if can_write:
                mtg_status_url = self.config["urls"]["mtg_status_url"]
                with urlopen(mtg_status_url) as resp:
                    if resp.status == 200:
                        json_resp = json.loads(resp.read().decode())
                        if json_resp["data"]["inMtg"]:
                            self.in_mtg()
                        else:
                            self.random_mode()
                self.config["state"]["last_write"] = sys._getframe().f_code.co_name
                self.writeConfig()

    def random_mode(self):
        rmodes = [mode for mode in self.modes if mode != 'static']
        self.set_mode(random.choice(rmodes))

    def in_mtg(self):
        self.set_mode("ticker", msg="in meeting", color="0xffff00")

    def get_mode(self):
        url = self.ticker_url + "mode"
        with urlopen(url) as resp:
            if resp.status == 200:
                return resp.read().decode().strip()
            else:
                return ""

    def set_mode(self, mode, msg=None, color=None, speed=None):
        url = self.ticker_url + "mode"
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

    def cam_events(self):
        log_cmd = ["log", "stream", "--style", "ndjson", "--predicate", 'subsystem contains "com.apple.UVCExtension" and composedMessage contains "Post PowerLog"']
        with Popen(log_cmd, stdout=PIPE) as cam_events:
            while cam_events.poll() == None:
                cam_event = str(cam_events.stdout.readline())
                self.getConfig()
                if (self.working_hour()):
                    if ('On' in cam_event):
                        print("setting ticker to in meeting")
                        self.in_mtg()
                    elif ('Off' in cam_event):
                        print("setting ticker to random mode")
                        self.random_mode()
                self.config["state"]["last_write"] = sys._getframe().f_code.co_name
                self.writeConfig()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--url", action="store", help="Url to use for connecting to tinman")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--inmtg", action="store_true", help="Set ticker to in meeting text scroll")
    group.add_argument("--checkmtg", action="store_true", help="Check if in mtg and set ticker to meeting text scroll")
    group.add_argument("--clear", action="store_true", help="Clear text on ticker")
    group.add_argument("--camevents", action="store_true", help="Attach to log and stream for camera events")
    group.add_argument("--ticker", action="store", help="Set ticker with message", metavar='<text>')
    group.add_argument("--mode", choices=TinmanApi.modes, help="Set ticker mode. Allowed values are " + ", ".join(TinmanApi.modes), metavar="<mode>")
    group.add_argument("--sleep", action=argparse.BooleanOptionalAction, help="Set sleep mode (disables display)")
    args = parser.parse_args()
    api = TinmanApi(args)
    if args.inmtg:
        api.in_mtg()
    elif args.checkmtg:
        api.check_mtg()
    elif args.clear:
        api.set_mode("ticker", msg="")
    elif args.camevents:
        api.cam_events()
    elif args.mode != None:
        api.set_mode(args.mode)
    elif args.ticker != None:
        api.set_mode("ticker", args.ticker)
    elif args.sleep != None:
        api.set_sleep(args.sleep)
    else:
        parser.print_usage()
        exit(1)

if __name__ == "__main__":
    main()
