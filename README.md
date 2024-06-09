# Домашнее задание №6
## Сборка многомодульных программ. Вычисление корней уравнений и определенных интегралов

### Постановка задачи
С заданной точностью ε вычислить площадь плоской фигуры, ограниченной тремя кривыми.

### Вариант задания
1. **Уравнения кривых:** вариант 6.
   1. <var>f</var><sub>1</sub> = 0.6<var>x</var> + 3,
   2. <var>f</var><sub>2</sub> = (<var>x</var> − 2)^3 - 1,
   3. <var>f</var><sub>3</sub> = 3 / <var>x</var>,  (<var>x</var> > 0).

3. **Метод приближенного решения уравнений:** метод деления отрезка пополам.
4. **Квадратурные формулы:** формула Симпсона (парабол). 

---
nasm -felf32 functions_value.asm

gcc -m32 -c integral.c

gcc -m32 integral.o functions_value.o -o a.out


