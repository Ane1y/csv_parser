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

Для запуска на Unix выполните скрипт
   ```
   ./build.sh
   ```

Для Windows:
   ```
   .\build.bat

   ```

Запустите CSVReader с указанным путем до CSV файла:
   ```
   $ ./build/csvreader file.csv
   ```
Замените `file.csv` на путь к вашему CSV файлу.

В консоль будет выведен результат работы программы


### Unit-тесты

Были проверены следующие случаи:

- Наличие циклической зависимости
- Неполные строки
- Некорретные значения
- Дублирующиеся названия столбцов/строк
- Корректность считывания при наличии пробелов в начале и в конце ячейки 
- Несуществующие адреса ячеек
