# Working with schemas

# Проверка verilog локально

1. Сборка: `iverilog -g2012 -o stack_tb.out stack_behaviour_tb.sv`
2. Симуляция: `vvp stack_tb.out +TIMES=5 +OUTCSV=st_stack_5.csv`
3. Запуск проверки:

   3.1. lite: `python ".github/workflows/verilog_checker.py" st_stack_5.csv ".github/workflows/ref_stack_lite_5.csv"`
   
   3.2. normal: `python ".github/workflows/verilog_checker.py" st_stack_5.csv ".github/workflows/ref_stack_normal_5.csv"`


