# **Friday**

Language capability:
- [x] Variables
- [X] Functions
- [X] Arithmetic expressions
- [ ] Loops
- [ ] Boolean expressions
- [ ] Conditional statements


**Syntax:**

There two ways to declare variable:
  - Keyword 'var':
```
var name = "John"
```
  - Character '_' as first symbol of variable name:
```
_age = 18
```
Two ways to declare function:
  - Keyword 'fun':
```
fun do_smth(arg1){
  println(arg1)
}

do_smth(name)
```
  - Character ':' as first symbol of function name:
```
:do_smth2(arg2){
  println(arg2)
}
do_smth2(age)
```
Built in functions:
  - *print()* - print text
`print(name)`
  - *println()* - print line of text
`println("Hello friday!")`


Code example:
```
var name1 = "John'
var age1 = 18
_name2 = "Jane"
_age2 = 21

fun info(name_1, age_1, name_2, age_2){
  println("First person: ", name_1, age_1)
  println("Second person: ", name_2, age_2)
}

info(name1, age1, name2, age2)

:second_info(name_1, name_2){
  println(name_1, name_2)
}
 
second_info(name1, name2)

_value = 5 + 3 * (10 + 5)

println(value)
```
