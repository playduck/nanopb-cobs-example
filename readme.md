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
Chose lucky number 4
Encoded message size: 2 bytes
08 04
Writing to file message.bin
--- Decoder ---
Encoded message from file size: 2 bytes
08 04
Your lucky number was 4!
```
