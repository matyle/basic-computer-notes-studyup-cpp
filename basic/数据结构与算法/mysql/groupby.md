```sql
# Write your MySQL query statement below
select Email 
from Person
group by Email
having count(*)>1;

#count字段比count(*)慢
# select Email 
# from Person
# group by Email
# having count(Email)>1;
```

