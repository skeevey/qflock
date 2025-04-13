# qflock

Glue for flock in q/kdb+

```
{x set`:./qflock 2:x,y}'[`lock`unlock;2 1]

l:lock[`$"/tmp/myResource";10i] //attempt to gain exclusive lock on /tmp/myResource for 10 seconds. Returns file descriptor

//do something

unlock l
```
