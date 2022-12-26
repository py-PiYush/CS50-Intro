const hex = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "A", "B", "C", "D", "E", "F"];

const btn = document.getElementById("btn");
const container = document.querySelector(".bg-random");

btn.addEventListener("click", function () {
  //generate a random hex code
  let code = "#";
  for (let i = 0; i < 6; i++) {
    const randomNumber = getRandomNumber();
    code += hex[randomNumber];
  }

  container.style.backgroundColor = code;
});

function getRandomNumber() {
  return Math.floor(Math.random() * hex.length);
}
