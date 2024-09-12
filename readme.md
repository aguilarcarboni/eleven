# Eleven

## CS1412: Programming Principles - Final Project

### Description
 
Eleven is an inrerpreted programming language.

Eleven combines the ease of use of Python with the functionality of C, 
since it can take advantage of pointers (strings).

### Installation and usage

```
git clone https://github.com/aguilarcarboni/eleven.git
```

Create a file named `program.el` and write your code there.

To run the program, type the following command in the terminal:
```
gcc main.c -o main
./main
```

### Documentation:

All commands must start in new line.

**Comments:**
```
$$$ comment
```

**Variable declarations:**
Supports integer and string types
```
int x = 5
string w = "stringy"
```

Strings can be indexed:
```
string s = copy(w[0])
```

Strings can be copied or concatenated:
```
concat(s, w)
```

Conditionals:
```
if condition: 
    statement
_
else if condition:
    statement
_
else:
    statement 
_
```

**Math functions:** (return a truncated int and print the float)
```
sin(angle)
cos(angle)
tan(angle)
```

**Write to console:**
```
log(expression)
```

**Globals:**
```
PI = 3 (come on engineers!)
```

### Examples

```
int x = 5
string w = "stringy"
string s = copy(w[0])
concat(s, w)
log(s)
```

### Results
- Scored 88% in the project.


### created by [@aguilarcarboni](https://github.com/aguilarcarboni/)
