Update 11/20/2019:
这个项目源自一次仓促的课程设计. 实现的思路质量并不怎么样. 仅供参考.

<h1>C++ 实现关系型数据库</h1> 
基于 C++11从底层实现了DBMS的部分功能,底层存储结构位0x3f版本的dbf文件。   
<h1>编译</h1>
IDE:Visual Studio 2015及以上   
OS:win10   
Complier:MSVC++11.0 或更高,必需在64位环境下进行编译   
<h1>查询语句</h1>   
<h3>Create</h3>   
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
<h3>Insert</h3>   
支持单记录,多记录insert   
示例:   

```
insert into stu values (1, 'zhangsan', 'M', 90.0, '1996-10-10');
insert into stu values (2, 'lisi', 'M', 100.1, '1996-10-10');
insert into stu values (3, 'tom', 'M', 101.0, '1996-10-11');
```
<h3>Select</h3>   

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
