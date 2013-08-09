#include <fstream>
#include <cstdlib>
#include <string>
#include <stack>

//plain old hello world
const char* sample = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

int main(int argc, char** argv) {
  //some constants
  const int RAM_SIZE = 30000;
  const std::string chars("><+-.,[]");

  char ram[RAM_SIZE] = {0};
  int p = 0; //ram pointer
  std::stack<int> brackets; //stack for managing loops

  std::string program; //stripped program
  int pos = 0; //current position in the program

  std::ifstream file;

  if (argc > 1) { //if the file name argument is supplied
    file.open(argv[1]); //open the file

    if (!file.is_open()) {
      printf("Couldn't open the file!");
      exit(1);
    }

    bool brackets_no_match = false;

    //read the file
    while(file.good()) {
      char c;
      file >> c;

      if (file.eof()) //if it's the end, get out of the loop
        break;

      if (chars.find(c) != std::string::npos) { //if it's a valid char
        program += c; //add it to the program

        //and do some bracket balance checking
        if (c == '[')
          brackets.push(0);

        if (c == ']') {
          if (brackets.empty()) {
            //there are no [ connected with this ] so program is not valid
            brackets_no_match = true;
            break;
          }

          brackets.pop();
        }
      }
    }

    file.close();

    //if there was an unbalance
    if (brackets_no_match || !brackets.empty()) {
      printf("Brackets do not match!");
      exit(1);
    }
  } else {
    //run the hello world program
    program = sample;
  }

  bool running = true;

  while (running) {
    switch(program[pos]) { //read a command
      case '>':
        (p + 1) > RAM_SIZE - 1 ? 0 : p++; //pointer wraps around the ram
        break;

      case '<':
        (p - 1) < 0 ? RAM_SIZE - 1 : p--;
        break;

      case '+':
        ram[p]++;
        break;

      case '-':
        ram[p]--;
        break;

      case '.':
        printf("%c", ram[p]);
        break;

      case ',':
        ram[p] = getchar();
        break;

      case '[':
        //push the position of the [ command onto the stack
        brackets.push(pos);

        if (!ram[p]) {
          //while (0)
          //we have to skip this loop!
          int s = brackets.size();

          //skip to the first command after the matching ]
          while (brackets.size() >= s) {
            pos++;

            //skip the inside-loops also
            if (program[pos] == '[')
              brackets.push(0);

            if (program[pos] == ']')
              brackets.pop();
          }
        }

        break;

      case ']':
        if (ram[p])
          //if the condition is true, get back to the matching [
          pos = brackets.top();
        else
          brackets.pop();

        break;
    }

    //increment current command
    pos++;

    //check if we're at the end of program
    if (pos >= program.length())
      running = false;
  }

  return 0;
}
