from cs50 import get_string


def main():
    card = get_string("Enter card number: ")
    if valid(card):
        print(get_result(card))
    else:
        print("INVALID")


def valid(card):
    total = 0
    for i, ch in enumerate(card[::-1]):
        if i % 2:
            num = 2 * int(ch)
            total = total + ((1 + (num % 10)) if num >= 10 else num)
        else:
            total += int(ch)
    return total % 10 == 0


def get_result(card):
    num_digits = len(card)
    first_two = int(card[:2])
    first_digit = int(card[0])

    if num_digits == 15 and (first_two == 34 or first_two == 37):
        return "AMEX"
    elif (num_digits == 13 or num_digits == 16) and first_digit == 4:
        return "VISA"
    elif num_digits == 16 and 51 <= first_two <= 55:
        return "MASTERCARD"
    else:
        return "INVALID"


if __name__ == "__main__":
    main()
