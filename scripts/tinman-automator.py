from urllib.parse import urlencode
from urllib.request import Request, urlopen
import argparse
import json
import configparser

config = configparser.ConfigParser()


def main():
    config.read('tinman-automator.conf')
    parser = argparse.ArgumentParser()
    parser.add_argument("--inmtg", action="store_true", help="Set ticker to in meeting text scroll")
    parser.add_argument("--rain", action="store_true", help="Set ticker to matrix digital rain")
    parser.add_argument("--sparkle", action="store_true", help="Set ticker to sparkle")
    parser.add_argument("--fire", action="store_true", help="Set ticker to doom fire")
    parser.add_argument("--checkmtg", action="store_true", help="Check if in mtg and set ticker to meeting text scroll")
    parser.add_argument("--clear", action="store_true", help="Clear text on ticker")
    args = parser.parse_args()
    if args.inmtg:
        in_mtg()
    elif args.rain:
        digital_rain()
    elif args.sparkle:
        sparkle()
    elif args.fire:
        fire()
    elif args.checkmtg:
        check_mtg()
    elif args.clear:
        set_mode("TEXT_SCROLL")
        set_msg("")
    else:
        parser.print_usage()
        exit(1)


def check_mtg():
    mtg_status_url = config['DEFAULT']['mtg_status_url']
    req = Request(mtg_status_url)
    resp = urlopen(req)
    if resp.status == 200:
        json_resp = json.loads(resp.read().decode())
        if json_resp['data']['inMtg']:
            in_mtg()
        else:
            digital_rain()


def in_mtg():
    set_mode("TEXT_SCROLL")
    set_msg("in meeting")
    set_speed(25)
    set_color("0xffff00")


def digital_rain():
    set_mode("DIGITAL_RAIN")
    set_speed(8)


def sparkle():
    set_mode("SPARKLE")
    set_speed(25)


def fire():
    set_mode("FIRE")
    set_speed(15)


def set_mode(mode):
    ticker_url = config['DEFAULT']['ticker_url']
    url = ticker_url + 'mode'
    fields = {'mode': mode}
    req = Request(url, urlencode(fields).encode())
    resp = urlopen(req)
    return resp.status == 200


def set_msg(msg):
    ticker_url = config['DEFAULT']['ticker_url']
    url = ticker_url + 'msg'
    fields = {'msg': msg}
    req = Request(url, urlencode(fields).encode())
    resp = urlopen(req)
    return resp.status == 200


def set_speed(speed):
    ticker_url = config['DEFAULT']['ticker_url']
    url = ticker_url + 'speed'
    fields = {'speed': speed}
    req = Request(url, urlencode(fields).encode())
    resp = urlopen(req)
    return resp.status == 200


def set_color(color):
    ticker_url = config['DEFAULT']['ticker_url']
    url = ticker_url + 'clr'
    fields = {'clr': color}
    req = Request(url, urlencode(fields).encode())
    resp = urlopen(req)
    return resp.status == 200


if __name__ == '__main__':
    main()
