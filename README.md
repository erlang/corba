# [Erlang Corba](https://www.erlang.org)

The corba repository contains a set of Erlang components, that implements a CORBA compliant 
Object Request Broker (ORB) and a number of standard services.

## Build
### Prerequisites

To build the *corba* repository one need a built version of OTP (e.g. erlc) and a
configured OTP source tree because the corba make files uses the OTP make system.

```
git clone https://github.com/erlang/otp.git
cd otp
./otp_build autoconf
./configure
```

### Compiling the source

The environment variable *ERL_TOP* should be set to the OTP source path 
and then it's possible to compile corba from the source with the following 
commands.

```
export ERL_TOP=<OTP source path>

git clone https://github.com/erlang/corba.git
cd corba
make configure
make
```

To release the applications set the *TESTROOT* to a directory of choice and 
the run the following commands.

```
export TESTROOT=<install path>

make release
```

And you will get the following structure there so for exmaple the 
environment variable `ERL_LIBS`can be used to get the application into the 
code path (`export ERL_LIBS=<my corba release path>/lib`).

```
lib/
    orber-<vsn>/
    cosEvent-<vsn>/
    cosEventDomain-<vsn>/
     :
     :
     :
```

### The documentation build 

The dokumentation is build with the following commands.

```
cd corba
make docs
```

To release it set the *TESTROOT* to a directory of choice and 
the run the following commands.

```
export TESTROOT=<install path>

make release_docs
```

This gives the following structure on the release path, where 
the top index is `doc/index.html`.

```
COPYRIGHT
doc/
lib/
    orber-<vsn>/
    cosEvent-<vsn>/
    cosEventDomain-<vsn>/
     :
     :
     :
README.md
```

## Versions and tags

There is only one version for all applications included in the corba repository.
This means that if one uses the tag *corba-4.5* then all the included applications
have that version, e.g. orber-4.5, cosEvent-4.5 etc.

## Bug Reports

Please visit [bugs.erlang.org](https://bugs.erlang.org/issues/?jql=project%20%3D%20ERL) for 
reporting bugs. The instructions for submitting bugs reports [can be found here](https://git
hub.com/erlang/otp/wiki/Bug-reports).

### Security Disclosure

We take security bugs in our applications seriously. Please disclose the issues regarding 
security by sending an email to **erlang-security [at] erlang [dot] org** and not by creating a 
public issue.

## Contributing

We are grateful to the community for contributing bug fixes and improvements. 
Contribution to the corba applications follow the same process as for Erlang/OTP.
We appreciate your help!

## License

Erlang/OTP is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE
-2.0).

> %CopyrightBegin%
>
> Copyright Ericsson AB 2010-2017. All Rights Reserved.
>
> Licensed under the Apache License, Version 2.0 (the "License");
> you may not use this file except in compliance with the License.
> You may obtain a copy of the License at
>
>     http://www.apache.org/licenses/LICENSE-2.0
>
> Unless required by applicable law or agreed to in writing, software
> distributed under the License is distributed on an "AS IS" BASIS,
> WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
> See the License for the specific language governing permissions and
> limitations under the License.
>
> %CopyrightEnd%
