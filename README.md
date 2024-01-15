# Program path for network connection (PFNC)

Cross platform application for resolving binary path of connecting application from its TCP port number. Outputs zero or more paths in table format.

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

The output is done to stdout in tab-separated table format. Suppose that we test connecting with netcat against our service listening on TCP port 4053 on localhost, then the output might be:

```shell
pfnc -p 4053 -r
Origin  Port    PID     Path
Client  41750   472583  /usr/bin/nc
```
