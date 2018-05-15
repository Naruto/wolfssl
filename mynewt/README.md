Apache Mynewt Port
=============

## Overview

This port is for Apache Mynewt available [here](https://mynewt.apache.org/).

It provides follows mynewt packages.

- crypto/wolfssl
    - wolfssl library
- apps/wolfcrypttest
    - wolfcrypt unit test application
- targets/wolfcrypttest_sim
    - simulator device for wolfcrypttest

## How to setup

Specify the path of the mynewt project and execute  `wolfssl/mynewt/setup.sh`.

```bash
mynewt/setup.sh　/path/to/myproject_path
```

This script will deploy wolfssl's mynewt package described in the Overview to the mynewt project.

## Customization
### logging

To enable logging, please append `-DDEBUG_WOLFSSL` to` apps.wolfcrypttest.pkg.yml` and `crypto.wolfssl.pkg.yml` in `pkg.cflags:`.
And add dependency of mynewt log modules.

mynewt/crypto.wolfssl.pkg.yml
```yaml
pkg.req_apis:
    ...
    - log
    - stats
    - console
pkg.cflags: -DWOLFSSL_APACHE_MYNEWT ... -DDEBUG_WOLFSSL
```

mynewt/apps.wolfcrypttest.pkg.yml
```yaml
pkg.deps:
    ...
    - "@apache-mynewt-core/libc/baselibc"
    - "@apache-mynewt-core/sys/log/full"
    - "@apache-mynewt-core/sys/stats/full"
pkg.cflags: -DWOLFSSL_APACHE_MYNEWT ... -DDEBUG_WOLFSSL
```

When it executes `wolfcrypttest.elf`, display tty device for output display devices.
please confirm with the `cat /dev/ttys00X` command etc.

## build & test

build

```
cd /path/to/myproject_path
newt clean wolfcrypttest_sim
newt build wolfcrypttest_sim
```

execute wolfcrypt test

```
./bin/targets/wolfcrypttest_sim/app/apps/wolfcrypttest/wolfcrypttest.elf
```