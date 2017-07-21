#!/bin/python3

import json
import requests
import sys
import time

base_url = 'https://maps.googleapis.com/maps/api/geocode/json?'
key = 'AIzaSyB8EgPxCzRyds80jKyuLrq9sd_ZWZMIFDo'

def get_location(city):
    url = base_url + 'address=' + city + '&key=' + key
    res = requests.get(url)
    obj = res.json()
    return obj['results'][0]['geometry']['location']

def get_next_str(string):
    if len(string) == 0:
        return 'A'
    c = string[-1]
    if ord(c) >= ord('Z'):
        string = get_next_str(string[:-1]) + 'A'
    else:
        string = string[:-1] + chr(ord(c) + 1)
    return string

def get_graph(filename, out):
    f = open(filename, "r")
    o = open(out, "w+")
    count = 0
    uniq = "A"
    for line in f:
        if count % 100 == 0:
            time.sleep(5)
        loc = get_location(line)
        o.write(uniq + " " + str(loc['lng']) + " " + str(loc['lat']) + "\n")
        count += 1
        uniq = get_next_str(uniq)

if __name__ == "__main__":
    if (sys.argv[1] and sys.argv[2]):
        get_graph(sys.argv[1], sys.argv[2])
