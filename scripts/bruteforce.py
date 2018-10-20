#!/usr/bin/python3

def bruteforce():
    for a in range(1, 7):
        for b in range(1, 7):
            for c in range(1, 7):
                for d in range(1, 7):
                    for e in range(1, 7):
                        if a == 4 or a == b or a == b or a == c or a == d or a == e:
                            continue
                        if b == 4 or b == c or b == d or b == e:
                            continue
                        if c == 4 or c == d or c == e:
                            continue
                        if d == 4 or d == e:
                            continue
                        if e == 4:
                            continue
                        print("4 {} {} {} {} {}".format(a, b, c, d, e))

if __name__ == "__main__":
    bruteforce()
