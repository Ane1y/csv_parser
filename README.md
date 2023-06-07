# CSV Reader
Консольная утилита для чтения и вывода CSV файлов

Принимает на вход csv-файл вида: 
```
,A,B,Cell
1,1,0,1
2,2,=A1+Cell30,0
30,0,=B1+A1,5
```
Вывод для данного примера:
```
,A,B,Cell
1,1,0,1
2,2,6,0,
30,0,1,5
```
### Запуск
Чтобы использовать утилиту, выполните следующие действия:

1. Соберите исполняемый файл CSVReader:
   ```
   $ cmake --build build --target install
   ```

2. Запустите инструмент CSVReader с указанным CSV файлом:
   ```
   $ ./build/csvreader file.csv
   ```
   Замените `file.csv` на путь к вашему CSV файлу.

3. Утилита прочитает и подсчитает ячейки, и полученные данные будут выведены в консоль

### Unit-тесты

Были проверены следующие случаи:

- Наличие циклической зависимости
- Неполные строки
- Некорретные значения
- Дублирующиеся названия столбцов/строк
- Корректность считывания при наличии пробелов в начале и в конце ячейки 
- Несуществующие адреса ячеек