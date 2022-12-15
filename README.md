# Boggle Solver

Given a dictionary and a random seed, this program will find all valid words (at least 3 letters long) on a Boggle board and returns the words sorted in alphabetical order, along with the total points possible.

## How to compile

```console
$ make onePlayerBoggle
```

If the above fails for some reason, try cleaning first:

```console
$ make clean
$ make onePlayerBoggle
```

## Running solver

```console
$ ./onePlayerBoggle [dictionary] [seed]
```

Example:
```console
$ ./onePlayerBoggle dictionary.txt 200
```
