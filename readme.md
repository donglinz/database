#C++ 实现关系型数据库   
基于 C++11从底层实现了DBMS的部分功能,底层存储结构位0x3f版本的dbf文件。   
#编译   
IDE:Visual Studio 2015及以上   
OS:win10   
Complier:MSVC++11.0 或更高,必需在64位环境下进行编译   
#查询语句   
###Create   
多字段,多数据类型。   
支持int, varchar, char, date, double数据类型   
示例:   
```
create table stu (
id int,
name varchar(20),
sex char,
weight double,
birtyday date
);
```
###Insert   
支持单记录,多记录insert   
示例:
```
insert into stu values (1, 'zhangsan', 'M', 90.0, '1996-10-10');
insert into stu values (2, 'lisi', 'M', 100.1, '1996-10-10');
insert into stu values (3, 'tom', 'M', 101.0, '1996-10-11');
```
###Select   

单条件,多条件,聚合函数,嵌套查询,对查询结果排序   
示例1多条件查询:
```
select id, name from stu where id>=2&&id<=5
```   
示例2嵌套查询:
```
select ID, Firstname from
(select ID, Firstname from
test where date=='2016-10-10') where id==1;
```   
示例3聚合函数:   
```
select max(weight), sex from stu group by sex having max(weight)>=100;
```   
示例4排序:
```
select ave(weight), birtyday from stu group by birtyday order by weight;
```
