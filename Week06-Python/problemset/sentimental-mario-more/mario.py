# TODO
from cs50 import get_int


def main():
    # get height from user
    height = get_height()

    # Print tower
    print_tower(height)


def get_height():
    while True:
        n = get_int("height: ")
        if 1 <= n <= 8:
            return n


def print_tower(height):
    for row in range(1, height + 1):
        print(f'{" " * (height - row)}{row * "#"}  {row * "#"}')


if __name__ == "__main__":
    main()
