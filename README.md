# SimpleLossy
A simple 2:1 lossy compression for audio

This was a solution to a problem I had with a microcontroller I wanted to play back some audio samples on. The audio samples were 16kB total and the microcontroller had 8kB of Flash. This algorithm compresses an 8-bit PCM data stream to a 4-bit data stream, which can easily be extracted.
