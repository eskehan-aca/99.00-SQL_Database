
# Work Report
## Name: <ins> Elizabeth Skehan </ins>
## Features:
- Not Implemented:
  - what features have been implemented
<br><br>
- Implemented:
  - what has been implemented
<br><br>
- Partly implemented:
  - what features have not been implemented
<br><br>
- Bugs
  - Known bugs
<br><br>
- Reflections:
  - Any thoughts you may have and would like to share.
<br><br>
# output
<pre>
----------running basic_test.cpp---------


[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from SQL_BASIC
[ RUN      ] SQL_BASIC.SQLBasic
>make table employee fields  last,       first,         dep,      salary, year
basic_test: table created.

>make table employee fields  last,       first,         dep,      salary, year
>insert into employee values Blow,       Joe,           CS,       100000, 2018
>insert into employee values Blow,       JoAnn,         Physics,  200000, 2016
>insert into employee values Johnson,    Jack,          HR,       150000, 2014
>insert into employee values Johnson,    "Jimmy",     Chemistry,140000, 2018
>make table student fields  fname,          lname,    major,    age
>insert into student values Flo,            Yao,        Art,    20
>insert into student values Bo,                      Yang,      CS,             28
>insert into student values "Sammuel L.", Jackson,      CS,             40        
>insert into student values "Billy",         Jackson,   Math,   27
>insert into student values "Mary Ann",   Davis,        Math,   30



>select * from employee
table name: select_recnos3, records: 4
record                          last                         first                           dep                        salary                          year
     0                          Blow                           Joe                            CS                        100000                          2018
     1                          Blow                         JoAnn                       Physics                        200000                          2016
     2                       Johnson                          Jack                            HR                        150000                          2014
     3                       Johnson                         Jimmy                     Chemistry                        140000                          2018


basic_test: records selected: (4)       [       0       1       2       3       ]


>select last, first, age from employee
table name: select_recnos4, records: 4
record                          last                         first                           age
     0                          Blow                           Joe                          Blow
     1                          Blow                         JoAnn                          Blow
     2                       Johnson                          Jack                       Johnson
     3                       Johnson                         Jimmy                       Johnson


basic_test: records selected: (4)       [       0       1       2       3       ]


>select last from employee
table name: select_recnos5, records: 4
record                          last
     0                          Blow
     1                          Blow
     2                       Johnson
     3                       Johnson


basic_test: records selected: (4)       [       0       1       2       3       ]


>select * from employee where last = Johnson
table name: select_recnos6, records: 2
record                          last                         first                           dep                        salary                          year
     0                       Johnson                          Jack                            HR                        150000                          2014
     1                       Johnson                         Jimmy                     Chemistry                        140000                          2018


basic_test: records selected: (2)       [       2       3       ]


>select * from employee where last=Blow and major="JoAnn"
table name: select_recnos7, records: 0
record                          last                         first                           dep                        salary                          year


basic_test: records selected: (0)       [       ]


>select * from student
table name: select_recnos8, records: 5
record                         fname                         lname                         major                           age
     0                           Flo                           Yao                           Art                            20
     1                            Bo                          Yang                            CS                            28
     2                    Sammuel L.                       Jackson                            CS                            40
     3                         Billy                       Jackson                          Math                            27
     4                      Mary Ann                         Davis                          Math                            30


basic_test: records selected: (5)       [       0       1       2       3       4       ]


>select * from student where (major=CS or major=Art)
table name: select_recnos9, records: 3
record                         fname                         lname                         major                           age
     0                           Flo                           Yao                           Art                            20
     1                            Bo                          Yang                            CS                            28
     2                    Sammuel L.                       Jackson                            CS                            40


basic_test: records selected: (3)       [       0       1       2       ]


>select * from student where lname>J
table name: select_recnos10, records: 4
record                         fname                         lname                         major                           age
     0                           Flo                           Yao                           Art                            20
     1                            Bo                          Yang                            CS                            28
     2                    Sammuel L.                       Jackson                            CS                            40
     3                         Billy                       Jackson                          Math                            27


basic_test: records selected: (4)       [       0       1       2       3       ]


>select * from student where lname>J and (major=CS or major=Art)
table name: select_recnos11, records: 3
record                         fname                         lname                         major                           age
     0                           Flo                           Yao                           Art                            20
     1                            Bo                          Yang                            CS                            28
     2                    Sammuel L.                       Jackson                            CS                            40


basic_test: records selected: (3)       [       0       1       2       ]

----- END TEST --------
[       OK ] SQL_BASIC.SQLBasic (387 ms)
[----------] 1 test from SQL_BASIC (387 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (388 ms total)
[  PASSED  ] 1 test.
</pre>