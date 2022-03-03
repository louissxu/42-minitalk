# 42 Minitalk

## Final Grade
### 125/100

A 42 Project. Passed with all bonuses.

## Overview

A simple client/server simplex communication service written in C. An exercise in bit manipulation in C and an introduction to the use and handling of Unix signals. See subject.pdf for full requirements and restrictions.

## Implementation

Data is transmitted from client to server using signals. Data string is converted to bit stream and sent to server using user defined signal 1 and 2 to transmit 1 and 0 bits. This is then rebuilt by the running server that is listening for these signals and has appropriate signal handlers set.

## Bonuses

### Unicode Characters

Handle Unicode characters. Unicode characters are encoded to bit stream as previous on the client end. Server side extended to parse variable length Unicode characters. When the first byte is received, it is parsed to determine the number of continuing characters. Then the entire buffer is filled of that number of characters before it is printed out.

### Server Acknowledges Message Receipt

On receipt of entire message (signified by a 0xFF byte sent from the client) the server sends back a byte to confirm message receipt. To allow this, the data string is now accompanied by metadata, in particular to include the client's PID so that the return signals can be sent back to it. The transmission now takes the form of an entire "message" that takes the following format.

#### Message format
| Byte Number | Content                                         |
|-------------|-------------------------------------------------|
| 0           | 0x00 signal byte to signify start of message    |
| 1 to 4      | 4 byte int containing PID of client             |
| 5 to (n-2)  | Bytes encoding the message string               |
| (n-1) to n  | 0xFF 0xFF signal byte to signify end of message |

## Extensions

### Bit Received Acknowledgement

On each bit transmitted by client, once processed by the server the server sends a confirmation signal back to the client. This informs the client that the server is now ready for the next bit.

Signals are not designed to send data in this way. Their transmission/receipt is not guaranteed and they are not buffered. Therefore, if a number of signals are sent in quick succession, then signals can be missed or received in the incorrect order.

The simple solution to this is to add a time delay however the wait time per bit to be safe is unnecessarily long. To get around this, the client waits a period of time, but when the server is happy to receive the next bit then it informs the client by sending a signal in that direction which triggers an interrupt and terminates the wait. This has the benefit of speeding up the message transmission whilst also hardening the client/server interaction.

### Server End-of-Message Realignment

Even with the above procedures occasionally a bit is still lost in transmission. This results in a garbled message. Worse still, the bytes are now misaligned so without correction any further messages sent are also garbled until the server is restarted. To fix this, the end of message signal consists of two consecutive 0xFF bytes. Therefore, no matter what the current byte alignment at least one entire 0xFF byte is received. This is interpreted by the server as signifying the end of message, the server then continues to discard any 1 bits (whatever is remaining of the second 0xFF) and realigns the start of a byte on the next 0 signal. This is why the first byte of a message is encoded as 0x00.
