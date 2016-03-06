comment
=======

* "user" and "sys" values of "fork" may be halved due to neglecting of the second process by "time". You should look only on the "real" value.
* If you want to try 1-CPU mode then you can use "tasksel 0x01 ./context-switch-fork". 1-CPU mode should work on "context switch" in many times slower.

system
======

```
model name	: Intel(R) Core(TM) i7 CPU       L 620  @ 2.00GHz
cpus: 4
Linux unnamed 4.3.0-0.bpo.1-amd64 #1 SMP Debian 4.3.3-7~bpo8+1 (2016-01-19) x86_64 GNU/Linux
```

context-switch-fork
===================

```
real	0m4.204s
user	0m4.144s
sys	0m0.028s
```

context-switch-thread
=====================

```
real	0m4.135s
user	0m8.256s
sys	0m0.000s
```

create-and-destroy-fork
=======================

```
real	0m53.622s
user	0m0.752s
sys	0m19.080s
```

create-and-destroy-thread
=========================

```
real	0m4.513s
user	0m0.488s
sys	0m3.784s
```

