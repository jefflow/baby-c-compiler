# Baby C Compiler
## Introduction
This compiler was built for a Compiler Construction class at CSU, Sacramento.

The compiler is made for a very simplified version of the C programming language called Baby C. 

Baby C contains the very basic features of a programming language: declarations, expressions, assignments, if-else statements, and while loops. A Baby C program consists of one function called `main`

**Note that this is a toy compiler, meaning it does not generate actual assembly and it doesn't target to a specific architecture. It is missing the optimizer and backend phases of a typical compiler.**

## How to build and run
Note that these instructions are written for macOS.

### Step 1: Install Homebrew
macOS by default does not have a package manager, so we need to install one. Run the following command in Terminal:
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
### Step 2: Install Bison
Run the following in Terminal:
```
brew install bison
```
Then run:
```
brew link bison --force
```

### Step 3: Move test cases out of the folder
Simply **select all** files in the `test-cases` folder and move them to the root of the `baby-c-compiler` folder.

### Step 4: Open terminal and `cd` to the `baby-c-compiler` folder
You don't need an explanation...

### Step 5: Run the `makefile`
Type:
```
make
```

### Step 6: Run the Baby C Compiler
In order to run the compiler, you need to feed it a test file with the extension: `.bc`. 
Run this command:
```
./bcc <insert test file name>.bc
```
Sample command:
```
./bcc test1.bc
```

## Expected inputs and outputs
It expects a `.bc` file, which contains the Baby C code. Once it processes the file, it will display the creation of the AST nodes in your terminal window. It will also generate a `.ILOC` file containing the ILOC code.

## Structure of the compiler
### Front End
Scanner Generator: Flex

Symbol Table: Hashtable

High-level Intermediate Representation: Abstract Syntax Trees

Low-level Intermediate Representation: ILOC 

Parser Generator: Bison

### Optimizer
Not implemented.

### Backend
Not implemented.







