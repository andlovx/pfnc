# Program path by network connection (PFNC)

Cross platform application for resolving binary path of application from its TCP/UDP port number. Outputs application path and related information in selected format.

## Usage

```shell
pfnc -p num [-l|-r]
```

### Options

- -p num: The port to scan.
- -l: Port refer to local side (i.e. the server port).
- -r: Port refer to remote side (the connecting client port).

### Example

List all clients connected to the HTTPS service.

```shell
pfnc -p 443 -r
```

Display information for client connecting from port 32517.

```shell
pfnc -p 32517 -l
```

### Output

The output is done to stdout in tab-separated table format by default. Suppose that we test connecting with netcat against our service listening on TCP port 4053 on localhost, then the output might look like:

```shell
pfnc -p 4053
Origin  Port    PID     Path
Client  50022   72569   /usr/bin/nc.openbsd
Server  60304   53628   /usr/bin/nc.openbsd
Client  60304   53631   /usr/bin/nc.openbsd
```

Other columns and output format can be selected.
