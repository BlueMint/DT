from nrf24 import Nrf24
import time

nrf = Nrf24(cePin=2,csnPin=3,channel=10,payload=8)
nrf.config()
nrf.setRADDR("host1")
nrf.setTADDR("host2")

while True:
    if not nrf.isSending():
        nrf.send(map(ord,"Helloooo"))
    time.sleep(5)