# nanopb-cobs-example

A simple example utility showing how to encode and decode nanopb protobufs with COBS framing.
The framing is required to transmit variable length protobufs over stream interfaces without sidechannels (e.g. SPI).

## Build

> [!NOTE]
> Clone this repository with the `--recursive` flag to download the git submodules or run `git pull --recurse-submodules`.

Build using cmake and python.
To include the python protobuf and grpcio tooling required for nanopb it is recommended to create a venv first:
```bash
python3 -m venv venv
source ./venv/bin/activate
pip install -r requirements.txt
cmake -Bbuild -GNinja && cmake --build build
```

Now find two executables in the build directory:
```bash
./build/nanopb-cobs-example_encode && ./build/nanopb-cobs-example_decode
```
Example output:
```
--- Encoder ---
Chose lucky number 28
Protobuf encoded message size: 2 bytes
08 1c
COBS encoded message size: 4 bytes
03 08 1c 00
Writing to file message.bin
--- Decoder ---
Encoded message from file size: 4 bytes
03 08 1c 00
COBS decoded message size: 2 bytes
08 1c
Your lucky number was 28!
```
