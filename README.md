# qflock

Glue for flock in q/kdb+

```
{x set`:./qflock 2:x,y}'[`lock`unlock;2 1]

l:lock[`$"/tmp/myResource";10i]

//do something

unlock l
```
