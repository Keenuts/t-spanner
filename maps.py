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

def get_graph(filename, out):
    f = open(filename, "r")
    o = open(out, "w+")
    count = 0
    for line in f:
        if count % 100 == 0:
            time.sleep(5)
        loc = get_location(line)
        o.write(line[:-1] + " " + str(loc['lat']) + " " + str(loc['lng']) + "\n")
        count += 1

if __name__ == "__main__":
    if (sys.argv[1] and sys.argv[2]):
        get_graph(sys.argv[1], sys.argv[2])
