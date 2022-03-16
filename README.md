# [Erlang Corba](https://www.erlang.org)

The corba repository contains a set of Erlang applications, that implements a CORBA compliant
Object Request Broker (ORB) and a number of Object Management Group (OMG) standard services.

## Applications

The following applications are a part of the corba package.

* Orber           - the Object Request Broker and the naming service (CosNaming)
* Ic              - IDL compiler
* CosEvent        - event service
* CosEventDomain  - event domain service (management of clusters of information channels)
* CosNotification - notification service
* CosTime         - time service
* CosTransaction  - transaction service
* CosProperty     - property service
* CosFileTransfer - file transfer service

## Build
### Prerequisites

In order to build the `corba` applications, a built OTP source tree of at least
version 23 is needed.

```
export ERL_TOP=<OTP source path>

git clone https://github.com/erlang/otp.git
cd otp
# The autoconf step is not necessary after OTP 24.0.1 and OTP 23.3.4.1
./otp_build autoconf
./otp_build configure
make
```

Ensure that correct OTP is available in the `PATH`.

In order to cross compile `corba` the OTP source tree should be configured for
the cross compilation. For more info on cross compilation of Erlang/OTP see
the document [Cross Compiling Erlang/OTP](http://erlang.org/doc/installation_guide/INSTALL-CROSS.html).

### Compiling the source 

The environment variable `ERL_TOP` *need* to be set to the OTP source path
and then it's possible to compile corba from the source with the following
commands.

```
export ERL_TOP=<OTP source path>

git clone https://github.com/erlang/corba.git
cd corba
make
```

To release the applications set the `RELEASE_ROOT` to a directory of choice and
the run the following command.

```
make release RELEASE_ROOT=<install path>
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

You can also install the `corba` applications directly into an existing
OTP installation either by using the `release` target or the `install`
target. When using the `install` target the `corba` installation will
use the same installation directory as configured in the OTP source tree.

### The documentation build

The documentation is built with the following commands.

```
cd corba
make docs
```

To release it, set the `RELEASE_ROOT` to a directory of choice and
the run the following commands.

```
make release_docs RELEASE_ROOT=<install path>
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

If you want to install the CORBA documentation to be a part of the OTP documentation installation.
Install the `corba` applications docs into the OTP documentation installation directory
either by using the `release_docs` target or the `install-docs` target.
When using the `install-docs` target the `corba` installation will use the same installation
directory as configured in the OTP source tree.
Then you build the OTP documentation and install it into the same directory, the top index will
then include the corba documentation.

## Versions and tags

There is only one version for all applications included in the corba repository.
This means that if one uses the tag `4.5` then all the included applications
have that version, e.g. `orber-4.5`, `cosEvent-4.5` etc.

## Bug Reports

Please visit [bugs.erlang.org](https://bugs.erlang.org/issues/?jql=project%20%3D%20ERL) for
reporting bugs. The instructions for submitting bugs reports [can be found here](https://github.com/erlang/otp/wiki/Bug-reports).

### Security Disclosure

We take security bugs in our applications seriously. Please disclose the issues regarding
security by sending an email to **erlang-security [at] erlang [dot] org** and not by creating a
public issue.

## Contributing

We are grateful to the community for contributing bug fixes and improvements.
Contribution to the corba applications follow the same process as for Erlang/OTP.
We appreciate your help!

## License

Erlang/Corba is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

> %CopyrightBegin%
>
> Copyright Ericsson AB 2018-2022. All Rights Reserved.
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
