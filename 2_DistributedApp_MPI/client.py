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
