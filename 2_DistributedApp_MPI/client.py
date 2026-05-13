# client.py

import xmlrpc.client


# Connect to server
proxy = xmlrpc.client.ServerProxy("http://localhost:1030/")

name3 = ""

while True:
    print("\n\t\t*** Menu ***")
    print("\n 1. Send Input String")
    print("\n 2. Display Reversed String")
    print("\n 0. Exit")

    ch = int(input("\nEnter your choice: "))

    if ch == 1:
        name1 = input("Enter First String: ")

        # Remote method call
        name3 = proxy.input(name1)

    elif ch == 2:
        print("\nReversed String =", name3)

    elif ch == 0:
        break

    else:
        print("Invalid choice")

# Steps to run:
# 1) python server.py
# 2) python client.py
#
# Input examples:
# 1) Choice 1 -> "hello", then Choice 2 -> output "olleh"
# 2) Choice 1 -> "distributed", then Choice 2 -> output "detubirtsid"
#
# Expected output:
# 1) Reversed String = olleh
# 2) Reversed String = detubirtsid
