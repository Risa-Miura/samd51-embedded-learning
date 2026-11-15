# Firmware projects

Small experiments build toward a measured understanding of the hardware. Source code lives under the repository's top-level `projects/` directory; this section is the reading guide.

| Project | Purpose | Current status |
| --- | --- | --- |
| [01 · LED blink](led-blink.md) | Establish Arduino compile/upload workflow | Sketch supplied; hardware verification pending |
| [02 · Timer interrupt LED](timer-interrupt.md) | Replace blocking delays with periodic interrupts | Specification only |
| [03 · ADC sampling](adc-sampling.md) | Acquire and preserve a finite sample buffer | Specification only |
| [04 · DMA benchmark](dma-benchmark.md) | Compare equivalent acquisition modes | Specification and optional analysis template |

A successful build is evidence of toolchain compatibility. Only a documented board run can establish physical behavior. Record measurements with the source commit and configuration.
