from cs50 import get_string


def main():
    # Get user input
    text = get_string("Enter text: ")

    # Count letters, words, sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculate Coleman-liau
    L = (letters * 100) / words
    S = (sentences * 100) / words
    index = coleman_liau(L, S)

    # Print result
    print_grade(index)
    # print(letters, words, sentences)


def count_letters(text):
    return sum(ch.isalnum() for ch in text)


def count_words(text):
    return len(text.split(" "))


def count_sentences(text):
    return sum(1 for ch in text if ch in ".!?")


def coleman_liau(l, s):
    return round(0.0588 * l - 0.296 * s - 15.8)


def print_grade(index):
    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()
