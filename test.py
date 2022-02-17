from arl_dilithium import generate_keypair, sign, verify, generate_keypair_random
import binascii
import hashlib
import time

pub,priv=generate_keypair_random()
print("==== PUBKEY ====")
print(pub.hex())

print("==== PRIVKEY ====")
print(priv.hex())
