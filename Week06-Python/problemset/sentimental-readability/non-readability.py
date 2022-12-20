text = input("Enter text: ").strip()
index = round(
    0.0588 * (sum(ch.isalnum() for ch in text) * 100) / len(text.split(" "))
    - 0.296 * (sum(1 for ch in text if ch in ".!?") * 100) / len(text.split(" "))
    - 15.8
)
print("Grade 16+") if index >= 16 else print("Before Grade 1") if index < 1 else print(
    f"Grade {index}"
)
